## Berkeley DB -- 软件设计的十点经验

----
[#AOSA]() [#架构设计]()

Berkeley DB是

### 设计经验一 -- 软件必须被设计为多个有边界的互相协作的模块
```
It is vital for any complex software package's testing and maintenance that the software be designed and built as a cooperating set of modules with well-defined API boundaries. The boundaries can (and should!) shift as needs dictate, but they always need to be there. The existence of those boundaries prevents the software from becoming an unmaintainable pile of spaghetti. Butler Lampson once said that all problems in computer science can be solved by another level of indirection. More to the point, when asked what it meant for something to be object-oriented, Lampson said it meant being able to have multiple implementations behind an API. The Berkeley DB design and implementation embody this approach of permitting multiple implementations behind a common interface, providing an object-oriented look and feel, even though the library is written in C.
```
对于任何复杂软件的测试和维护而言，这一点是至关重要的；软件必须被设计和构建为具有明确 API边界的一系列相互协作的模块。这些边界可以灵活调整，但是必须存在。这些边界的存在能够避免该软件变得不可维护。Butler Lampson 曾说过， 计算机科学领域的任何问题都可以通过增加一个间接的中间层来解决。当被问到这在面向对象系统中意味着什么，Lampson 进一步阐述到，这意味着API 背后要能够有多种不同的实现。Berkeley DB 的设计和实现体现了这种允许通用接口背后有不同实现的设计方式，尽管系统是用 C 语言实现的，它仍然能给人一种面向对象的感觉。

### 设计经验二 -- 
```
A software design is simply one of several ways to force yourself to think through the entire problem before attempting to solve it. Skilled programmers use different techniques to this end: some write a first version and throw it away, some write extensive manual pages or design documents, others fill out a code template where every requirement is identified and assigned to a specific function or comment. For example, in Berkeley DB, we created a complete set of Unix-style manual pages for the access methods and underlying components before writing any code. Regardless of the technique used, it's difficult to think clearly about program architecture after code debugging begins, not to mention that large architectural changes often waste previous debugging effort. Software architecture requires a different mind set from debugging code, and the architecture you have when you begin debugging is usually the architecture you'll deliver in that release.
```
软件设计是一种强迫自己在尝试解决问题之前深入思考整个问题的一种方式。有经验的程序员会用不同的方式来达到这个目的：一些人会写下第一个版本然后丢掉，一些人会写下操作手册或者设计文档，另外一些人会尝试填满一个能够定位和结算一些通用方法的代码模版。例如，在编写 Berkeley DB时，我们创建了一套完整的 Unix 风格的说明手册，用来在写代码之前描述这些访问方法和内部模块。不管使用哪种技术，在开始代码调试之后都很难思考清楚程序的架构，更不用说有些大型的架构变化经常会浪费前期的调试工作。软件架构需要与调试代码不同的思维方式。当你开始调试代码时候的软件架构往往就是你最终交付的软件架构。

### 设计经验三 -- 
```
Software architecture does not age gracefully. Software architecture degrades in direct proportion to the number of changes made to the software: bug fixes corrode the layering and new features stress design. Deciding when the software architecture has degraded sufficiently that you should re-design or re-write a module is a hard decision. On one hand, as the architecture degrades, maintenance and development become more difficult and at the end of that path is a legacy piece of software maintainable only by having an army of brute-force testers for every release, because nobody understands how the software works inside. On the other hand, users will bitterly complain over the instability and incompatibilities that result from fundamental changes. As a software architect, your only guarantee is that someone will be angry with you no matter which path you choose.
```
软件架构并不会优雅地衰老。软件架构降级与其中发生 change 的数量成正比，修复 bug 会腐蚀分层，新功能会强迫设计。决定软件何时降级到需要重新设计或者重写一个模块是一个艰难的决定。一方面，随着架构的降级，维护和开发变得更难，最终是一个庞大的软件维护的只能由大量测试者暴力测试来保证，因为没有人会理解软件内部是如何运作的。另一方面，用户会痛苦地抱怨软件的不稳定性和不兼容性，一些来自来自底层的结果会发生变化。作为一个架构师，你唯一能够保证的是总会有人抱怨你的选择，无论你选择了哪条路。

### 设计经验四 --
```
It doesn't matter how you name your variables, methods, functions, or what comments or code style you use; that is, there are a large number of formats and styles that are "good enough." What does matter, and matters very much, is that naming and style be consistent. Skilled programmers derive a tremendous amount of information from code format and object naming. You should view naming and style inconsistencies as some programmers investing time and effort to lie to the other programmers, and vice versa. Failing to follow house coding conventions is a firing offense.
```
无论您如何命名变量、方法、函数，以及使用何种注释或代码风格，这并不重要；也就是说，存在许多足够好的格式和风格。真正重要的是，命名和风格必须保持一致。熟练的程序员可以从代码格式和对象命名中获取大量的信息。您应该将命名和风格的不一致视为一些程序员花费时间和精力欺骗其他程序员的行为，反之亦然。不遵守内部的编码规范是会导致被解雇的违规行为。

### 设计经验五 --
```
Software architects must choose their upgrade battles carefully: users will accept minor changes to upgrade to new releases (if you guarantee compile-time errors, that is, obvious failures until the upgrade is complete; upgrade changes should never fail in subtle ways). But to make truly fundamental changes, you must admit it's a new code base and requires a port of your user base. Obviously, new code bases and application ports are not cheap in time or resources, but neither is angering your user base by telling them a huge overhaul is really a minor upgrade.
```
软件架构师必须慎重选择升级战斗：用户将接受小的变化以升级到新版本（如果您能保证编译时错误，也就是明显的故障，直到升级完成为止；升级变化绝不能以微妙的方式失败）。但是，要进行真正的基本变更，您必须承认这是一个新的代码基础，并需要对用户基础进行移植。显然，新的代码基础和应用程序移植在时间或资源上都不便宜，但是通过告诉用户一个巨大的改进实际上只是一个小的升级来激怒他们，同样也是得不偿失的。