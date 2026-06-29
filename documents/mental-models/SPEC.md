# Project Spec — AI Engineer's Mental Models

Version: v1.1
Status: Draft for review
Supersedes: v1.0 (the original ChatGPT brainstorm)

> 本文件是专栏的"契约"。所有文章、目录、模板、工程落地都以此为准。
> v1.1 相对 v1.0 的改动集中在第 5、6、10、11 节,以及新增第 13、14 节。
> 每一处与 v1.0 不同的决定，都在 §0 决策矩阵里标注了来源。

---

## 0. Decision Matrix（v1.1 锁定的决定）

这一节是 v1.1 的核心。所有决定均来自与作者的逐条讨论确认。

| 维度 | 决定 | 说明 |
|------|------|------|
| 站点归属 | `liang7878.github.io` 下的新专栏 | 复用现有 Hexo + NexT + gh-pages 部署，不另起独立站点 |
| 语言 | 中英**双全文** | 每篇真正产出中、英两个完整版本（不是骨架双语） |
| 双语落地 | **i18n 双目录（方案 B）** | `/mental-models/<slug>/`（中）+ `/en/mental-models/<slug>/`（英），配 `language_switcher` 与 hreflang |
| URL / slug | 英文 slug + 语义化路径 | 走 `/mental-models/<slug>/`，脱离博客现有 `posts/:abbrlink.html` |
| 单篇模板 | 13 段完整模板 | 对某模型**不适用的 section 可整段省略**，不强行凑写、不注水 |
| Related Models | 写在正文，用**站内链接**指向相关模型文章 | 不进 front-matter；为 Phase 4 的"从链接反推图"留后路 |
| 检索系统 | 4 级 tag（Discipline / AI Domain / Engineering / Application） | 见 §13，负责检索与间隔复习 |
| 知识关联 | tag + 正文链接 | **不引入** `related` front-matter 字段（作者明确决定） |
| 章节结构 | 8 层（World→…→Society） | **一模型一主 layer**：每个模型只归一个章节，跨章关系靠 tag + 正文链接表达 |
| 翻译分工 | Orchestrator 出另一语言初稿 → 作者润色定稿 | 决定协作角色：AI 起草，人定稿 |

### 0.1 关于"知识图谱"的诚实说明（重要）

v1.0 第 9 节反复强调 "Graph not Tree" 和 "交互式知识图谱"。v1.1 在数据层面的现实是：

- **采用 tag 系统**（4 级），它负责的是"分组 / 集合 / 共现"——例如"所有 `discipline: Economics` 的文章"。
- **不引入结构化的 `related` 边字段**（作者决定）。因此"模型→模型的有向语义链路"（如 `Jevons → Scarcity → Marginal Utility`）**不会以机器可读的图边形式存在**。
- 这种关系改为**以正文 `Related Models` 段 + 站内超链接**的形式承载：读者能点、能读、能跟随，覆盖了约 80% 的"知识关联"价值。
- **代价**：Phase 4 若要做"全局知识图谱可视化"，数据源将是「tag 共现」+「从正文链接反向抽取的边」，而非一个预先声明好的图。届时再用脚本处理，不阻塞 Phase 1~3。

> 一句话：v1.1 用「tag 分组 + 正文链接」务实地承载知识关联；放弃了「预声明的有向图边」。这是有意识的取舍，不是疏漏。

---

## 1. Vision（沿用 v1.0）

打造一个长期更新（5~10 年）的知识专栏，用跨学科的 Mental Models 帮助 AI 工程师理解现代 AI 系统。

这个专栏**不是**：经济学教程 / 博弈论教材 / 心理学百科 / 科普合集。

而是：

> 用跨学科理论解释 AI 系统、Agent、LLM Infrastructure、产品设计、投资决策和复杂世界。

最终目标不是写 100 篇文章，而是建立一套属于 AI 工程师的知识地图。

一句话定位：

> **Build better AI systems by understanding how the world works.**

---

## 2. Target Audience（沿用 v1.0）

