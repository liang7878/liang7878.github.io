---
title: ESE 源码分析
description: "梳理 Extensible Storage Engine 的核心组件、页式存储模型以及事务日志机制，帮助快速理解 Active Directory 背后的存储引擎。"
tags:
  - Active Directory
  - ESE
  - 存储引擎
categories: '工作分享'
date: 2022-07-30 10:10:33
abbrlink: edcfded2
---

ESE（Extensible Storage Engine）是 Active Directory 等 Windows Server 关键组件的底层存储引擎。它以 B+ Tree 为核心组织形式，针对随机读写、高可用和低延迟做了大量工程优化。理解 ESE 的页式结构和缓存机制，有助于定位目录服务中常见的性能瓶颈。

## 页式存储与索引结构

- 物理页大小默认 8KB，每页按照页头、记录区、可用区三段布局，通过 slot table 保持记录的有序。
- 索引逻辑依赖 B+ Tree，内部节点存放键和子页指针，叶子节点保存实际记录；支持聚簇索引和二级索引。
- 记录使用 tag array 管理字段偏移，从而在不搬移整页的情况下完成记录更新和压缩。

## 日志与恢复流程

- 事务采用 ARIES 风格的 Write-Ahead Logging，执行顺序为：生成日志 → 写入日志文件 → 刷脏页。
- 检查点线程周期性将脏页刷盘，并在日志中记录 checkpoint LSN，保证重启时恢复窗口受控。
- 重做阶段以 checkpoint LSN 为起点回放日志，撤销阶段借助 undo list 回滚未完成事务。

## 缓存与并发控制

- Buffer Manager 维护全局页缓存（cache manager），以 LRU-K 变种替换策略提升热点页命中率。
- 会话通过 session + cursor 的方式访问数据，内部使用 latch（轻量自旋锁）保护页级别并发。
- 记录级加锁（record lock）和意向锁（intent lock）组合，既保证一致性又避免锁粒度过大。

后续在阅读 AD 实际堆栈（如 DSA 目录数据库）时，可以据此定位对象落盘位置、分析空洞回收以及事务冲突，这些都是排查生产问题的基础能力。
