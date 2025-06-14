---
title: 读《程序员的 README》
tags:
  - 阅读
  - 技术杂谈
  - 书籍阅读
categories: 书籍阅读
abbrlink: 11e2c87a
date: 2023-09-09 21:58:30
---

最近在挺多地方见到网友推荐这本书，看到微信阅读上面已经有这本书的中文版，抽了点时间看了一下，都说这本书常读常新，我读下来感觉其中的一些观点的确对于程序员的成长有一定的指导作用，尤其是对于新进入职场的新手程序员和一些瓶颈期的程序员，与我工作这几年所沉淀的方法论也有一定的重合。我们系统过一遍，看看书中是如何指导程序员的入门和成长的。

我们按照以下几个部分来过一遍这本书：

- 整体发展路径
- 如何步入自觉阶段
- 如何做好项目变更
- 如何写好代码
- 如何管理依赖项
- 如何进行测试
- Code Review
- 软件交付
- On-Call
- 技术设计流程
- 构建可演进的架构
- 敏捷开发
- 如何与管理者合作
- 职业生涯规划

<!--more-->

## 整体发展路径

对于一个工程师而言，想晋级的话我们必须具备以下四个方向的能力：技术知识 （立身之本，我们需要理解我们的系统，熟悉如何写代码、如何在线上维护我们的代码）、执行力（如何用技术解决问题、如何 On-Call，经常参加技术讲座、阅读小组等）、沟通能力（很多人觉得程序员只要写好代码就好了，但是如何以建设性的方式定义问题、提出问题，如何帮助别人解决问题，如何文档化我们的工作，如何撰写设计文档并征求反馈意见等，这些问题对于一个程序员的精进非常重要）、领导力（从新手程序员到技术专家甚至技术经理的重要能力，我们需要能够在一定的工作范围能自主独立完成工作，处理好各类问题，积极参与项目和季度的规划，帮助团队内其他成员成长等，这些对于我们构建自己影响力都十分重要）。

对于任何一个新人而言，我们可以做的最简单的事情是将我们所经历的（或者说所观察到的、学习到的）流程、内容等各类东西文档化，这样不仅能够帮助我们熟悉整个团队的工作流程，同时也可以帮助我们构建自己的影响力。同时要和 manager 确认我们参加团队和公司的各种重要会议，以获取相应的信息。（公司的组织架构决定了决策信息是自上而下流动的，获取更多的信息能够拥有超过周围人的视野，以做出更加符合公司导向的决策，进而获得各种机会）

这里提到两个点：坎宁安定律和自行车棚效应，坎宁安定律（Cunningham's law）是说“在互联网上获得正确答案的方式不是问问题，而是发布一个错误的答案”。团队里的同事在看到你的文档之后会帮忙修正一些内容，“重点并不是要写一份完美的文档，而是要写得足够多，以引发讨论，充实细节”。我们不应该把这些评论和纠正当做对我们个人的批评。（这里其实有一个小问题，涉及到我们个人的声誉，即我们写出来的东西是否经得起一定的推敲，如果写了过多错误的内容，这对我们个人的声誉也会有一定的影响，所以我们可以有理解的错误，但我们不应该有过多“明显的”、“粗浅的”、“因为不细心而导致的”错误，这个是我的个人观点，书上并没有提到这些）。另一个是自行车棚效应（bike-sheddling, Law of triviality），团队往往对于真正重要的决定草草通过，而对于一些无关紧要的事情吹毛求疵。

在你的成长过程中，持续学习是至关重要的。了解如何编译、测试和部署代码。阅读那些提交代码的请求和代码评审意见。不要害怕询问更多的信息。多报名参加技术讲座、午餐会、阅读小组、导师计划，诸如此类。多和老板 1:1，理解老板的期望并谈谈你自己的目标，这些都很重要。