**主要读者**：AI Infrastructure Engineer · AI Application Engineer · Agent Developer · LLM Engineer · Distributed System Engineer · ML Engineer · Technical PM

**扩展读者**：创业者 · 产品经理 · 投资者 · 技术管理者 · 对 AI 感兴趣的软件工程师

---

## 3. Philosophy（沿用 v1.0，三大原则不变）

**Principle 1 — 不要介绍理论，解释世界。**
不要写"什么是杰文斯悖论"，而要写"为什么 AI 推理越来越便宜，但 GPU 永远不够？"。理论只是解释工具，不是目的。

**Principle 2 — Everything maps to AI.**
任何一个理论都必须映射到 AI（至少能解释 LLM / Agent / Infra / 产品 / 投资中的若干个）。不能解释 AI 的，不收录。

**Principle 3 — Mental Models > Definitions.**
不要记概念，建立思维方式。读者遇到新问题时，应自动联想到对应的 Mental Model，而不是背定义。

### 3.1 定位护栏（v1.1 新增）

为防止专栏从"AI 工程师思维工具箱"漂移成"什么都聊的科普"，对跨界 section 设一条硬纪律：

> **任何 Investment / Life Perspective 的案例，都必须在结尾绕回"这对设计 / 运维 / 评估 AI 系统意味着什么"。** 跨界是为了照亮 AI，而不是为跨界而跨界。

---

## 4. Long-term Goal（沿用 v1.0）

- 100+ Mental Models
- 200+ 原创文章
- 500+ 真实案例
- 1000+ 知识连接（以 tag 共现 + 正文站内链接的形式实现，见 §0.1）

---

## 5. Knowledge Architecture（v1.1 去重修订）

整个专栏按"理解世界的层次"组织，而非按学科。8 个 layer：

```
World → Resources → Systems → Complexity → Agents → Information → Decision → Organization → Society
```

> **一模型一主 layer 原则**：每个模型只归属一个 layer（即 Hexo category）。v1.0 中跨章重复的模型已在此去重并指定唯一归属（见各章 ★ 标注）。它在其它语境下的作用，靠 `ai_domain` / `application` tag + 正文链接表达，而不是再写一篇。

### 第一章 — Resources（资源如何配置？）
学科：Economics · Operations Research · Optimization

- Scarcity
- Opportunity Cost
- Marginal Utility
- Utility Maximization
- Supply & Demand
- Pareto Optimality
- Jevons Paradox
- Externality
- Comparative Advantage
- Economies of Scale
- Economies of Scope

### 第二章 — Systems（系统如何运行？）
学科：Systems Thinking · Cybernetics

- Feedback Loop
- Positive Feedback
- Negative Feedback
- Delay
- Bottleneck
- Constraint
- Second-order Effect
- Leverage Point
- Flywheel
- Goodhart's Law

> ★ 去重：`Conway's Law` 从本章移除，唯一归属到 **第七章 Organization**（它本质是"组织结构决定系统结构"，更贴近组织层）。

### 第三章 — Complexity（复杂系统）
学科：Complexity Science · Network Science

- Emergence
- Long Tail
- Power Law
- Network Effect ★（唯一归属本章）
- Scale-free Network
- Self Organization
- Tipping Point
- Cascade Failure

> ★ 去重：`Network Effect` 在 v1.0 中同时出现在第三章和第八章。v1.1 唯一归属 **第三章 Complexity**（它的数学本质是复杂网络/正反馈）。其在平台经济中的体现，由第八章相关文章**正文链接回**本篇，并打 `application: Product Design` 等 tag。
> ★ 去重：`Winner Takes All` 视为 `Power Law` / `Network Effect` 的推论，**并入** Power Law 篇的 AI Mapping / Real-world，不单列（除非后续发现独立价值）。

### 第四章 — Agents（多个智能体如何合作？）
学科：Game Theory · Decision Theory · Mechanism Design

- Prisoner's Dilemma
- Nash Equilibrium
- Coordination Game
- Tragedy of the Commons
- Mechanism Design
- Auction Theory
- Signaling
- Screening

