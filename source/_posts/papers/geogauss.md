---
title: '我的七周七数据库 -- GeoGauss: Strongly Consistent and Light-Coordinated OLTP for Geo-Replicated SQL Database'
tags:
  - Paper Reading
  - 数据库
categories: 论文阅读
abbrlink: c995a12e
date: 2023-11-20 21:31:59
---

今天要分享的文章是SIGMOD2023华为的一篇文章，主要介绍的是全球多区域场景下如何实现满足 强一致性需求的 OLTP 数据库。
文章整体结构如下：
![[paper_arch.png]]


我们首先简单跟着论文理解一下跨区域数据库这一场景， 接着看一下 GeoGauss的整体架构和具体实现，最后看一下其与其他数据库的实验对比。

## 场景介绍
跨国公司往往需要在全球范围内构建自己的数据中心，以满足全球业务的需求。这一场景下数据库产品的设计目标有三点：高可用、强一致和高性能。

高可用的解决方案主要有两种，如下图所示：
![[geodb_solution.png]]
一种是数据分片、