我们要积极参与各种代码 review 、项目设计和季度规划，这些有助于我们获取更多的信息，进而增加自己的影响力。我们经常出现一些情况，参加了一个会议，但我们并没有真正的参加，即一场会议下来我们并不知道发生了什么，这样既浪费了时间也浪费了我们获取信息的机会。

我们要学习如何交付代码，学习如何维护自己在线上的 feature，这个过程能够帮助我们理解我们的业务场景，理解客户的需求是什么。

接着我们会开始负责一个小的项目，做项目规划，设计相对复杂的系统，这个过程中我们要沉淀对于团队或者公司的工作流的观察和见解，思考哪些是有效的、哪些是可改进的、哪些是无效的，和老板们谈谈自己的看法，同时也可以和老板们谈谈自己的长期规划。

完成上面这些步骤，我们就可以说我们在团队内能够胜任一些工作，贡献自己的价值了。从我个人理解而言，能够稳定达到以上表现，基本就是 senior 及以上的工程师水平了，后续的进一步发展也只是在这些的基础上进一步加深和扩宽自己的领域，基本的方法论是不变的。

## 如何步入自觉阶段

能力的4个阶段：“无意识的无能力”(unconscious incompetence)、“有意识的无能力”(conscious incompetence)，“有意识的有能力”(conscious competence)和“无意识的有能力”(unconscious competence)。我想大多数新手工程师都是在第二阶段甚至是第一阶段，两者的区别在于是否知道自己的努力方向。

**软件工程领域在不断地发展。无论你是一名刚毕业的学生还是一名经验丰富的老手，如果你不学习，你就会落后。**

每周都花一些时间去阅读团队文档、设计文档、代码、work item、书籍、论文和技术网站。在读代码时，如果可以的话我们应该尽量使用 IDE 去读，通过调试来跟踪和理解代码的运行逻辑。通过读团队的文档、work item 和 PR，我们可以知道整个团队目前在做什么工作。

如何提出问题对于个人成长也十分重要，我们使用这样三个步骤：做研究，提出明确的问题，并恰当地安排解决你的问题所需的时间。首先我们必须要自己研究一下，在自己设定的时间限制之内如果没有解决问题，我们可以记录下我们研究的整个过程，在提问时提供给对方，这样对方能知道你做出了足够的努力，也能了解到你到底是哪里没有弄清楚。注意提问时不要打扰他人，通过邮件来提问是一个不错的方式，你也可以礼貌地发一个消息给对方，在一定时间内没得到回复的话，可以友好地 ping 一下。反过来，如果别人在向你提问时，你也可以尽量集中批量处理，避免不断地被别人打断，而浪费自己的大块时间。

有两种典型的成长障碍：冒充者综合征和邓宁-克鲁格效应。

患有冒充者综合征的人无法将自己的成功归因于自己的能力，并总是担心有朝一日会被他人识破自己其实是骗子这件事。他们坚信自己的成功并非源于自己的努力或能力，而是凭借著运气、良好的时机，或别人误以为他们能力很强、很聪明，才导致他们的成功。对于这种情况，我们应该多和别人交流，获取正面的反馈，这样慢慢就能够意识到自己的成绩，并建立信心。

邓宁-克鲁格效应（Dunning-Kruger effect）则完全相反，即过于自信，认为自己的想法是正确的，批评公司的技术栈，即井底之蛙。解决的办法有很多：保持开放的心态，多和大佬交流，在遇到不同意见时多问问为什么会有这样的决策，培养权衡利弊的心态，而不是非黑即白。我自己的经验是，我们依然可以利用上面的坎宁安定律，多输出自己的观点和想法，这样能够在与别人交流的过程中意识到自己的不足。

## 如何做好项目变更

大家如果看看各个公司的内部代码就会意识到，企业的代码质量是参差不齐的，大型项目的代码用“屎山”来形容毫不为过。原因是什么？我们可以理解一下软件的熵（software entropy）这个概念，即软件代码会慢慢走向无序的状态。原因有很多，某些程序员代码写的不好或者业务场景不断变化等。我们可以通过一些方法保持代码的整洁，避免熵增，比如代码风格检测等。

