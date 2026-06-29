---
title: {{ title }}
lang: zh-CN
# permalink 决定专栏语义化 URL（脱离全局 abbrlink）。英文版改为 en/mental-models/<slug>/
permalink: mental-models/<slug>/
# alternate 指向另一语言版本，供中英切换链接 + hreflang 读取。英文版指回 /mental-models/<slug>/
alternate: /en/mental-models/<slug>/
# categories = 该模型唯一主 layer（§5）：Resources/Systems/Complexity/Agents/Information/Decision/Organization/Society
categories:
  - Mental Models
  - <layer>
# tags：4 级 tag 扁平合并进原生 tags（§13）。建议顺序 discipline → ai_domain → engineering → application
# discipline(1~3) ai_domain(2~4) engineering(1~3) application(1~2)
tags:
  - <discipline>
  - <ai_domain>
  - <engineering>
  - <application>
date: {{ date }}
---

{# ============================================================
   AI Engineer's Mental Models — 文章模板（spec v1.2 §6）
   13 段标准模板。对本模型不适用的 section 可整段删除（不写 N/A，不凑数、不注水）。
   建议每篇必有：One-liner / Question / Core Idea / AI Mapping / Related Models / Key Takeaway
   写作风格（§7）：Explain, don't impress. 大量 Why/How/What-if + 直觉 + 真实案例 + 图 + 比喻。
   配图（§8）：黑白极简，Mermaid / SVG。用 {% mermaid %}...{% endmermaid %} 包裹。
   ============================================================ #}

> **One-liner（一句话定义）**
> 中文：……
> EN: …… *(中英各一句)*

<!-- more -->

## Question

<!-- 用一个真实的 AI 工程问题开头。例：为什么 GPT 推理越来越便宜，GPU 却越来越紧张？ -->

## Origin

<!-- 谁、何时、为何提出。只写关键背景，不堆历史。不适用可删本段。 -->

## Core Idea

<!-- 为什么会发生？强调直觉，不用数学。建议配一张 Concept Diagram： -->

{% mermaid %}
graph LR
  A[...] --> B[...]
{% endmermaid %}

## Real-world Example

<!-- 经典案例（煤炭 / Uber / 航空 / 互联网…）。不适用可删本段。 -->

## AI Mapping

<!-- 全篇最重要。从 LLM / Agent / Infrastructure / Product / Evaluation / Training / Inference / Cloud
     中挑“真正贴切的”若干个举例，不要求 8 个全填，牵强的宁可省略。 -->

## Engineering Perspective

<!-- 从 Infra / Agent / PM / Researcher 视角各自“应该想到什么”。不强制 4 角色全写。不适用可删。 -->

## Investment Perspective

<!-- 投资领域如何体现。§3.1 护栏：结尾必须绕回“这对设计/运维/评估 AI 系统意味着什么”。不适用可删。 -->

## Life Perspective

<!-- 生活案例。同样遵守 §3.1 护栏，绕回 AI。不适用可删。 -->

## Misconceptions

<!-- 最容易误解什么。不适用可删。 -->

## When It Doesn't Apply

<!-- 何时失效？哪些条件下不成立？培养批判性思维。不适用可删。 -->

## Related Models

<!-- 知识关联的主要载体（§9）。每个相关模型都用站内链接指向其文章，而非纯文字： -->
<!-- - [Opportunity Cost](/mental-models/opportunity-cost/) -->
<!-- - [Network Effect](/mental-models/network-effect/) -->

## Key Takeaway

<!-- 一句值得长期记住的话（中英）。 -->
> 中文：……
> EN: ……
