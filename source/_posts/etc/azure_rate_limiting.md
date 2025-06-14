---
title: “Azure设计模式 -- 如何设计一个请求速率限制器”
tags:
  - 技术杂谈
  - Azure
  - 阅读
  - 分布式
categories: 工作分享
abbrlink: 6f202b0d
date: 2023-10-27 20:22:30
---

一提到限流算法，大家肯定就是要聊时间窗口、漏斗桶、令牌桶之类的，今天跟着Azure的文档来看看限流器在生产环境怎么搭建和使用。

## 什么是Rate limiting？

假如现在有一个线上的服务，由于业务的增长或者调用方的不正确操作产生了大量的流量，如果我们直接拒绝过量的请求，那么会出现以下的情况：

以Azure Cosmos DB为例，假设我们有一个应用现在要尝试将数据写入Cosmos DB：

1. 应用需要将10k条记录写入Cosmos DB，每条记录消耗10个请求单元，我们一共需要100k个请求单元来完成整个写入的工作
2. 此时Cosmos DB仅有20k个请求单元可供服务
3. 当10k的记录发送到DB时，由于负载容量有限，仅有2k条记录被成功写入，但8k条记录被拒绝
4. 接着8k的记录被发送到DB,2k写入，6k拒绝
5. 接着6k的记录被发送，2k写入，4k拒绝
6. 接着4k的记录被发送，2k写入，2k拒绝
7. 最后2k的记录被发送，全部记录写入完成

可以发现，应用总共发送了30k的记录，但是只有10k的数据被写入DB。这产生了大量额外的请求开销。

与此同时，由于大量请求被拒绝，应用中需要处理20k个错误信息，这也会带来内存、存储等多方面的开销。

由于不知道DB端的限流策略，这种单纯重试的方式也无法给调用方一个预期多久可以完成所有数据的写入。

因此我们需要速率限制器，帮助我们控制一定时间周期内发送到我们服务中的请求数量。

## 如何来做请求速率限制器

我们可以基于不同的指标来决定如何限制请求的速率：

- 可以是请求的数量，比如每秒20个请求
- 可以是数据的大小，比如每分钟3个GB
- 也可以是操作的开销，比如每秒钟20k个请求单元

以上指标都可以用来实现速率控制器。在我们日常的场景里，往往是应用端会接收到大量的请求，因此如何使用负载有限的服务端是应用端必须解决的问题，我们可以简单地将请求缓存在本地，但这也会有进程崩溃导致请求失败的可能性。为了避免这类风险，我们可以把请求放在一个可靠的消息队列服务中，由作业执行器来以一定的速率来读取请求并发送到后端服务，这样就实现了一个速率控制器，提交请求变成了将请求写入消息队列，作业执行器则只会在能够处理请求的时候将请求从消息队列取出。

![Alt text](durable_msging_service.png)

当你发送记录时，你用于释放记录的时间周期可能比服务进行限流的周期更加精细。系统通常根据你可以轻松理解和处理的时间段来设置限流。然而，对于运行服务的计算机来说，这些时间跨度可能相对较长，与其处理信息的速度相比。例如，系统可能以每秒或每分钟为单位进行限流，但通常代码处理的时间单位是纳秒或毫秒。

虽然不是必需的，但通常建议更频繁地发送少量记录以提高吞吐量。因此，你可以比每秒或每分钟批量释放记录更加精细地处理，以保持你的资源消耗（内存、CPU、网络等）以更均匀的速率进行，从而防止由于突发请求造成的潜在瓶颈。例如，如果一个服务允许每秒处理100次操作，速率限制器的实现可以通过每200毫秒释放20次操作来平衡请求，如下图所示。

![Alt text](rate_limited_flow.png)

此外，有时需要让多个不协调的进程共享一个受限制的服务。为了在这种情况下实施速率限制，你可以逻辑上将服务的容量分区，然后使用分布式互斥系统在这些分区上管理独占锁。不协调的进程可以在需要容量时竞争这些分区上的锁。对于进程持有锁的每个分区，它被授予一定数量的容量。

例如，如果受限制的系统允许每秒500个请求，你可以创建20个分区，每个分区允许每秒25个请求。如果一个进程需要发出100个请求，它可能请求分布式互斥系统的四个分区。系统可能授予两个分区的独占锁，持续10秒。然后，该进程的速率限制将为每秒50个请求，任务将在两秒内完成，然后释放锁。

实现这种模式的一种方法是使用Azure Storage。在这种情况下，你可以在容器中为每个逻辑分区创建一个0字节的blob。然后，你的应用程序可以直接针对这些blob获取短时间（例如15秒）的独占租约。对于每个获得的租约，应用程序将能够使用该分区的容量。应用程序随后需要跟踪租约时间，以便在租约到期时停止使用其被授予的容量。在实现此模式时，通常希望每个进程在需要容量时尝试租用一个随机分区，以进一步降低延迟，你可能会为每个进程分配一小部分独占容量。因此，只有在需要超出其保留容量时，进程才会尝试获得对共享容量的租约。

![Alt text](azure_blob_storate.png)

在决定如何实施这种模式时，请考虑以下因素：

1. 尽管速率限制模式可以减少限制错误的数量，但你的应用程序仍然需要正确处理可能发生的任何限制错误。

2. 如果你的应用程序有多个工作流程访问同一个受限制的服务，你需要将它们全部整合到你的速率限制策略中。例如，你可能支持将记录批量加载到数据库中，同时也支持在同一数据库中查询记录。你可以通过确保所有工作流程都通过相同的速率限制机制进行控制，来管理容量。或者，你可以为每个工作流程保留单独的容量池。

3. 受限制的服务可能被多个应用程序使用。在某些情况下，可以协调这种使用（如上所示）。如果你开始看到比预期更多的限制错误，这可能是多个应用程序访问服务之间的竞争的迹象。如果是这样，你可能需要考虑在其他应用程序的使用降低之前，暂时减少你的速率限制机制施加的吞吐量。

使用这种模式可以达到以下目的：

1. **减少受限制服务引发的限制错误：** 通过合理的速率限制策略，可以降低受限制服务引发的限制错误。逻辑上将服务的容量分区，并使用分布式互斥系统管理这些分区上的独占锁，可以确保请求在可用的容量内得到处理，从而减少限制错误的发生。

2. **减少与简单错误重试方法相比的流量：** 与简单的错误重试方法相比，使用速率限制模式可以减少传输的数据量。通过根据可用容量调整请求的发送频率，可以避免将大量请求同时发送到服务，从而降低了网络流量。

3. **减少内存消耗：** 速率限制模式允许在有处理能力的情况下再将记录出队，从而降低了内存的使用。只有当系统有足够的处理能力来处理记录时，才将它们从队列中移除，避免了在内存中保存大量未处理的记录，节省了内存资源。

## Reference

[1] https://learn.microsoft.com/en-us/azure/architecture/patterns/rate-limiting-pattern