另一个则是技术债（technical debt），经常出现的情况是我们现在要赶着交付一个项目了，所以某些地方就不得不妥协，或者说我们在上线之前并不知道某些场景，所以在写代码时候没考虑到。其实这些都是无可避免的，但是我们可以想办法解决。讨论技术债的模版如下：
1.按事实陈述情况；
2.描述技术债的风险和成本；
3.提出解决方案；
4.讨论备选方案（不采取行动也是备选方案）；
5.权衡利弊。
说白了，有些技术债不一定要解决，取决于解决它带来的收益是什么。

在做代码变更的时候，我们要保证几点：
1. 在做变更时要为我们变更的功能增加测试，小步前进。
2. 保证过手的代码要比之前更干净。
3. 不要一次性做很大的修改，最好慢慢修改，多提交PR，慢慢来，这样既方便 review，又可以避免忽略某些问题，捅大娄子。我个人其实犯过这个错误，在写项目的时候，一个大 PR 看起来是很厉害，但其实一堆小 PR 才更好 track 和 review。
4. 不要为了重构而重构，重构一定要能带来价值，有些代码能干掉的就不要留下来。
5. 用好 git（也有的是 svn），用一个标准模版来提交我们的 PR。如果没有既定的规则，可以遵循克里斯·比姆斯的建议。用一个空行将标题与正文分开。标题行限制在50个字符以内。标题行要大写。不要以句号结束标题行。在标题行中使用命令式语气。将正文限制在72个字符之内。用正文解释修改的内容和原因，而不解释如何修改。

做技术的人很本能地追求最新的技术，但是在做技术选型时我们应该尽量保守，选择比较成熟的方案。不要追求特立独行而特立独行，要学会思考和理解公司内的实现方案，思考其现状的原因，这样才不会无缘无故地搞一些自定义方案。对重构保持谨慎，思考清楚之后再做重构，因为重构的过程往往会超过我们的预期。

## 如何写好代码

写代码这部分其实没什么技术内容，主要是编程习惯。

要做好防御性编程，主要包括：
- 避免空值
- 对不可变的变量要及时声明
- 做好类型检查
- 对输入进行验证，我的经验是，最好所有方法都验证一下（哪怕是 private 方法，因为 private 方法可能被后面在当前文件内做变更的人调用到）
- 用好异常，异常能够返回很多信息，在抛异常和抓异常时最好都有明确定义，这样会安全一些
- “早抛晚捕”，就是说尽可能在接近错误的地方抛异常
- 退避重试，不要不停地重试，尽量以退避的方式重试，避免不断重试，另外为了避免惊群，要给重试的时间间隔加一些随机的延时。
- 构建幂等系统，这个就不说了，避免重试产生错误
- 及时释放资源，资源及时释放能够避免泄露。

## 如何管理项目依赖

在现有的代码上增加一个依赖似乎是一个简单的决定。“不要重复自己”(Don’t repeat yourself，DRY)是一个通常被教导的原则。

一个好的版本管理方案，其版本都具有以下特点：

- 唯一性(unique)：版本不应该被重复使用。构件会被分发、缓存，并被自动化工作流拉取。永远不要在现有版本下重新发布更改的代码。
- 可比性(comparable)：版本应该帮助人们和工具对版本的优先顺序进行推断。当一个构建依赖于同一构件的多个版本时，可以使用优先顺序来解决冲突。
- 信息性(informative)：版本信息区分了预先发布的代码和已发布的代码，将构建流水号与构件相关联，并设置了稳定性和兼容性的合理预期。

语义版本管理(semantic versioning，SemVer)是版本管理中最常用的方案之一。官方的SemVer规范可在其网站中找到。该规范定义了3个数字：主版本号、次版本号和补丁版本号（有时也称作微版本号）。这3个数字被合并为“主版本号.次版本号.补丁版本号”的版本号格式。

