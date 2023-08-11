---
title: "Active Directory 的存储引擎 —— ESE是如何在 AD 中工作的？"
categories: "工作分享"
tags:
  - 数据库
  - 索引
  - index
  - 存储引擎
  - 图数据库
  - 图存储
---

在向完全不了解 AD 的人介绍什么是 AD 时，我常常会说 AD 是一个用于企业资源管理的图数据库，数据库中主要存在两种数据类型：表示对象的 AD object 和表示对象之间关系的 link。 通过对象和对象之间的连接，AD 暴露了一个层次结构，管理了人员、设备、权限、DNS 等各类数据。依赖于 ESE（Extensible Storage）提供的数据存储和查询，AD 基于 LDAP 协议对外暴露了针对这些对象和连接的数据操作能力。

## 什么是 ESE （Extensible Storage）

ESE原名 JET(Joint Engine Technology) Blue，是微软开发的一个支持有索引的顺序访问方法（Indexed and sequential）的数据存储引擎。顾名思义，ESE 支持索引访问和顺序访问。微软有大量的服务都构建在 ESE 上，比如 Active Directory、Exchange Server 等等。ESE 数据库的数据存储在单个文件中，每个文件由一个或者多个用户定义的数据表组成。数据通过一个或多个用户定义的列存储在数据表中。数据索引则可以将数据组织成不同形式。在使用 ESE 的 API 时，调用方可以创建 cursor 来以不同的数据来访问数据。数据表的元素包含以下几项：

- Column: 列，用于存储特定类型的数据，列可以是固定长度或者变长的，取决于存储的数据。某些列可以存储空数据或者多个值
- Records：数据记录，单条数据记录由具有唯一标识作为主键的多个列值组成
- Index：索引是一组定义表中记录存储顺序的关键列。聚集索引（也称为主索引）定义了表内记录存储的顺序。可以定义多个索引以指定对表中记录不同的遍历顺序。索引还可以基于简单的条件（如记录中特定关键列值的存在或缺失）限制可见的记录集。
- Cursor：光标指示表中的当前记录，并使用当前索引导航到表中的记录。光标还包含有关当前准备的更新状态的信息。

# 如何操作 ESE

ESE 通过 handle（句柄）来直接操作数据库，句柄以层次结构维护，如下图所示:
![Alt text](/images/ese_handle.png)
句柄以层次结构进行管理，instance handle 是整个层次结构的根，接下来的层级分别是 session、database、table、column。通过不同层级的句柄，我们可以直接操作对应层级的数据。

# AD 是如何使用 ESE 的索引的？

ESE的数据存储和MySQL类似，底层通过B+树组织数据。数据表中可以定义主键，数据存储会基于主键按照顺序存储。当然，我们可以创建多个索引，非主键的索引可以按照不同的排序方式组织数据，而非主键的二级索引只会通过指针来指向真实的数据记录。由于 AD 本身对外暴露的逻辑结构是层次结构，所以 AD 在使用 ESE 索引时采用了针对这一逻辑结构的索引结构。

## References

[1] https://en.wikipedia.org/wiki/Extensible_Storage_Engine
[2] https://learn.microsoft.com/en-us/windows/win32/extensible-storage-engine/extensible-storage-engine
