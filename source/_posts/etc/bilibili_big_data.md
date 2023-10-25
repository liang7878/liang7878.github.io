---
title: “《B 站大数据体系建设的技术选型与落地实践》直播总结 -- B站大数据离线平台架构演进与实践”
tags:
  - 大数据
  - 技术杂谈
  - OLAP
  - 数据仓库
  - 广告
  - 查询
categories: 技术杂谈
abbrlink: c8fa70e0
date: 2023-10-14 23:31:11
---

这篇是上周五 B 站和 deeplus 技术分享的第一个 session，直播的时候我正在上班，基本全程错过，今晚趁这个时间把第一部分先看一下，我们一步一步来。

- 离线平台整体架构介绍
- 存储架构改造
- 多机房架构建设
- 资源混部建设
- 计算引擎改造
- 未来展望和思考

p.s. 这是在看完视频之后第三遍回顾整个分享的内容，其实分享中的细节很少，但是信息量很大，整个分享基本上涵盖了过去几年 b 站大数据平台的技术演进过程，其中一部分利用了社区的工作，一部分做了定制化的改造或者优化。可惜我本人不是做这个方向，不过很多东西也能够印证我自己工作领域的一些思路。

<!-- more -->

### 离线平台整体架构介绍

首先是 B 站离线平台的整体架构，应用层主要有报表平台、查询平台、数据质量和开发平台，中间服务层通过 Dispatcher 做统一的路由，主要使用了 Presto Gateway（后面对接多套 Presto 集群）、 Kyuubi（处理 Spark 相关的查询） 和 HMS，引擎层使用了 Spark、Hive、Presto 和 Flink。ETL 查询由 Spark 承担，Ad-Hoc 查询通过 Presto。资源层由 Yarn 和 K8S 做资源管理。底层存储使用 HDFS/Alluxio。权限管理整体使用Apache Ranger实现。

![Alt text](offline_arch.png)

```
Note: 
这里贴一些介绍，给不了解大数据平台的同学（包括我自己）

Presto, or Presto database (PrestoDB), is an open-source, distributed SQL query engine that can query large data sets from different data sources, enabling enterprises to address data problems at scale. It gives organizations of all sizes a fast, efficient way to analyze big data from various sources including on-premise systems and the cloud. Presto also helps businesses query petabytes of data using their current SQL capabilities, without having to learn a new language.

Presto 是一个Facebook开源的一套分布式 SQL 查询引擎，可以接入不同的数据源，提供统一的查询体验。这个系统最开始写出来是为了替代 Hive，因为 Hive 在数据量过大的情况下查询效率比较低，无法满足交互式查询的需求，所以 FB 的工程师们开发了 Presto，用于在 HDFS 上执行高效的实时查询。

Presto-gateway 是在多个Presto集群前的一个有状态Load-balancer，Proxy和Router，它提供了透明的访问方法。

Kyuubi 是网易数帆大数据团队贡献给 Apache 社区的开源项目。Kyuubi 主要应用在大数据领域场景，包括大数据离线计算、adhoc、BI等方向。Kyuubi 是一个分布式、支持多租户、兼容 JDBC 或 ODBC 的大数据处理服务。
```

