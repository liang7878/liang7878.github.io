---
title: Active Directory 介绍
tags:
  - 数据库
categories: ' 工作分享'
abbrlink: 1f41e82f
---

其实我们真的是个数据库。

<!-- more -->

每次和朋友聊到我们使用的数据库，他们都会一脸问号。什么是 [Active Directory(AD)](https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/get-started/virtual-dc/active-directory-domain-services-overview)？AD 不是用来做权限管理的吗？每次我都要解释，我们是一个非关系型数据库，是构建在 ESE 引擎上，支持 LDAP 协议的一个数据库，数据的逻辑视图是树形层次结构的。其实我自己对 AD 的了解也仅限与此，更多一点，可能知道AD 可以提供 DNS 服务，可以提供 Kerberos 验证。今天这篇文章，我就大概先讲一下什么是 AD，内容会有一些中英夹杂，因为很多单词翻译出来和我在工作中的表达方式有所不同，翻译之后会略显生硬，所以使用英文以保留其原意。

维基百科对 AD 的解释是[“Active Directory (AD) is a directory service developed by Microsoft for Windows domain networks. ”](https://en.wikipedia.org/wiki/Active_Directory)。Directory 是一种存储网络对象的层次结构。Directory Service提供了一套存储访问 directory数据的方法，让这些数据能够被网络用户和管理员访问到。

AD 里面存储这各种各样的数据，比如服务器、打印机、网络用户、计算机账户等各种数据。AD 提供了安全的方式（登录验证、访问控制）让用户来访问这些数据。

接下来我们从几个方面讲一下 AD：

- AD 的整体结构
- AD 里有哪些角色
- AD 的 schema是什么
- AD 如何存储和访问数据

## AD 的整体结构

从存储结构来讲，AD 里面主要包含四个部分：

- AD 的 domain 和 forest。forest 是 AD 里面的安全边界，domain、forest 和 organizational unit 是 AD 逻辑结构的核心元素
- DNS。AD 还提供了DNS 域名解析服务
- Schema。数据的 schema
- Data store. 负责 AD 里面的数据存储和检索。

![Active Directory Data Structure and Storage Architecture](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2003/images/cc759186.ccf65c10-edb1-4a3a-ad87-38775ee43b8a(ws.10).gif)

在 forest 下面还有 domain，domain 可以把 forest 中的数据几家看进一步的分割，这样可以更灵活地控制数据的复制同步，这部分我们在下次可以详细讲一下。而 OU 则可以将 domain 下的资源进一步分割，便于更加灵活的管理。下面这张图是forest、domain、OU 的逻辑结构

![Logical Structure Architecture](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2003/image2s/cc759186.a55b8656-0958-4681-88bd-86a6f27a4451(ws.10).gif)

## AD的角色
通常来讲，一个安装有 AD 服务的windows 服务器就被称为一个 domain controller，简称 DC。正常来说，DC 上面存有其所在 domain下面一个 domain partition的所有数据。事实上一台 DC 可以保存一个或者多个 domain partition。其实有一些DC会扮演特殊的角色，提供一些特殊的功能，主要分两大类，一是 Global Catalog Server，另外就是各种 Operation Master。

首先我们来看看 Global Catalog Server，简称 GC。如上文所说，一个 DC 里面存有其所在 domain 里面的所有数据，而 GC 则活跃在 forest 层面。G 上一般保存着它自己所在 domain 的所有的可写的 replica 数据，和部分forest 内其他 domain 的可读数据。GC 里面的数据由 AD 的 replication 机制来自动复制。这些可读数据可以用来搜索到对应的对象。可读数据一般是一个 object 的某些 attribute，这个可以通过修改 schema 来进行配置。一般来讲，一个 forest 里面的第一台 DC 会被自动配置成 GC，在加入更多DC 之后，我们也可以指定其他 DC 来作为 GC。

另一类就是各种 master 了，这些 master 负责执行一些任务来保证特定数据的一致性。AD 的 master 主要有五个：schema master、domain naming master、relative identifier（RID）master、primary domain controller（PDC）emulateor 和 infrastructure master。我们把它们叫做 FSMO(Flexible Single Master Operation) roles。

AD 是一个多主数据库，非常灵活，我们可以在 forest 里任何一台 dc 上做数据的更新，但是，这同样也导致了一些数据冲突的产生。AD 可以通过比较版本号和时间戳的方式来解决一些冲突，但是有时候也会出现无法解决冲突的情况。所以在执行一些特殊的操作，比如说 domain 的添加和删除、或者密码修改的时候，AD 会以一种单主的方式来运行。在单主模式下，只有一台 DC 被允许执行更新的过程。上面提到的各种 master 就是在这个时候用到，用来负责处理不同类型的更新。

下面讲一下不同的 master 的职责:

- Schema master。顾名思义，这个角色主要是为了处理一些 schema 上的更新。schema 的更新只能发生在拥有这个角色的机器上，更新完成后被复制到其他的 DC 上。
- Domain naming master。这个角色负责处理 forest 范围内的 domain 更新。
- RID master。负责处理来自domain 下所有 DC 的 RID Pool 请求。当 DC 创建一个安全主体对象（用户或者组）时，它会给这个对象一个唯一的 Security ID(SID)，SID 由两部分组成，一部分是 domain SID，用于区分 domain，另一部分则是一个 relative ID(RID)，用于区分 domain 下的对象。domain 下的每个 DC 都有一个 RID pool，用于给自己创建的对象赋予一个 RID，当 一个 DC 的RID pool 剩余的RID 不够时，会向 domain 的 RID master 申请额外的 RID。每一个 domain 下面有且只有一个 RID master。
- PDC emulator。这个角色主要负责同步时间。Kerberos 协议需要基于时间戳实现，整个组织内部的时间按照层次结构来逐层同步，这个 role 就是整个时间同步结构的根，它会从外部数据源同步时间。在 windows domain 下，这个角色同样还承担着一些其他和 auth相关的功能。
- Infrastructure master。这个角色主要负责更新 object 在跨 domain 对象引用中的 SID 和 DN。

## AD 的 schema 是什么
AD 里面的数据是以对象为单位来进行存储的。Schema 就是这些对象的格式，我们在存储一个对象之前，需要将这个对象的格式定义在 AD 的 schema 中，每个对象由许多不同的属性组成，比如 name、guid、whenCreated、whenChanged等等。这个有点类似于关系数据库里面对表结构的定义，但是又略有不同。也可以理解为一个 mongoDB，但是里面所有 object 的结构都是预先定义好的。

## AD 如何存储和访问数据
AD 使用 LDAP 协议来访问数据，在执行搜索时，通过指定 search base，可以将查询范围限定在层次结构的某个对象之下，通过 LDAP filter可以指定对应的查询条件。

AD 的数据存储在 ESE database中，ESE 全称 Extensible Storage Engine(ESE)，也叫 JET Blue。某种程度上，AD 就是一个访问 ESE 数据库的客户端，以存储计算的方式来分层的话，AD 相当于一个计算层，ESE 作为存储层。AD 还会提供 replication 的能力，将数据在一个 forest 内互相复制、同步。从数据库的架构角度来说，AD 属于 Shared Everything 的结构，不同节点之间并不共享任何计算和存储的资源。与 MySQL等其他数据库类似，ESE 底层使用 B+树组织所有的数据。

之后我会写几篇文章详细介绍AD 的复制同步和存储是如何实现的。