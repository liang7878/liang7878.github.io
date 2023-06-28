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

```
// PointGetPlan is a fast plan for simple point get.
// When we detect that the statement has a unique equal access condition, this plan is used.
// This plan is much faster to build and to execute because it avoid the optimization and coprocessor cost.
```
简单来说，`PointGetPlan`就是用于执行存在唯一的 equal 条件的查询语句的查询计划。

紧接着，这里会对当前计划进行判断，确认是否可以讲计划存入 cache，这里有一系列不同类型的物理计划不允许 cache。我们后面可以单独研究一下哪些计划可以放入 cache，哪些不行。判断可以放入 cache 的 plan，这里会生成对应的 key 和 value，然后放入 cache，并将对应的 plan 返回。

接下来到了比较具体的优化部分，首先从 node 中提取出 `tableHints`,并对语句中的所有 hint 进行处理。完成处理后，如果这里是一个 point plan，函数会直接尝试 `PointGetPlan`来执行当前查询。接着获取 `TxnManager`，尝试`AdviseWarmup，这部分会在对 Txn 的分析里涉及到。