> ★ 去重：`Principal-Agent Problem` 在 v1.0 中出现在第四章和第七章。v1.1 唯一归属 **第七章 Organization**（它是组织/激励问题）。第四章如需引用，正文链接过去即可。

### 第五章 — Information（信息如何传播？）
学科：Information Theory

- Entropy
- Information Bottleneck
- Mutual Information
- Channel Capacity
- Noise
- Signal
- Compression
- Redundancy

### 第六章 — Decision（如何做决策？）
学科：Statistics · Behavioral Economics · Decision Theory

- Bayesian Thinking
- Expected Value
- Regression to the Mean
- Survivorship Bias
- Loss Aversion
- Confirmation Bias
- Anchoring
- Availability Bias

### 第七章 — Organization（组织为什么这样运作？）
学科：Management · Organizational Theory

- Incentive
- Principal-Agent Problem ★（从第四章迁入，唯一归属本章）
- Conway's Law ★（从第二章迁入，唯一归属本章）
- Brooks' Law
- Theory of Constraints
- OODA Loop
- PDCA

### 第八章 — Society（平台 / 市场为什么这样演化？）
学科：Economics · Finance · Innovation

- Platform Economy
- Lock-in
- Switching Cost
- Market Design
- Innovation Diffusion

> ★ 去重：`Network Effect` 不在本章单列（见第三章）。本章涉及网络效应处，正文链接回第三章。

### 5.1 去重汇总表

| 模型 | v1.0 出现章节 | v1.1 唯一归属 | 理由 |
|------|---------------|---------------|------|
| Network Effect | 三、八 | **三 Complexity** | 数学本质是复杂网络 |
| Principal-Agent | 四、七 | **七 Organization** | 本质是组织激励问题 |
| Conway's Law | 二、七 | **七 Organization** | 组织结构 → 系统结构 |
| Winner Takes All | 三 | 并入 Power Law | 是幂律/网络效应的推论 |

---

## 6. Article Template（v1.1：13 段完整模板，可省略不适用段）

> **规则**：以下 13 段为标准模板。**某段对当前模型不适用时，可整段省略**（不写 "N/A" 占位，直接不出现该段）。判断标准：宁可少写，不可注水。每篇的核心价值在 **Core Idea** 与 **AI Mapping**。

1. **Title** — 模型名（中英）。
2. **One-liner（一句话定义）** — 中英各一句。例：*Jevons Paradox：Efficiency improvements often increase total resource consumption.*
3. **Question** — 用一个真实的 AI 问题开头。例：*为什么 GPT 推理越来越便宜，GPU 却越来越紧张？*
4. **Origin** — 谁、何时、为何提出。只写关键背景，不堆历史。
5. **Core Idea** — 为什么会发生？最好配图（§8），强调直觉，不用数学。
6. **Real-world Example** — 经典案例（煤炭 / Uber / 航空 / 互联网…）。
7. **AI Mapping**（全篇最重要）— 从 LLM / Agent / Infrastructure / Product / Evaluation / Training / Inference / Cloud 中**挑真正贴切的若干个**举例。**不要求 8 个全填**；牵强的映射宁可省略。
8. **Engineering Perspective** — 从 Infra / Agent / PM / Researcher 视角各自"应该想到什么"。**不强制 4 个角色全写**，写真正有洞见的。
9. **Investment Perspective** — 投资领域如何体现（须遵守 §3.1 护栏，绕回 AI）。
10. **Life Perspective** — 生活案例（须遵守 §3.1 护栏，绕回 AI）。
11. **Misconceptions** — 最容易误解什么。例：Jevons 不是"效率提高**一定**增加需求"，而是"**可能**，需条件"。
12. **When It Doesn't Apply** — 何时失效？哪些条件下不成立？培养批判性思维。
13. **Related Models** — 列出相关模型，**每个都用站内链接**指向对应文章（这是知识关联的主要载体，见 §0.1）。
14. **Key Takeaway** — 一句值得长期记住的话（中英）。例：*Efficiency doesn't eliminate scarcity. It often reshapes it.*

