# TiDB 查询计划优化，Optimize 是怎么实现的

查询优化器可以说是 DBMS 的灵魂模块了，这篇文章会简单讲一下 TiDB 的查询优化模块，从`planner/optimize.go`的 `Optimize`方法出发，逐步拆解 TiDB 查询优化的整个过程。

方法的定义如下,前两个参数是 context，我们先不用管，接着是抽象语法树的`node`，以及用于读取 schema 信息的接口。本方法会返回一个执行计划和`fieldName`的 slice。
```go
func Optimize(ctx context.Context, sctx sessionctx.Context, node ast.Node, is infoschema.InfoSchema) (core.Plan, types.NameSlice, error)
```

下面来详细看一下整体的实现思路，这是一个两百多行的超长方法，里面还有一些边边角角的实现，我们尝试来理解一下整个实现过程。

进入方法后，首先会做一些 session variable 的检查，比如`InRestrictedSQL` 比如`isolationReadContainTiFlash`，这些检查与一些具体 feature 有关，我们暂时可以先不用管。

接着来到一个 Optimize 方法`OptimizeExecStmt`, 如果当前 node 是一个 [`EXECUTE Statement`](https://dev.mysql.com/doc/refman/5.7/en/execute.html)，那我们就直接调用`buildLogicalPlan`方法构建逻辑计划，这部分就涉及到 `planner`部分的源码，我们之后再对这里进行分析。在构建完成逻辑计划后，再调用`GetPlanFromSessionPlanCache`来获取一个执行计划，若 session 的 plan cache 中存在对应的 plan，则从 cache 直接返回，否则就调用`generateNewPlan`方法来生成一个新的 plan。

我们来看它是如何生成一个新的 plan 的。首先这里会尝试记录一个 cache miss 的 metric（当然这是很小的一个点，但是 metric 其实是生产环境中诊断系统健康状态的重要工具，我们看代码的过程中同样要学习如何打 metric），然后将读出的 `PreparedAst`进行优化。这样就又回到了 Optimize 方法里面。完成优化生成 plan 后，紧接着会尝试调用`tryCachePointPlan`来判断当前计划是否是一个`point execution plan`，并尝试将其 cache 下来。下面是 `PointGetPlan`的注释：

```go
// PointGetPlan is a fast plan for simple point get.
// When we detect that the statement has a unique equal access condition, this plan is used.
// This plan is much faster to build and to execute because it avoid the optimization and coprocessor cost.
```

简单来说，`PointGetPlan`就是用于执行存在唯一的 equal 条件的查询语句的查询计划。

紧接着，这里会对当前计划进行判断，确认是否可以讲计划存入 cache，这里有一系列不同类型的物理计划不允许 cache。我们后面可以单独研究一下哪些计划可以放入 cache，哪些不行。判断可以放入 cache 的 plan，这里会生成对应的 key 和 value，然后放入 cache，并将对应的 plan 返回。

接下来到了比较具体的优化部分，首先从 node 中提取出 `tableHints`,并对语句中的所有 hint 进行处理。完成处理后，如果这里是一个 point plan，函数会直接尝试 `PointGetPlan`来执行当前查询。接着获取 `TxnManager`，尝试`AdviseWarmup，这部分会在对 Txn 的分析里涉及到。

这里接着做了一个`useBinding`的检查，我本来不知道这个是什么，后来查了一下之后发现其实就是去为特定语句绑定查询计划，比如指定索引之类的。这种操作能够人为查询过程中使用的索引，确保查询计划按照预期的方式生成。

接下来最重要的`optimize`方法来了，这个方法就是执行整个优化的关键方法了。首先，语法树节点会`Accept`一个基于当前 session 的 context 生成的`hintProcessor`(这个操作我没太明白是在做什么事情，可以后面再看)，接着，我们从`planBuilderPool`里面取出一个 builder，用于生成逻辑计划，比较有意思的是这里对 builder 使用了`sync.Pool` 进行了复用和保存，`builder`在完成初始化后，会被拿来构建逻辑计划，这里用到`buildLogicalPlan`方法，这里会基于对应语句来构建执行计划。构建后逻辑计划后会执行权限检查，接着会做表锁的检查。通过上述检查后，如果当前 plan 不是逻辑计划，则会直接返回；如果是，则尝试使用瀑布计划器进行优化，找到最优的计划，并返回最佳计划。如果没有开启瀑布计划器，则会调用 `DoOptimize`方法进行优化，并返回最终计划。

上面的`optimize`方法有三个方法需要进一步分析，这三个方法应该就是整个查询优化最核心部分的内容了。

1. `buildLogicalPlan`：基于不同的语句类型构建初始的逻辑计划，涉及的语句比较多。以最基本的`SELECT`语句举例，在构建计划之前会做一些准备工作，比如对语句的检查、是否涉及 join 操作、是否有是`SELECT FOR UPDATE`等等，在构建计划时，先会创建一个基于 `FROM`语句的带有 data source 的 plan，这里面涉及到一些view 的检查、partition 的检查，接着会基于`FROM`的不同数据源构建，如果是一个 table，那么就会去找到一些可能可以使用的索引，作为 `IndexHints`等信息放入`DataSource`中，`DataSource`中还会有一些诸如`Column`、`PossibleAccessPath`等信息，并将其作为 planer 返回，返回后会做一些特定的处理，比如`SORT`、`LIMIT`等。里面的操作相对来说比较详细，后面可能会针对某个语句执行的整个生命周期写一篇文章，这样可能更好理解整个过程。
2. 瀑布优化器的`FindBestPlan`：这个方法的注释非常完整，我们看起来也比较轻松。下面是注释的内容：

```go
// FindBestPlan is the optimization entrance of the cascades planner. The
// optimization is composed of 3 phases: preprocessing, exploration and implementation.
//
// ------------------------------------------------------------------------------
// Phase 1: Preprocessing
// ------------------------------------------------------------------------------
//
// The target of this phase is to preprocess the plan tree by some heuristic
// rules which should always be beneficial, for example Column Pruning.
//
// ------------------------------------------------------------------------------
// Phase 2: Exploration
// ------------------------------------------------------------------------------
//
// The target of this phase is to explore all the logically equivalent
// expressions by exploring all the equivalent group expressions of each group.
//
// At the very beginning, there is only one group expression in a Group. After
// applying some transformation rules on certain expressions of the Group, all
// the equivalent expressions are found and stored in the Group. This procedure
// can be regarded as searching for a weak connected component in a directed
// graph, where nodes are expressions and directed edges are the transformation
// rules.
//
// ------------------------------------------------------------------------------
// Phase 3: Implementation
// ------------------------------------------------------------------------------
//
// The target of this phase is to search the best physical plan for a Group
// which satisfies a certain required physical property.
//
// In this phase, we need to enumerate all the applicable implementation rules
// for each expression in each group under the required physical property. A
// memo structure is used for a group to reduce the repeated search on the same
// required physical property.
```

- 首先是预处理阶段。在这个阶段会对整个计划树做一个预处理，使用一些启发式规则进行优化，比如列裁剪。（我以为这里有很多优化规则，不过看方法只有一个列裁剪。。）
- 接着是探索阶段。探索阶段会尝试对每一组表达式的所有等效表达式进行探索。在最开始的时候，一个 group 里面只有一组表达式，在执行一些转换规则之后，我们会找到所有的等效表达式并存入 Group 中。这个过程可以被当做在一个有向图里找一个弱连接组件，在这个图里，节点是表达式，边是各种转换规则。
- 最后一个阶段是实现阶段，目标是找到一个最优的物理计划。这里会在要求的物理属性下对每一组的每一个表达式都遍历所有可用的实现规则。这里会有一个 memo 结构体，用于减少重复的搜索。

3. `DoOptimize`：这个方法是为了将逻辑计划优化成一个物理计划。这里会应用很多的优化规则，优化规则存储在`optRuleList`里面，我们会在下一篇文章里谈到。

```go
在数据库领域，逻辑计划（Logical Plan）和物理计划（Physical Plan）是查询执行的两个关键阶段。它们表示了查询在执行过程中的不同层次和优化方式。

逻辑计划是描述查询逻辑操作的计划，它不依赖于具体的执行环境和数据存储方式。逻辑计划通常由查询优化器根据用户提交的查询语句生成，它是对查询语句的语义解析和语义优化的结果。逻辑计划描述了查询的逻辑操作流程，包括涉及的表、连接方式、过滤条件、聚合操作等，但不关注具体的执行细节和物理存储结构。

物理计划是根据逻辑计划和执行环境的具体情况生成的计划，它依赖于具体的数据库系统、存储引擎和硬件设备。物理计划是优化器根据逻辑计划和系统统计信息等综合考虑的结果，它将逻辑计划转化为实际的执行操作。物理计划考虑了具体的执行代价、数据访问路径、索引的选择、并行度等因素，以最优的方式执行查询操作。

总结来说，逻辑计划关注查询的逻辑操作流程和语义，独立于具体的执行环境；而物理计划则将逻辑计划转化为可执行的操作，考虑了具体的执行细节和执行代价。逻辑计划是查询优化的一个关键阶段，它对查询进行了抽象和优化；物理计划则是执行阶段的具体操作计划，它考虑了实际的执行环境和资源情况。
```
