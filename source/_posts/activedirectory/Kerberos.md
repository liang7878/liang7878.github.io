---
title: Kerberos 详解
description: "概览 Kerberos 双向认证流程、票据结构与 Active Directory 中的实际落地，聚焦排查常见登陆问题需要的关键细节。"
tags:
  - Kerberos
  - 身份认证
  - Active Directory
categories: '工作分享'
date: 2022-07-30 10:10:33
abbrlink: 1ec0428
---

虽然 Kerberos 看起来像古老的协议，但在企业网络里它依然是最可靠的单点登陆方案。一次成功的认证往往只需要几百毫秒，却牵涉 AS、TGS、应用服务器三方交互：

1. **AS-REQ / AS-REP**：客户端向 KDC（Key Distribution Center）申请 TGT，凭借的是预共享的密码学凭据（通常来自用户密码的散列）。
2. **TGS-REQ / TGS-REP**：携带 TGT 向 TGS 申请针对具体服务主体的 Service Ticket。
3. **AP-REQ / AP-REP**：客户端把 Service Ticket 交给服务端，完成最终的双向认证与会话密钥协商。

值得关注的细节：

- **票据结构**：TGT 由 KDC 私钥签名，服务票据则由目标服务账户的密钥签名；两者内部都包含客户端 ID、有效时间和 session key。
- **时钟同步**：Kerberos 默认允许 ±5 分钟的时间漂移，NTP 配置不到位会直接导致登陆失败。
- **SPN 与委派**：服务主体名称（SPN）映射着域内服务账户，配置错误会触发 `KRB_AP_ERR_MODIFIED`；而委派（delegation）决定票据能否被转递。

调试思路：

- 使用 `klist`、`setspn`、`ktpass` 等工具确认票据是否按预期生成。
- 在域控查看系统日志（Event ID 4768、4769、4771）快速定位认证失败原因。
- 对安全敏感场景，引入 FAST（Flexible Authentication Secure Tunneling）和 AES256 算法，避免 NTLM 降级风险。

掌握这些基础，可以在排查 “无法登陆域”、“跨林访问失败”、“SQL Server Kerberos 认证异常” 等常见问题时少走弯路。
