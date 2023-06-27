# TiDB 存储接口设计

今天开始一个新的系列，分析 TiDB的源码。TiDB是目前开源做的比较好的一个数据库，目前商用做的也挺好的，项目影响力很大。因为是 Go语言实现的，读起来也没有什么太大的门槛。考虑到之后可能会参考 TiDB 做一些自己的小项目，这个系列应该会陆陆续续把整个项目的代码都分析完。由于行文仓促，我对项目也不是特别熟悉，这个系列难免会有错漏之处，之后回顾这个系列的时候，有些地方我可能会自己打脸，重新写一版本。

今天这一篇会简单过一遍 TiDB 如何和TiKV 做交互，主要记录我在读代码过程中看到的一些点。

TiDB 的源码里面有两个模块与存储有关，分别是 store 和 kv。store 部分主要负责，kv 部分主要负责

首先是 store 模块。

在 tidb-server 启动的时候，main 函数中会将 TiKV 的Driver注册到 kvstore 的一个 map 里。`Driver` 接口里面值定义了一个 `Open` 方法，定义在 kv 模块的 kv.go 里，用于返回一个基于当前 store 的 storage 实例。完成注册后，`main` 函数会读取 config 中配置的 keyspaceName，基于此创建一个 storage 实例，用于创建 server。前面注册的 driver 会在此处基于 store path 解析出来，调用 `Open` 方法创建 storage 实例。
```go
// tidb-server/main.go
func main() {
    ...
    registerStores()
    ...
    keyspaceName := keyspace.GetKeyspaceNameBySettings()

    ...
	storage, dom := createStoreAndDomain(keyspaceName)
	svr := createServer(storage, dom)
}
```

这里涉及了另外一个内容更多的接口`Storage`。下面是接口的定义，里面的方法相对较多，我们可以简单过一下几个重要的方法。`Begin`用于开启一个全局事务，`GetSnapshot`方法可以计入传入的 version 读取一个快照，`GetClient`用于获取一个存储 client 实例，`GetMPPClient`用于获取一个 mpp client 实例(MPP是一个面试过程中喜欢聊到的话题，我们可以后面单独聊一下这部分)，`Close`用于关闭当前 store，`UUID`、`CurrentVersion`、`Name`、`Describe`等方法可以获取当前存储实例的相关信息，`GetMemCache`用于获取当前存储的 memory manager，`GetMinSafeTS`基于给定的 txnScope 获取当前存储的最小 SafeTS(这个没太懂是干什么的，后面可以再仔细看看),`GetLockWaits`可以获取所有的 lock wait 信息，`GetCodec`获取当前存储的 `codec`。

```go
// Storage defines the interface for storage.
// Isolation should be at least SI(SNAPSHOT ISOLATION)
type Storage interface {
	// Begin a global transaction
	Begin(opts ...tikv.TxnOption) (Transaction, error)
	// GetSnapshot gets a snapshot that is able to read any data which data is <= ver.
	// if ver is MaxVersion or > current max committed version, we will use current version for this snapshot.
	GetSnapshot(ver Version) Snapshot
	// GetClient gets a client instance.
	GetClient() Client
	// GetMPPClient gets a mpp client instance.
	GetMPPClient() MPPClient
	// Close store
	Close() error
	// UUID return a unique ID which represents a Storage.
	UUID() string
	// CurrentVersion returns current max committed version with the given txnScope (local or global).
	CurrentVersion(txnScope string) (Version, error)
	// GetOracle gets a timestamp oracle client.
	GetOracle() oracle.Oracle
	// SupportDeleteRange gets the storage support delete range or not.
	SupportDeleteRange() (supported bool)
	// Name gets the name of the storage engine
	Name() string
	// Describe returns of brief introduction of the storage
	Describe() string
	// ShowStatus returns the specified status of the storage
	ShowStatus(ctx context.Context, key string) (interface{}, error)
	// GetMemCache return memory manager of the storage.
	GetMemCache() MemManager
	// GetMinSafeTS return the minimal SafeTS of the storage with given txnScope.
	GetMinSafeTS(txnScope string) uint64
	// GetLockWaits return all lock wait information
	GetLockWaits() ([]*deadlockpb.WaitForEntry, error)
	// GetCodec gets the codec of the storage.
	GetCodec() tikv.Codec
}
```

这些方法具体在哪些地方使用，我们可以在后面继续研究。我们可以简单看一下 TiDB 中 `tikv_driver.go`里对这些接口的实现，整个文件只有 400 多行，读起来也比较简单。

首先可以看下当前 storage 是如何创建的。如前文所述，`main`方法中在完成 driver 注册后，会打开 storage，创建 storage 实例。`TiKVDriver`中的`OpenWithOptions`方法，这个方法里会加载一些关键配置，创建一些必要的组件，比如 placement driver相关的实例，用于连接[集群总控中心](https://cn.pingcap.com/blog/placement-driver)（我也不知道为什么叫这个名字，其实 meta server 之类的名字好像更好理解一些），包括 pdCli 和 pdClient，pdCli 用来直接连接 pd server，pdClient 在 pdCli 的基础上对一些元信息做编解码的工作；比如 叫做 SafePointKV 的etcd实例，这个我暂时没明白是用来做什么的，看注释是说用来做 mock test和 runtime的无缝集成的；还有 RPC client，用来做和 tikv server 的 rpc 管理的。最后会基于上述生成的组件，生成一个 KV store 的实例。这里还有一个比较有意思的是会生成一个 coprStore，应该是用来服务于 [Coprocessor](https://cn.pingcap.com/blog/tikv-source-code-reading-14) 场景，这个东西在很多系统中都有实现，后面可以专门讲一下。完成这一系列工作之后，一个 storage 实例就生成了。

接着来看一下`Begin`方法，这个方法是用于开启一个全局事务，主要靠调用 KVStore 的 `Begin` 方法来实现。在 KVStore 的`Begin`方法中，首先加载传入的 option，然后获取启动事务的 timestamp，并基于这个 timestamp 创建一个 TiKV的快照TiKVSnapshot，然后返回 KVTxn 实例，构建TiKVTxn 后返回。Txn相关的细节在之后的文章可以详细介绍一下。

`tikv_driver.go`里面关于 `storage`接口的实现还有一些其他的方法，我们可以在后面调用到的时候来进一步分析。