如何避免各种循环依赖呢？首先是隔离依赖项，虽然我们经常说 DRY，但是必要情况下，我们要敢于复制代码，避免奇怪的依赖关系。在添加依赖项时，我们应该按需要添加，而不应该一股脑地全都加入到依赖列表中。

## 如何进行测试

常见的测试主要有以下几种：

- 单元测试（Unit Testing）：单元测试是对软件系统中最小可测试单元进行测试的过程。这个“最小可测试单元”通常是指一个函数、方法或者类。单元测试旨在验证单个单元的功能是否按照设计达到预期的结果。它通常由开发人员编写，用于确保代码的每个部分都能够独立地、正确地运行。单元测试通常是自动化的，可以使用各种单元测试框架进行执行。
- 集成测试（Integration Testing）：集成测试是将已经经过单元测试的模块或组件组合在一起，测试它们在一起工作的能力。集成测试的目标是确保各个模块在组合后能够正确地相互协作，不仅仅是各个单元独立运行正确。集成测试可以帮助发现模块之间的接口问题、数据传递问题等。
- 系统测试（System Testing）：系统测试是对整个软件系统进行测试的过程。它的目标是验证系统是否符合需求规格和用户期望。系统测试通常是在模拟生产环境下进行的，包括各种功能测试、性能测试、安全性测试等。系统测试旨在确认整个系统的功能和性能，以及它们是否满足了用户的需求。
- 性能测试（Performance Testing）：性能测试是用来评估系统在不同工作负载下的性能表现。性能测试可以包括负载测试（测试系统在不同用户并发访问下的性能）、压力测试（测试系统在极端工作负载下的性能）、容量测试（测试系统在正常工作负载下的性能，评估系统的容量）等。性能测试帮助开发人员和系统管理员了解系统的性能极限，并确保系统在大规模用户使用时仍然能够提供足够的性能。
- 验收测试（Acceptance Testing）：验收测试是在软件交付给用户或客户之前进行的测试。它的目的是确认软件是否符合用户的需求，并且是否准备好被部署和使用。验收测试通常由最终用户、客户或项目相关的利益相关者执行，用于验证系统是否满足约定的需求标准和质量标准。验收测试可以包括用户验收测试（User Acceptance Testing，UAT）、alpha测试、beta测试等。

## 代码评审（Code Review）

从你的代码上得到的那些批评性的评论可能让你很难接受。切记应该保持一些情感上的距离——这些评审意见是针对代码的，而不是针对你个人的，而且这甚至都不算是你的代码，将来整个团队会拥有这些代码。

不要羞于要求别人评审你的代码。如果你没有得到任何反馈，请向团队报告（但不要催促）

在 review 别人的代码时，不一定只要盯着别人代码里面存在的问题，如果遇到对你有启发的好的代码，也可以通过 comment 让别人知道。

## 软件交付

我们应该尽可能频繁地发布，这样发生回滚时产生的变更也比较小

## On-Call

On-Call工作是依照优先级分类来进行的。P0、P1、P2，依此类推。将工作按类别进行优先级排序有助于界定任务的紧迫性。具体的类别名称和含义因公司而异，但P0任务是大的任务。谷歌云的支持优先级梯队提供了一个如何定义优先级的例子（你可以在谷歌云的技术支持页面找到相关说明）。

- P1：严重影响(critical impact)——服务在生产环境中无法使用。
- P2：高影响(high impact)——服务的使用受到严重损害。
- P3：中等影响(medium impact)——服务的使用部分受损。
- P4：低影响(low impact)——服务完全可用。

事故处理是On-Call人员最重要的职责。大多数开发人员认为处理事故是为了解决生产问题。解决问题确实很重要，但在关键事故中，第一个目标是减轻问题的影响并恢复服务。第二个目标是捕捉信息，以便以后分析问题是如何发生以及为什么发生的。确定事故的原因，证明它是罪魁祸首，并解决根本问题——只是你的第三个目标。
事故响应分为以下5个阶段。

