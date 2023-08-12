---
title: 查询优化大揭秘 TiDB的查询优化规则有哪些？
tags: TiDB
categories: 源码分析
date: 2023-08-02 22:52:52
abbrlink: 9ecc5fe9
---

根据源码中的`optRuleList`列表来看，TiDB 的逻辑计划优化规则总共有22种，下面是源码：

<!-- more -->

```go
var optRuleList = []logicalOptRule{
	&gcSubstituter{},
	&columnPruner{},
	&resultReorder{},
	&buildKeySolver{},
	&decorrelateSolver{},
	&semiJoinRewriter{},
	&aggregationEliminator{},
	&skewDistinctAggRewriter{},
	&projectionEliminator{},
	&maxMinEliminator{},
	&ppdSolver{},
	&outerJoinEliminator{},
	&partitionProcessor{},
	&collectPredicateColumnsPoint{},
	&aggregationPushDownSolver{},
	&deriveTopNFromWindow{},
	&predicateSimplification{},
	&pushDownTopNOptimizer{},
	&syncWaitStatsLoadPoint{},
	&joinReOrderSolver{},
	&columnPruner{}, // column pruning again at last, note it will mess up the results of buildKeySolver
	&pushDownSequenceSolver{},
}
```

我们现在来一个一个过一遍这些 rule。

1. gcSubstituter。下面是`optimize`方法的注释：

```go
// optimize try to replace the expression to indexed virtual generate column in where, group by, order by, and field clause
// so that we can use the index on expression.
// For example. select a+1 from t order by a+1, with a virtual generate column c as (a+1) and
// an index on c. We need to replace a+1 with c so that we can use the index on c.
// See also https://dev.mysql.com/doc/refman/8.0/en/generated-column-index-optimizations.html
```