从[这篇文章](https://dbaplus.cn/news-73-4481-1.html)来看，Kyuubi是用来提供多租户能力的。

这里其实我有一个困惑就是，为什么引擎层要用这么多不同的服务？我搜了一下，有下面这样一个对比的表格：

![Alt text](hive_spark.png)

说到底是为了支持不同的查询场景。（还有个猜测是已经上线的业务已经一定程度上被依赖了，所以直接以兼容的方式演进）

目前 b 站大数据平台整体数据量为 1 EB，2 个机房，万台节点，元数据总量 150 亿，Spark/Hive ETL作业数每天 28w，Presto 查询量每天 30w。

### 存储架构改造

存储层面临的挑战：
    - 文件数量快速增长导致单个 NS 接近负载极限，数据量增长不可控，集群水位过高，影响用户作业执行效率
    - 单一机房无法承担所有节点，异地机房的带宽又受到限制
    - 用户任务复杂，无法切割

这里的优化工作主要有三个方面，提升元数据层扩容效率，推动数据治理和建设多机房体系

### HDFS稳定性建设

通过 NNProxy统一 NS 视图（之所以没有用 ViewFS 是因为其对客户端的依赖比较重），NNProxy 可以利用其路由管理功能向用户提供统一的完整的目录树服务，在 mount table 中配置路径到集群的映射信息，用户可以无感访问不同name node，对用户透明的 HDFS Federation上线。

Note: 这里的 NNProxy 说的应该是字节开源的 NameNodeProxy，下面是几种 Hadoop Federation 方案的对比

|| **NNProxy** | **ViewFS** | **WebHDFS** |
| - |- | - | -|
| **Multiple language support**| Yes| No| Yes|
| **Unified APIs provided**| Yes| Yes| No|
| **Mount table stored in**| Zookeeper| Configuration| Configuration|
||| Client-side configurations are not usually easy to change|
| **Client-side library**| No| Heavy| Light|
| **Call latency introduced**| Medium| No latency| High|
| **Centralized cache support**| Yes| Yes| No|

第二是元数据拆分，定制开发了 MergeFS，实时写入不同ns，新的数据写入新的 ns，开发了ns balancer支持元数据自动迁移。

```
这里抄一段 B 站自己文章里面对MergeFS 的介绍：

由于HDFS集群存储数据量的迅猛增长，单个NameSpace已经无法满足元数据量的快速增长，我们在经历了HDFS 联邦机制后扩展成多NameSpace，满足了一段时间的需求。但是随着集群元数据量的指数级增长，特别是小文件数量的猛增，HDFS 联邦机制逐渐无法满足当时的需求。

为了能够快速新增NameSpace以及让新增的NameSpace迅速接入已有的集群，负载新增的元数据，现有的联邦机制和社区版本的RBF也无法满足当前的要求，我们决定在RBF的基础上，深度定制开发，来解决主节点扩展性问题。

当时元数据层的压力主要来源于3个方面：

- 存量元数据数据量大，新增文件数量增长迅猛。

- 新增NameSpace无法快速进行迁移，迁移效率不足。

- 大量目录存在实时写入，历史的迁移方式需要停止写入。

为了解决元数据层扩展能力不足的问题，经过调研社区3.0的HDFS Router和业界相关方案后，我们决定在社区3.3版本的HDFS Router 的基础上，进行定制开发MergeFs来解决集群元数据层扩展性能问题。

在社区版本的HDFS Router 基础上，定制化开发MergeFS支持元数据迁移，MergeFS 支持按一个挂载点配置2个NameSpace，新写入数据会按规则路由到新增的NameSpace中，但历史数据仍然可见，通过这种方式，我们能迅速扩张新的NameSpace，缓解老NameSpace的写入压力。

建设了NameSpace Balancer工具，能在业务低峰时期自动化的异步迁移老NameSpace的历史数据到新扩容的NameSpace中，迁移完成后收归掉挂载点，最终实现路径完全迁移到新的NameSpace中。

基于HDFS元仓，不断分析出增长较快的目录，用于指导哪些数据需要迁移。

在支持了接入层的MergeFS后，元数据扩张不再成为瓶颈，我们扩容了14组NameSpace，支持了90亿左右的元数据总量，迁移了54亿左右的元数据。与此同时，整个集群的元数据层QPS得到了极大的提升，整体QPS从50K/s上涨到177.8K/s，整个数据迁移工作对上层数据计算任务透明，极大的减少了迁移的工作量，迁移工作量从1人/week，下降到0.1人/week。

```
第三是对稳定性和性能做了优化，稳定性上因为有了 NNProxy，这里做了一个多维度的 RPC 限流（用户、作业、请求类型、 ns），对 Renew Lease 优化，对资源开销较大的操作做了 meta 缓存，另外就是 FileSystemNamespace 和 BlockPool 做了锁的拆分（提高了 50%的 QPS）， 和 INode 锁拆分（预计还能把 QPS 再提升 50%）。

下图介绍了如何拆分 ns，通过将一个挂载点映射成两个namespace，老的数据写到老的 namespace ，新写的数据写到新的 namespace，接着通过 snapshot 将数据从老的 ns 异步迁移到到新的 ns。同时可以通过元仓分析找出增长较快的目录来做 ns balancer 的迁移。

![Alt text](merge_fs.png)

下图是ns拆分的成果，首先 QPS 有了明显的上涨，NS 数量从 4 组扩容到了 30+组。

![Alt text](merge_fs_result.png)

考虑到 ns 的容量瓶颈，他们上线了 Observer来分担读的压力，基于 RBF 架构，计算引擎通过 CallerContext 透传是否进行 Observer 读请求。

```
Note：这里我看了一下所谓的 Observer NameNode解决方案，其实本质上就是一个读写分离。
```

Alluxio 缓存读加速，热表/热分区直接从 Alluxio 读，缓存加速。基于元仓分析出热表，自动添加热表挂载点，加载表/分区数据到 Alluxio 集群，上线后热表读性能提升 2-3 倍。

Note：为什么 Alluxio 可以加速呢？

### HDFS 容量挑战

![Alt text](hdfs_capacity.png)

首先上线了冷备压缩的解决方法，通过每日分析Fsimage 文件，将 HDFS 审计日志入仓，结合 Hive 的元仓信息得到整个 HDFS 文件信息宽表，为数据治理提供数据指导。

```
Note: FSImage 是 NameNode 中关于元数据的镜像，一般称为检查点的镜像；会在内存和磁盘中各保存一份；
```

通过 EC的解决方案降低存储成本，通过更小的存储副本降低成本，“the default 3x replication scheme in HDFS has 200% overhead in storage space and other resources (e.g., network bandwidth).” “a natural improvement is to use Erasure Coding (EC) in place of replication, which provides the same level of fault-tolerance with much less storage space. In typical Erasure Coding (EC) setups, the storage overhead is no more than 50%.”

基于 HDFS 元仓自动化数据转化。

冷数据定义：
![Alt text](cold_data.png)

### 多机房架构建设

多机房建设的背景

![Alt text](multi_datacenter.png)

解决方案

![Alt text](multi_datacenter_solution.png)

单元化改造

![Alt text](unit.png)

数据如何迁移？

![Alt text](data_migration.png)

数据如何路由？

![Alt text](data_router.png)

这里提到了可以通过 IP 位置感知来决定将任务路由到对应机房，这部分的工作我没太明白为什么要这么做。

### 资源混部建设

混部架构的背景和挑战

![Alt text](mix_deployment.png)

B站离线平台资源调度侧的主要挑战有两个方面:

1. 随着业务的不断增长，离线集群规模快速膨胀，用户对资源的需求在持续增大，主集群长期处于Pending较高的状态，资源需求超过交付量

2. 出于降本增效的考虑，消解Pending的方法不能仅靠物理机的增加了，而是需要在物理机整体数量不变的基础上通过超卖来提升集群整体的资源利用率。

为了应对上述挑战，调度侧在向内与向外两个方向上进行了积极的探索。“向内”聚焦于单台物理机，通过超配的方式不断提高单台物理机的利用率，使得单台节点能够处理更多的任务；“向外”与云平台部门合作，共同探索混部技术的落地，到目前为止，已经完成了离线超配，离在线混部、在离线混部等集群建设以及潮汐混部的技术实现，使得不同集群间的资源能够被更充分地调动。

超配组件所依赖的主要理论是“用户申请的资源量一般大于用户真实使用的资源量”。根据这个理论，实现超配组件的主要思路是，根据当前机器的实际负载情况，向调度组件虚报一定的资源量，使得更多的任务能够被调度到该台机器上运行。然而，这种做法也带来了一定的风险。在极端情况下，机器上运行的大部分任务的用户申请量会接近于用户真实使用的资源量。这种情况下，超配组件需要及时发现并响应，驱逐一定量的任务以保证机器整体的稳定运行。因此，超配组件必须具备智能管理的能力，能够根据机器实际的负载情况和任务的资源需求，动态调整超配量，以保证机器整体的稳定性和可靠性。同时，超配组件还应该具备良好的容错性和监控机制，能够及时发现和处理机器故障或异常情况，保障业务的连续性和稳定性。总之，超配组件虽然能够带来更高的资源利用率，但也需要合理使用和管理，以避免带来潜在的风险和损失。

![Alt text](overload.png)

潮汐混部

![Alt text](tide.png)

### 计算引擎改造

![Alt text](compute_bg.png)

升级 Spark，这个过程主要做了一个 AB Test，预估迁移后的收益。

![Alt text](ab_test.png)

基于历史信息的任务参数优化

![Alt text](hbo.png)

数据组织

![Alt text](data_strcture.png)

智能诊断

![Alt text](auto_diagnose.png)

HMS 优化

![Alt text](hms.png)

### 未来展望和思考

![Alt text](future.png)

### Reference

- [1] https://hive.apache.org/
- [2] https://kyuubi.apache.org/
- [3] https://ranger.apache.org/
- [4] https://github.com/bytedance/nnproxy
- [5] https://lrting.top/backend/6009/
- [6] https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/ObserverNameNode.html
- [7] https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HDFSErasureCoding.html
- [8] http://blog.itpub.net/70027827/viewspace-2954254/
- [9] https://github.com/lyft/presto-gateway
- [10] https://www.cnblogs.com/163yun/p/16833191.html
- [11] https://ahana.io/learn/comparisons/hive-vs-presto-vs-spark/
- [12] https://dbaplus.cn/news-73-4481-1.html
- [13] https://bilibili.com/read/cv15897101/