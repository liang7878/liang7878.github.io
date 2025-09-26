---
title: etcd 源码分析 - Raft 是如何实现的
description: "从 etcd 的 raft 模块拆解角色切换、日志复制与快照落盘流程，帮助在面试或排障时快速回答一致性相关的问题。"
tags:
  - etcd
  - Raft
  - 分布式系统
categories: '面试分享'
date: 2022-07-30 10:10:33
abbrlink: 40b3bac4
---

etcd 的一致性核心完全由 `raft` 模块实现，该模块既可以单独嵌入，也可以作为库被 TiKV、CockroachDB 等项目复用。源码中最值得关注的几个部分如下。

## 状态机与角色切换

- `raft.StateType` 定义了 Follower、Candidate、Leader 三种角色，状态转换由 `tick` 例程驱动，分别处理心跳、选举和 leader 检测。
- 随机化的选举超时时间（`randomizedElectionTimeout`）防止惊群；一旦 `tickElection` 触发，节点会自增任期并发送 `MsgVote`。
- 任期变化通过 `raft.reset(term)` 完成，清空未提交日志和待处理消息，保证新任期干净启动。

## 日志复制与提交

- Leader 将写请求封装为 `raftpb.Entry`，先写入内存日志（`raftLog`），再通过 `MsgApp` 发送给跟随者。
- 跟随者返回 `MsgAppResp` 指明匹配的索引，Leader 更新 `Progress` 结构中的 `Match` / `Next`，并根据多数派推进 `committed`。
- 提交给上层状态机前，要把已提交日志通过 `Ready.CommittedEntries` 暴露，由应用层串行执行。

## 快照与持久化

- 为防止日志无限增长，`raftLog.maybeCompact` 会在应用层创建快照后裁剪前缀。
- 快照落盘由 `Snapshotter` 负责，重放时 `raft.Restore` 校验 `Term/Index`，确保只在更高的日志位置应用快照。
- WAL 层（`etcdserver/wal`）负责日志持久化，采用滚动文件 + CRC 校验，崩溃恢复时通过 `decoder` 回放。

## 工程上的小技巧

- etcd 将网络 IO 和状态机逻辑彻底分离：`raft.Node` 只处理消息，`raft.RawNode` 暴露给上层调度；真正的发送和存盘由 `raft/raft_node.go` 中的事件循环完成。
- Leader 批量处理心跳与日志（`MsgBeat`、`MsgAppBatch`），减少系统调用；同时对大日志尺寸启用 `Inflights` 滑动窗口，避免跟随者积压。
- 单元测试大量使用 table-driven pattern，通过伪造消息序列模拟网络分区、延迟、重复消息等 corner case。

在面试场景中，围绕 “如何保证日志不回退”、“如何处理网络分区”、“如何减少快照停顿” 等问题，配合上述源码细节，就可以从架构和实现两个层面回答得比较扎实。