简单来说，这个优化规则是将where、group by、order by或者字段子句中的表达式替换成被索引的虚拟生成列，从而能够使用索引来访问。具体例子可以看注释中的解释，或者mysql 中对[生成列索引优化](https://dev.mysql.com/doc/refman/8.0/en/generated-column-index-optimizations.html)的解释。

2. columnPruner。这个方法直接没有注释了，但是也很好理解，其实就是列裁剪，对于一些用不到的列，优化过程中可以直接去除，避免多余的 IO 占用。

3. resultReorder。 结果重排序，对没有排序的结果插入一个排序操作符，用于对结果进行排序。

```go
/*
resultReorder reorder query results.
NOTE. it's not a common rule for all queries, it's specially implemented for a few customers.

Results of some queries are not ordered, for example.

	create table t (a int); insert into t values (1), (2); select a from t;

In the case above, the result can be `1 2` or `2 1`, which is not ordered.
This rule reorders results by modifying or injecting a Sort operator.
 1. iterate the plan from the root, and ignore all input-order operators (Sel/Proj/Limit);
 2. when meeting the first non-input-order operator,
    2.1. if it's a Sort, update it by appending all output columns into its order-by list,
    2.2. otherwise, inject a new Sort upon this operator.
*/
```

4. buildKeySolver。提取索引信息为 schema 设置对应的 key。这个东西应该是之后会用来决定如何使用索引的，以后看到这个内容可以再回来研究一下。

5. decorrelateSolver。用来将 `apply plan` 转换成 `join plan`。这个应该就是关联子查询去关联，说白了就是讲一些能够转换成 join 操作的子查询重写成 join 的形式，好处是可以减少子查询重复执行次数。详细解释可以见https://docs.pingcap.com/zh/tidb/dev/correlated-subquery-optimization

```markdown
"Apply plan" 是一个数据库查询优化和执行的概念，用于在查询执行过程中使用嵌套循环连接或半连接操作。

在关系数据库中，当执行包含连接操作的查询时，优化器可以选择使用不同的连接算法来获取结果。一种常见的连接算法是嵌套循环连接（Nested Loop Join），它基于嵌套循环的方式逐个比较连接条件并获取匹配的行。

"Apply plan" 是一种特殊的嵌套循环连接算法，它在连接操作中使用了子查询或者外部查询的结果。具体来说，"Apply plan" 在每次迭代连接的过程中，将外部查询的结果作为输入，并将其应用于嵌套循环连接中的内部查询。这样可以在每次迭代中有效地使用外部查询的结果来过滤和匹配内部查询的行。

"Apply plan" 在执行连接操作时具有一些优势，例如：

1. 减少连接操作的数据量：通过在每次迭代中使用外部查询的结果进行过滤，可以减少连接操作涉及的数据量，从而提高查询性能和效率。

2. 优化连接操作的执行顺序："Apply plan" 可以改变连接操作的执行顺序，先执行外部查询，然后在每次迭代中应用内部查询，这样可以根据数据的特点和选择性来优化连接操作的执行计划。

3. 支持相关子查询："Apply plan" 在连接操作中使用了子查询的结果，这样可以支持相关子查询（Correlated Subquery）的语义和语法。

需要注意的是，"Apply plan" 并不是适用于所有情况的最佳连接算法。在实际应用中，优化器会根据查询的复杂性、数据分布、索引情况等因素综合考虑，选择最适合的连接算法来执行查询。

--------

"Join plan" 是数据库查询优化和执行中的一个概念，用于指代执行连接操作时所采用的计划或策略。

在关系数据库中，"join" 是指将两个或多个表基于某个关联条件进行连接操作，以获取符合条件的结果集。在执行连接操作时，优化器需要选择适当的连接算法和连接顺序，以获得最佳的查询性能。

"Join plan" 描述了执行连接操作时所采用的具体计划或策略，包括以下方面：

1. 连接算法选择：根据查询条件、表的大小、索引情况等因素，优化器会选择适合的连接算法，例如嵌套循环连接、哈希连接、排序合并连接等。每种连接算法都有其适用的场景和性能特点。

2. 连接顺序决策：当连接操作涉及多个表时，优化器需要确定连接的顺序。连接顺序的选择可以影响查询性能，例如选择连接顺序可以最小化中间结果集的大小，减少连接操作的数据量。

3. 连接类型决策：连接操作可以有不同的连接类型，如内连接、左连接、右连接、全外连接等。"Join plan" 包括选择适当的连接类型以满足查询需求。

4. 索引和优化技术：优化器会考虑现有的索引和其他优化技术，以提高连接操作的性能。它可能选择使用索引来加速连接操作，并利用统计信息和查询历史来评估和选择最佳的连接计划。

总之，“Join plan” 是在查询优化过程中确定连接操作的具体计划或策略。优化器通过考虑连接算法、连接顺序、连接类型以及索引和优化技术等因素，选择最佳的连接计划，以实现高效的查询执行。
```

6. semiJoinRewriter。半连接重写。

```markdown
"semiJoin"（半连接）是数据库查询中的一种操作，用于判断一个表中的数据是否存在于另一个表中。

在半连接操作中，查询优化器会选择一个表作为左表（left table），另一个表作为右表（right table）。然后，它会根据两个表之间的连接条件，筛选出左表中符合条件的行，并检查这些行是否存在于右表中。

半连接的目的是提取左表中满足连接条件的行，而不关心右表的具体数据。它通常用于优化查询性能，减少数据传输和处理的开销。

半连接操作可以有不同的实现方式，其中一种常见的实现方式是使用存在谓词（EXISTS predicate）。该谓词会在连接操作中使用子查询来判断左表的行是否存在于右表中，返回一个布尔值作为结果。

举个例子，假设有两个表 A 和 B，需要判断表 A 中的某些行是否在表 B 中存在。使用半连接操作，可以编写类似下面的查询：

    ```sql
    SELECT *
    FROM A
    WHERE EXISTS (
    SELECT 1
    FROM B
    WHERE A.id = B.id
    );
    ```

上述查询中，子查询会根据连接条件 A.id = B.id 来检查表 A 的行是否存在于表 B 中。如果存在匹配的行，则返回结果。

通过使用半连接操作，可以有效地筛选出左表中存在于右表中的行，从而减少数据量和查询的开销，提高查询性能。
```

7. aggregationEliminator。消除一些无意义的聚合函数，下面是方法的注释，可以参考其中的例子来理解。

```go
// tryToEliminateAggregation will eliminate aggregation grouped by unique key.
// e.g. select min(b) from t group by a. If a is a unique key, then this sql is equal to `select b from t group by a`.
// For count(expr), sum(expr), avg(expr), count(distinct expr, [expr...]) we may need to rewrite the expr. Details are shown below.
// If we can eliminate agg successful, we return a projection. Else we return a nil pointer.
```

8. skewDistinctAggRewriter。将group distinct 聚集函数重写成两级聚合。可以参考下面注释中的例子来理解。这种优化是为了优化在group key数据偏移的情况下缓解数据偏移。这个规则会被应用于满足以下条件的 query：
    1. 至少有一个 group by的语句
    2. 有且仅有一个 distinct aggregate 函数（仅限于 count、avg 和 sum）

```go
// skewDistinctAggRewriter will rewrite group distinct aggregate into 2 level aggregates, e.g.:
//
//	select S_NATIONKEY as s, count(S_SUPPKEY), count(distinct S_NAME) from supplier group by s;
//
// will be rewritten to
//
//	select S_NATIONKEY as s, sum(c), count(S_NAME) from (
//	  select S_NATIONKEY, S_NAME, count(S_SUPPKEY) c from supplier group by S_NATIONKEY, S_NAME
//	) as T group by s;
//
// If the group key is highly skewed and the distinct key has large number of distinct values
// (a.k.a. high cardinality), the query execution will be slow. This rule may help to ease the
// skew issue.
//
// The rewrite rule only applies to query that satisfies:
// - The aggregate has at least 1 group by column (the group key can be columns or expressions)
// - The aggregate has 1 and only 1 distinct aggregate function (limited to count, avg, sum)
//
// This rule is disabled by default. Use tidb_opt_skew_distinct_agg to enable the rule.
```

9. projectionEliminator。消除逻辑计划中多余的投影。

10. maxMinEliminator。消除最大值最小值函数。下面是注释，可以看一下其中的例子：

```go
// maxMinEliminator tries to eliminate max/min aggregate function.
// For SQL like `select max(id) from t;`, we could optimize it to `select max(id) from (select id from t order by id desc limit 1 where id is not null) t;`.
// For SQL like `select min(id) from t;`, we could optimize it to `select max(id) from (select id from t order by id limit 1 where id is not null) t;`.
// For SQL like `select max(id), min(id) from t;`, we could optimize it to the cartesianJoin result of the two queries above if `id` has an index.
```

11. ppdSolver。这个名字有意思，其实是`PredicatePushDown`的缩写。 这个是用来执行谓词下推的规则。

```go
// PredicatePushDown pushes down the predicates in the where/on/having clauses as deeply as possible.
// It will accept a predicate that is an expression slice, and return the expressions that can't be pushed.
// Because it might change the root if the having clause exists, we need to return a plan that represents a new root.

谓词下推（Predicate Pushdown）是数据库查询优化的一种技术，它通过将查询条件下推到数据源（如表、索引）级别进行处理，减少数据的读取和处理量，从而提高查询性能。

在传统的查询执行过程中，查询优化器通常会将查询条件应用于获取结果集之后进行筛选。这意味着数据库系统会读取所有的数据记录，并在获取结果集后再应用查询条件进行过滤，导致数据的读取和处理开销较大。

而谓词下推的思想是，在查询过程中尽早地将查询条件下推到数据源级别，即在读取数据之前应用查询条件进行过滤。这样可以减少读取和处理的数据量，提高查询性能。

通过谓词下推，数据库系统可以在数据源级别利用索引、统计信息和其他优化技术来加速查询过程。它可以有效地削减不必要的数据读取和处理，减少中间结果集的大小，并在查询过程中提前过滤掉不符合查询条件的数据。

谓词下推的具体操作可以包括以下方面：

1. 下推过滤条件：将查询条件应用于索引扫描或表扫描的过程中，尽早地过滤掉不满足条件的数据。

2. 下推投影操作：根据查询需要，将字段选择（Projection）操作下推到数据源级别，减少返回结果的列数。

3. 下推连接条件：对连接操作进行优化，将连接条件下推到连接操作的数据源，减少连接的数据量。

谓词下推在数据库查询优化中起着重要的作用，它可以显著提高查询性能，减少数据读取和处理的开销。通过在数据源级别应用查询条件，可以最大程度地减少不必要的数据操作，加速查询过程并降低系统资源的消耗。
```

12. outerJoinEliminator。消除外连接。主要有以下两个规则：
    1. 外连接消除：例如左外连接，如果父查询只使用左表的列和右表（内部表）的连接键（唯一键），则可以消除左外连接。
    2. 使用不考虑重复的聚合函数进行外连接消除：例如左外连接。如果父查询只使用带有 'distinct' 标签的左表列，则可以消除左外连接。

下面是代码中的注释：
```
// tryToEliminateOuterJoin will eliminate outer join plan base on the following rules
//  1. outer join elimination: For example left outer join, if the parent only use the
//     columns from left table and the join key of right table(the inner table) is a unique
//     key of the right table. the left outer join can be eliminated.
//  2. outer join elimination with duplicate agnostic aggregate functions: For example left outer join.
//     If the parent only use the columns from left table with 'distinct' label. The left outer join can
//     be eliminated.
```

13. partitionProcessor。对分区语句进行重写。主要是做固定分区修剪。下面是注释内容：

```go
// partitionProcessor rewrites the ast for table partition.
// Used by static partition prune mode.
/*
// create table t (id int) partition by range (id)
//   (partition p1 values less than (10),
//    partition p2 values less than (20),
//    partition p3 values less than (30))
//
// select * from t is equal to
// select * from (union all
//      select * from p1 where id < 10
//      select * from p2 where id < 20
//      select * from p3 where id < 30)
*/
// partitionProcessor is here because it's easier to prune partition after predicate push down.
```
这里提到在谓词下推之后做分区修剪会更加简单。

14. collectPredicateColumnsPoint。这个方法没有一句注释。不过这个方法有几个比较重要的子方法，下面是这几个子方法的注释：(TODO)
```
// CollectColumnStatsUsage collects column stats usage from logical plan.
// predicate indicates whether to collect predicate columns and histNeeded indicates whether to collect histogram-needed columns.
// The first return value is predicate columns(nil if predicate is false) and the second return value is histogram-needed columns(nil if histNeeded is false).


// collectSyncIndices will collect the indices which includes following conditions:
// 1. the indices contained the any one of histNeededColumns, eg: histNeededColumns contained A,B columns, and idx_a is
// composed up by A column, then we thought the idx_a should be collected
// 2. The stats condition of idx_a can't meet IsFullLoad, which means its stats was evicted previously
```

15. aggregationPushDownSolver。聚集函数下推，下面是重要方法的注释：

```go
// tryToPushDownAgg tries to push down an aggregate function into a join path. If all aggFuncs are first row, we won't
// process it temporarily. If not, We will add additional group by columns and first row functions. We make a new aggregation operator.
// If the pushed aggregation is grouped by unique key, it's no need to push it down.
```

16. deriveTopNFromWindow。 从窗口函数中推导 TopN或Limit。按照官方文档中的例子，其实就是通过改写带有窗口函数的语句，减少无意义的排序操作。从下面的例子其实很容易理解这个过程：
```
// 改写前
SELECT * FROM (SELECT ROW_NUMBER() OVER (ORDER BY a) AS rownumber FROM t) dt WHERE rownumber <= 3

// 改写后
WITH t_topN AS (SELECT a FROM t1 ORDER BY a LIMIT 3) SELECT * FROM (SELECT ROW_NUMBER() OVER (ORDER BY a) AS rownumber FROM t_topN) dt WHERE rownumber <= 3
```
可以看出，进行改写之后，原语句中对全表的 sort 操作被简化成一个 sort + limit 操作，极大地节省了资源。

17. predicateSimplification。谓词简化，其实就是对一些谓词语句进行简化。

18. pushDownTopNOptimizer。这个方法将下推 topN或者 limit操作。 

上述的几个下推方法其实都是类似的思想，讲一些计算操作或者条件判断尽可能下推到距离数据源近的地方，尽早完成数据的过滤操作，从而减少数据传输和计算的开销。

19. syncWaitStatsLoadPoint。同步等待数据加载。

20. joinReOrderSolver。递归采集 join 组，然后对每个组执行 join 重排序算法。

21. columnPruner。最后再进行一次列裁剪，因为前面的列裁剪可能会被`buildKeySolver`弄乱。

22. pushDownSequenceSolver。递归执行下推序列。

## Reference
[1] https://docs.pingcap.com/zh/tidb/dev/sql-logical-optimization