> 注：第 14 段 Key Takeaway 与第 1 段 Title 一样属于"每篇必有"。真正可省略的是 4、6、8、9、10、11、12 这些视模型而定的段。Question / Core Idea / AI Mapping / Related Models / Key Takeaway 建议每篇都有。

---

## 7. Writing Style（沿用 v1.0）

目标风格：**Paul Graham × Chip Huyen × Charlie Munger × Anthropic Engineering Blog**。

共同特点：不用术语炫技 · 不用长篇数学 · 大量 Why / How / What-if · 直觉 · 真实案例 · 图 · 比喻。

核心信条：**Explain. Don't impress.**

### 7.1 双语写作约定（v1.1 新增）
- 中、英为**两个完整版本**，分别位于 i18n 双目录（§14）。
- 工作流：Orchestrator 先出**另一语言初稿**，作者**润色定稿**（默认：作者先写中文 → AI 出英文初稿 → 作者定稿；反向亦可）。
- 术语：两版各自地道。中文版正文可内联英文术语；英文版不夹中文。
- 两版的 slug 相同（英文 slug），靠目录前缀区分语言。

---

## 8. Visual Style（v1.1 明确技术依赖）

每篇建议至少包含：① Concept Diagram ② AI Mapping 图 ③（可选）局部关系示意图。

统一风格：**黑白 · 极简 · Mermaid / Excalidraw / SVG**。不要复杂配色。

> 技术依赖：现有 `_config.next.yml` 中 `mermaid.enable: false`，专栏落地时需开启（见 §14）。图中文字 label 在中英两版分别用对应语言。

---

## 9. Knowledge Linking（v1.1 取代 v1.0 的"Knowledge Graph"）

> v1.0 此节标题为 "Knowledge Graph"。基于 §0.1 的决定，v1.1 更名为 **Knowledge Linking**，以如实反映其实现方式。

知识关联通过两条途径实现，**均不依赖** `related` front-matter 字段：

1. **Tag 共现**（§13）：4 级 tag 让"同学科 / 同 AI 领域 / 同工程场景 / 同应用面"的文章可被聚合检索。
2. **正文站内链接**：每篇的 `Related Models` 段（及正文中任何提及其它模型处）用站内链接指向目标文章，形成读者可跟随的路径。

**链接纪律**（保证未来可从链接反推图）：
- `Related Models` 段内每个模型名都必须是指向该模型文章的站内链接，而非纯文字。
- 链接使用稳定的语义化路径（`/mental-models/<slug>/`），不使用易变的 abbrlink。

---

## 10. Blog Structure（v1.1：i18n 双目录 + 语义化路径）

```
source/_posts/mental-models/          # 中文版（默认语言 zh-CN）
  resources/
    jevons-paradox.md                 # → /mental-models/jevons-paradox/
    opportunity-cost.md
  systems/
  complexity/
  agents/
  information/
  decision/
  organization/
  society/

source/en/_posts/mental-models/       # 英文版（en）
  resources/
    jevons-paradox.md                 # → /en/mental-models/jevons-paradox/
  ...
```

（具体目录形态以 §14 技术落地验证为准；上图为意向结构。）

**每篇页面应具备**：上一篇 / 下一篇导航 · Related Reading（即 Related Models 链接）· 语言切换（中⇄英）。

**专栏门户页**（计划）：
- `/mental-models/`（中）与 `/en/mental-models/`（英）的索引页：README + Roadmap + 8 层导航。

---

## 11. Roadmap（v1.1 重排：先地基，再批量）

> v1.0 的 Phase 1 直接是"20 篇基础模型"。v1.1 在前面插入 **Phase 0 地基**，理由：用 1 篇标杆把"双语 + 模板 + i18n + mermaid + 部署"整条管线跑通，比先规划 20 篇空标题更能验证模式是否成立。