- 分流(triage)：工程师必须找到问题，确定其严重性，并确定谁能修复它。
- 协同(coordination)：团队（以及潜在的用户）必须得到这个问题的通知。如果On-Call人员自己不能解决这个问题，他们必须提醒那些能解决的人。
- 应急方案(mitigation)：工程师必须尽快让事情稳定下来。缓解并不是长期的修复，你只是在试图“止血”。问题可以通过回滚一个版本、将故障转移到另一个环境、关闭有问题的特性或增加硬件资源来缓解。
- 解决方案(resolution)：在问题得到缓解后，工程师有一些时间来喘口气、深入思考，并为解决问题而努力。工程师将继续调查问题，以确定和解决潜在的问题。一旦眼前的问题得到解决，事故也就得到了解决。
- 后续行动(follow-up)：对事故的根本原因——为什么会发生，进行调查。如果事故很严重，就会进行正式的事后调查，或进行回顾性调查。建立后续任务，以防止那个（或那些）根本原因的再次出现。团队要寻找流程、工具或文档中的任何漏洞。在所有的后续任务完成之前，相应事故的处理不应该被认为已经结束了。

重要的是快速解决问题，不要逞英雄

## 技术设计流程

你的首要任务是定义和理解你要解决的那个（或那些）问题。你需要了解问题的边界，以便知道如何解决它，并避免构建错误的东西。

通过实验可以验证一些想法，不要写测试，也不要花时间打磨代码，这一步最重要的是进行验证

尽可能安排大块时间来进行设计，只有这样才能进行深入的思考

设计文档是为了传达你的想法，形成变更的指导方针，如果变更有以下三个特点，那么我们就需要写文档：

1. 该项目将需要至少一个月的工程时间。
2. 这一变更将对软件的扩展和维护产生长期的影响。
3. 该变更将显著影响其他团队。

下面是一个可用的模板：

- 概要；
- 现状与背景；
- 变更的目的；
- 需求；
- 潜在的解决方案；
- 建议的解决方案；
- 设计与架构；
- 系统构成图；
- UI/UX变更点；
- 代码变更点；
- API变更点；
- 持久层变更点；
- 测试计划；
- 发布计划；
- 遗留的问题；
- 附录

要了解团队的设计评审流程，多和团队交流，在交流过程中让别人知道你的想法，进而得到反馈。不要让设计文档成为别人了解你想法的第一步

多为团队的设计工作贡献力量

## 构建可演进的架构

复杂系统有两个特点：高依赖性和高隐蔽性。高依赖性导致软件依赖其他 API 或者代码行为，导致系统很难修改。高隐蔽性使我们很难预测变更的副作用。

YAGNI,You ain't gonna need it。保持代码简单的方式之一是避免什么代码都写出来。避免过早优化，避免不必要的灵活抽象模型，以及避免最小可行产品(minimum viable product，MVP)所不需要的产品特性——你需要那些可以获得用户反馈的最低限度的功能集。

最小惊讶原则：不要让用户感到惊讶，不要让开发者感到惊讶，请使用惯用的代码风格和开发模式

高内聚、低耦合，对专业领域知识进行封装

在做数据管理时，尽量不要采用无模式的方式，最好定义好明确的 schema，降低程序的隐蔽性。

## 敏捷计划

敏捷开发是一种软件开发模型，被广泛采用于快速交付优质软件的场景。了解核心理念和常见的敏捷过程的目标，如冲刺计划、每日站会、评审和回顾，将有助于你有效地实践它们。（当然这不是 996）

Scrum 是一种敏捷（Agile）软件开发框架，用于管理和协作团队开发复杂产品。Scrum 强调迭代和增量的开发，它能够帮助团队更好地应对需求变化，提高团队的灵活性和适应性。以下是 Scrum 框架的主要特点和角色：

**Scrum 框架的特点：**

