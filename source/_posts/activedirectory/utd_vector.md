---
title: AD 的 Replication
description: "总结 Active Directory 多主复制的逻辑、UTD Vector 的含义以及常见拓扑调优策略，便于快速排查目录同步延迟。"
tags:
  - Active Directory
  - Replication
  - UTD Vector
categories: '工作分享'
date: 2022-07-30 10:10:33
abbrlink: 8a34612
---

Active Directory 采用多主复制模型，同一对象可以在任一可写域控上修改，然后通过拓扑同步给其他域控。理解复制管线的关键有三个：

1. **拓扑与站点链接**：知识一致性由 KCC（Knowledge Consistency Checker）生成的拓扑保障；站点链接成本决定哪台域控之间优先复制。
2. **更新跟踪**：每条属性修改会生成 USN（Update Sequence Number）和 Originating DSA（源域控），跨域控同步时需要借助高水位线来比较谁落后。
3. **冲突解决**：若同一属性被并发修改，比较版本号（`version`）、时间戳（`timeCreated/timeChanged`）和源域控 GUID，最终写入胜出的副本。

## UTD Vector 的作用

- **Up-To-Dateness Vector** 是域控级别的高水位线表，记录 “我已经收到某域控的最大 USN”。
- 复制会话开始前，提供方会基于 UTD Vector 过滤已同步的条目，减少冗余流量。
- 如果域控长时间离线导致 UTD Vector 过旧，重新上线时会触发初始同步（full sync）。

## 常见调优与排障思路

- 使用 `repadmin /showrepl` 检查最近一次复制时间、是否存在 pending replication。
- `repadmin /showutdvec` 可以打印 UTD 向量，快速识别落后的域控。
- 日志 ID 2042（Event Viewer → Directory Service）提示域控超出 Tombstone 生命周期，需要执行强制同步或重建副本。
- 跨站点链路延迟大时，可调大 `repadmin /options` 中的压缩、批次参数，或通过 RODC 减少写路径。

掌握这些信息后，可以更有底气地解释 “AD 为什么还没同步到其他站点” 这类问题，并且在扩容或异地容灾场景中做出合理规划。