- **Phase 0 — 地基（Foundation）**
  - 敲定本 spec v1.1（本文件）。
  - 搭技术脚手架：i18n 双目录 + 专栏语义化 permalink（与现有 abbrlink 共存）+ 开启 mermaid + 文章 scaffold + 语言切换。
  - 产出 **1 篇标杆样板**：建议 `Jevons Paradox`（v1.0 中例子最完整），中英双全文，跑通从写作到部署的全流程。
  - 验收：标杆文章在中英两个 URL 正常渲染、图正常、tag 生效、Related 链接可点、语言可切换、SEO（hreflang/canonical/description）正确。

- **Phase 1 — 基础模型（~20 篇）**：真正理解世界。优先第一、二、三章里最具代表性的模型。

- **Phase 2 — AI Mapping 深化（~50 篇）**：覆盖到 LLM / Infra / Agent 的系统性映射。

- **Phase 3 — Knowledge Linking 织网**：回填所有文章的 Related Models 站内链接，形成可跟随的阅读路径网。

- **Phase 4 — Interactive Version**：基于 tag 共现 + 从正文链接抽取的边，做交互式可视化（数据源见 §0.1）。

- **Phase 5 — Book**：整理成《AI Engineer's Mental Models》。

---

## 12. Success Criteria（沿用 v1.0）

这个专栏成功，不是因为"介绍了 100 个概念"，而是因为：

> 任何一个 AI 工程师，在遇到一个新的系统问题时，都能想到：**"我以前在哪个 Mental Model 里见过这种结构？"**

最终希望它成为一本 AI 工程师的《思维工具箱》——不是教人背知识，而是帮助人建立一种能跨越计算机科学、经济学、博弈论、复杂系统、心理学和投资的统一思考框架。

---

## 13. Tagging Specification（v1.1 采纳，作为检索与复习系统）

> 来源：作者提供的 Tagging Spec，v1.1 原样采纳。tag 的目标是**知识检索与间隔复习**，不是博客分类。
> 与 §5 的关系：§5 的 `layer` 是文章的**唯一主分类（category，树）**；本节的 4 级 tag 是**跨维度的多标签（扁平，可多选）**。两者并存、各司其职。

### Level 1 — Discipline Tags（学科）
```yaml
discipline:
  - Economics
  - Game Theory
  - Information Theory
  - Statistics
  - Operations Research
  - Systems Thinking
  - Complexity Science
  - Psychology
  - Behavioral Economics
  - Management
  - Organizational Theory
  - Philosophy
  - Decision Theory
  - Biology
  - Network Science
  - Finance
  - Cybernetics
```

### Level 2 — AI Domain Tags（AI 领域）
```yaml
ai_domain:
  - LLM
  - Agent
  - Multi-Agent
  - AI Infrastructure
  - AI Platform
  - AI Product
  - Prompt Engineering
  - RAG
  - Reasoning
  - Evaluation
  - Reinforcement Learning
  - Inference
  - Training
  - Model Serving
  - GPU Scheduling
  - Routing
  - Memory
  - Tool Calling
  - Workflow
```

### Level 3 — Engineering Topics（工程场景）
```yaml
engineering:
  - Distributed Systems
  - Scheduling
  - Resource Allocation
  - Scalability
  - Load Balancing
  - Reliability
  - Optimization
  - Caching
  - Queueing
  - Observability
  - Networking
  - Storage
```

### Level 4 — Application Tags（AI 之外的应用）
```yaml
application:
  - Investing
  - Product Design
  - Startup
  - Management
  - Career
  - Learning
  - Decision Making
  - Negotiation
  - Everyday Life
```

### 数量约束（每篇典型值）
- discipline: 1~3
- ai_domain: 2~4
- engineering: 1~3
- application: 1~2

### 设计原则
- 一个 mental model 不应只属于一个学科；**鼓励跨学科打标**。
- 目标是构建可检索、可复习、可（后期）可视化的知识网，而非教科书目录。

### 示例

**Jevons Paradox**
```yaml
discipline: [Economics, Complexity Science]
ai_domain:  [Inference, AI Infrastructure, GPU Scheduling]
engineering: [Resource Allocation, Scalability]
application: [Investing, Product Design]
```