1. **迭代开发：** Scrum 将项目分解成小的、可管理的部分，每个部分通常为一个迭代，被称为“冲刺”（Sprint）。每个冲刺通常持续两周到一个月，期间团队专注于完成预定的任务。

2. **角色明确：** Scrum 定义了明确的角色，包括产品负责人（Product Owner）、Scrum Master 和开发团队（Development Team），每个角色有特定的职责和权力。

3. **自组织团队：** Scrum 鼓励团队自组织，决定如何完成任务，提高团队的责任感和自主性。

4. **仪式：** Scrum 拥有一系列仪式，包括每日站会（Daily Standup）、冲刺计划会（Sprint Planning Meeting）、冲刺评审会（Sprint Review Meeting）和冲刺回顾会（Sprint Retrospective Meeting）。这些仪式帮助团队保持协作，确保团队成员了解项目的当前状态。

5. **产品待办列表和冲刺待办列表：** 产品待办列表（Product Backlog）是项目需求的总列表，冲刺待办列表（Sprint Backlog）则是每个冲刺的任务列表。

**Scrum 的角色：**

1. **产品负责人（Product Owner）：** 产品负责人是客户、用户和团队之间的桥梁。他们负责定义产品特性、功能和优先级，并且在每个冲刺结束时评估和接受完成的工作。

2. **Scrum Master：** Scrum Master 是团队的敏捷教练，负责确保团队遵循 Scrum 流程，帮助团队解决问题，去除障碍，并持续改进团队的效率和质量。

3. **开发团队（Development Team）：** 开发团队是一组跨职能的专业人员，他们负责实际开发工作，包括设计、编码、测试等。开发团队的成员通常是自组织的，能够完成团队所承担的各项任务。

Scrum 提供了一种结构化的方法来管理和推动软件开发项目，使团队更加灵活、适应性更强，有助于更好地满足客户需求，提高项目的成功率。

## 与管理者合作

（重点来了。。。）

大家虽然老是嘲讽向上管理，但是永远永远永远都要保持和你的管理者的沟通，多和老板聊天，有助于你了解整个工作的方向，让老板了解你的工作进展和存在的问题。多从老板和其他同事那里得到反馈，也要多给予反馈，老板不怕你菜，老板比较怕惊喜（其实多数是惊吓）。

OKR框架是公司定义目标和衡量其是否成功的一种方式。在OKR框架中，公司、团队和个人都定义了目标（目的），并为每个目标附上衡量标准（关键结果）。每个目标都附有3到5个关键结果，它们是标志着目标达成的具体指标。

OKR通常被设定得比合理值略高，以创造“达成”或“延伸”目标的条件。这种理念意味着你不应该百分之百地达成目标的OKR，这是一个表明你设定的目标还不够高的迹象。大多数OKR的实施以60%到80%的成功率为预期目标，这意味着只有60%到80%的目标应被实现。如果你达成了80%以上的目标，你就会丧失进取心；如果低于60%，你设置的目标就不太现实或者你的表现没有达到预期。（为什么不把OKR设定为100%，并奖励超额完成的个人呢？多个进取型的目标可以让你灵活地决定在实施过程中舍弃哪个，而不需要像100%的完成率所预期的那样精确。）请确保你了解你的公司是把OKR当作必须达成的目标，还是有一定预期失败率的附加了进取心的目标！

## 职业生涯规划

成为 T 型人才，这些人既是通才（在一系列广泛的有价值的事情上有很高的技能——T的顶端横线），也是专家（在某个垂直领域中成为佼佼者——T的竖线）。

参加各种训练营，加入感兴趣的项目。

主导你自己的晋升，和老板讨论一下他的期望，看看自己离晋升还有哪些可改进的地方。

学会自我调节，不要过度劳累，保持健康规律的工作节奏。


-----

对于新手程序员，这本书真的很值得看一遍，梳理提到的很多正确的事情浅显易懂而又显而易见，只要实践就能得到成果。知易行难，知行合一是一辈子的修行。