**Prisoner's Dilemma**
```yaml
discipline: [Game Theory, Decision Theory]
ai_domain:  [Multi-Agent, Tool Calling]
engineering: [Resource Allocation]
application: [Negotiation, Management]
```

**Information Bottleneck**
```yaml
discipline: [Information Theory]
ai_domain:  [LLM, Prompt Engineering, RAG]
engineering: [Optimization]
application: [Learning]
```

---

## 14. Technical Landing（v1.1 新增：工程落地与已知约束）

> 本节记录"现有博客里开专栏"涉及的真实技术工作量与约束。这些不阻塞 spec，但 Phase 0 必须逐一验证。

### 14.1 已知约束（如实告知）
1. **专栏语义化 permalink 与现有 abbrlink 共存**
   - 博客其它文章保持 `posts/:abbrlink.html`。
   - 专栏走 `/mental-models/<slug>/`，需用 per-post `permalink:` front-matter 覆盖或目录级 permalink 规则实现。Hexo 支持，Phase 0 验证配置。
2. **NexT i18n 双语（方案 B）**
   - "同一篇文章中英两版互切" 在 Hexo/NexT 中**非开箱即用**，通常靠目录约定 + 自定义 language switcher 链接。
   - Phase 0 需调研 NexT 8.x i18n 当前最佳实践，确认 `/` 与 `/en/` 的目录、`language: [zh-CN, en]`、`language_switcher`、以及 hreflang 注入的具体落地。
3. **mermaid 默认关闭**
   - `_config.next.yml` 中 `mermaid.enable: false`，专栏需开启。
4. **SEO 衔接**（与上一轮 SEO 优化对齐）
   - 双语需 `hreflang` 互指；canonical 各自指向本语言 URL；feed/sitemap 已就绪（上一轮已配置 `hexo-generator-feed` + sitemap + lazyload）。
   - 自定义 `source/_data/head.njk` 已承载 JSON-LD/OG，专栏文章应复用；如需 hreflang，可能在 head.njk 增量。

### 14.2 Front-matter 模板（意向，Phase 0 定稿）
```yaml
title: "Jevons Paradox"
layer: Resources                    # §5 的唯一主分类（→ category）
slug: jevons-paradox                # 英文 slug，中英两版一致
lang: zh-CN                         # 英文版为 en
permalink: mental-models/jevons-paradox/   # 英文版加 /en 前缀
discipline: [Economics, Complexity Science]
ai_domain:  [Inference, AI Infrastructure, GPU Scheduling]
engineering: [Resource Allocation, Scalability]
application: [Investing, Product Design]
date: 2026-xx-xx
```
> 注：是否用 `categories: [Mental Models, Resources]` 形成两级分类，Phase 0 定。tag 字段名（discipline/ai_domain/...）是否合并进 NexT 的 `tags` 还是作为自定义 taxonomy，Phase 0 验证。

---

## 15. Open Questions（留待 Phase 0 解决，不阻塞当前）
1. NexT i18n 的具体落地路径（方案 B 的工程细节）。
2. 4 级 tag 用 Hexo 原生 `tags` 扁平承载，还是注册自定义 taxonomy？前者简单、后者更结构化。
3. 专栏门户页（`/mental-models/` 索引）的形态与生成方式。
4. `categories` 两级结构（Mental Models / <layer>）是否与现有博客分类体系冲突。
5. 双语 sitemap / hreflang 的注入点（head.njk 还是插件）。

---

## Changelog
- **v1.1** — 锁定 11 项核心决定（§0）；§5 去重并指定一模型一主 layer；§6 改为"完整模板可省略不适用段"；§9 "Knowledge Graph"→"Knowledge Linking"并如实说明取舍；§10 改为 i18n 双目录 + 语义化路径；§11 新增 Phase 0 地基；§13 采纳 4 级 tag 系统；新增 §14 技术落地与约束、§15 待解问题。
- **v1.0** — 初始 brainstorm（作者 × ChatGPT）。
