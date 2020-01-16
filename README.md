# CCAChain - The Enterprise Blockchain Smart Contract Blockchain Platform

欢迎来到 CCAChain 的代码仓库！CCAChain 使企业能够快速构建和部署高性能、高安全性的基于区块链的应用程序。

CCAChain 的一些功能包括：

- 免费限时交易
- 低延迟块确认（0.5秒）
- 低开销的拜占庭式容错最终确定性
- WebAssembly 智能合约平台
- 轻量级客户端验证
- 延迟交易
- 延时安全
- 基于分层角色的权限管理
- 跨链通信

# 系统需求

当前支持以下操作系统：
- Centos 7
- Ubuntu 18.04
- Fedora 25 and higher (Fedora 27 recommended)
- Mint 18
- MacOS Darwin 10.12 and higher (MacOS 10.14.x recommended)

> 仅 CentOS 7 经过开发者充分测试和验证。如果在其他 OS 上编译遇到问题且不能自行解决，请使用 CentOS 7。

编译需要满足如下条件：
- 至少 7GB 的未使用内存
- 至少 20GB 的空闲磁盘空间

# 资源

- 区块浏览器：https://ccachain.info
- 文版通门户：https://www.wenbantong.com
- EOSIO 开发者文档：
  - https://developers.eos.io/eosio-nodeos/v1.7.0/docs
  - https://developers.eos.io/eosio-nodeos/v1.7.0/reference
  - https://developers.eos.io/eosio-cleos/docs

# 编译安装

CCAChain 基于 EOSIO 软件堆栈进行底层深度定制和优化，但与上游 EOSIO 保持同步更新、Bug 修复和特性升级。开发者可通过 EOSIO 的开发者官网，了解如何搭建全节点和智能合约开发。CCAChain 与 EOSIO 差异的地方，对于上层开发者来说基本上保持透明。如果有特别需要注意的地方，这里会做出说明：
- 目前没有提供已编译的二进制文件，需要自行编译。
- 定制和优化部分说明：
  - Kafka 区块数据导出插件
  - MySQL 区块数据导出插件
  - 同构跨链通信机制
  - 智能合约权限控制机制收紧：黑白名单
  - 投票者分红激励
  - 编译流程优化

克隆代码仓库：
```sh
git clone https://github.com/ccachain/ccachain --recursive
# 如果 clone 时没有加 `--recursive`，则需更新子模块
git submodule update --init --recursive
```

> 如果你之前安装过 CCAChain，请执行 `./scripts/eosio_uninstall.sh`。

编译：
```sh
cd ccachain
./scripts/eosio_build.sh
```

查看编译后的二进制可执行程序：
```sh
ls build/bin/
# cleos  keosd  nodeos
```

安装（可选）：
```sh
sudo ./scripts/eosio_install.sh
```

# 连接 CCAChain 主网全节点

CCAChain 的 chain_id 为：`d2b33a6fddcc1d4321bba54a32f6992c32354499b074379ef9a626f3c264e891`。

- CCAChain 主网 API 节点之一：`https://fullnode.ccachain.info`
- CCAChain 主网 P2P 节点之一：`p2pnode.ccachain.info:9876`

查询 CCAChain 基本信息：
```sh
/build/bin/cleos -u https://fullnode.ccachain.info get info
# {
#   "server_version": "bd884a0c",
#   "chain_id": "d2b33a6fddcc1d4321bba54a32f6992c32354499b074379ef9a626f3c264e891",
#   "head_block_num": 73929520,
#   "last_irreversible_block_num": 73929192,
#   "last_irreversible_block_id": "046811e8d1e2830776a9678e9c95858cfaba0d95693354380a9ec3f054e55e7f",
#   "head_block_id": "046813300c3f012d7d8283cb46d63446fd74d40547e13261812a347ccfe30acd",
#   "head_block_time": "2019-12-08T05:06:55.500",
#   "head_block_producer": "ulikeredwine",
#   "virtual_block_cpu_limit": 200000000,
#   "virtual_block_net_limit": 1048576000,
#   "block_cpu_limit": 199900,
#   "block_net_limit": 1048576,
#   "server_version_string": "v1.7.3"
# }
```

保存 genesis 信息到文件：
```sh
cat > genesis.json <<EOF
{
  "initial_timestamp": "2018-10-06T08:08:08.888",
  "initial_key": "EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i",
  "initial_configuration": {
    "max_block_net_usage": 1048576,
    "target_block_net_usage_pct": 1000,
    "max_transaction_net_usage": 524288,
    "base_per_transaction_net_usage": 12,
    "net_usage_leeway": 500,
    "context_free_discount_net_usage_num": 20,
    "context_free_discount_net_usage_den": 100,
    "max_block_cpu_usage": 200000,
    "target_block_cpu_usage_pct": 1000,
    "max_transaction_cpu_usage": 150000,
    "min_transaction_cpu_usage": 100,
    "max_transaction_lifetime": 3600,
    "deferred_trx_expiration_window": 600,
    "max_transaction_delay": 3888000,
    "max_inline_action_size": 4096,
    "max_inline_action_depth": 4,
    "max_authority_depth": 6
  }
}
EOF
```

使用 genesis 文件初始化节点：
```sh
./build/bin/nodeos --genesis-json ./genesis.json --exit-after-initialize-blockchain
```

查看节点数据目录：
```sh
ls /$HOME/.local/share/eosio/nodeos/
# config  data
```

修改配置文件：
```sh
vim /$HOME/.local/share/eosio/nodeos/config/config.ini
# 修改连接 P2P 节点为：p2p-peer-address = p2pnode.ccachain.info:9876
```
参考打开参数：
```sh
http-server-address = 127.0.0.1:8888
enable-stale-production = true
producer-name = eosio
plugin = eosio::producer_plugin
plugin = eosio::chain_api_plugin
plugin = eosio::http_plugin
```
启动节点进行区块数据同步：
```sh
./build/bin/nodeos
```
或
```sh
nodeos -e -p eosio --plugin eosio::chain_api_plugin --plugin eosio::history_api_plugin
```
将看到如下打印输出：
```sh
info  2019-12-08T05:24:02.209 thread-0  chain_plugin.cpp:340          plugin_initialize    ] initializing chain plugin
info  2019-12-08T05:24:02.227 thread-0  block_log.cpp:125             open                 ] Log is nonempty
info  2019-12-08T05:24:02.227 thread-0  block_log.cpp:152             open                 ] Index is nonempty
info  2019-12-08T05:24:02.229 thread-0  main.cpp:99                   main                 ] nodeos version v1.7.3-184-gbd884a0
info  2019-12-08T05:24:02.229 thread-0  main.cpp:100                  main                 ] eosio root is /root/.local/share
info  2019-12-08T05:24:02.229 thread-0  main.cpp:101                  main                 ] nodeos using configuration file /root/.local/share/eosio/nodeos/config/config.ini
info  2019-12-08T05:24:02.230 thread-0  main.cpp:102                  main                 ] nodeos data directory is /root/.local/share/eosio/nodeos/data
info  2019-12-08T05:24:02.230 thread-0  controller.cpp:1865           set_blackwhitelist   ] blackwhitelist: {"sender_bypass_whiteblacklist":[],"actor_whitelist":[],"actor_blacklist":[],"contract_whitelist":[],"contract_blacklist":[],"action_blacklist":[],"key_blacklist":[]}
info  2019-12-08T05:24:02.230 thread-0  chain_plugin.cpp:738          plugin_startup       ] starting chain in read/write mode
info  2019-12-08T05:24:02.230 thread-0  chain_plugin.cpp:742          plugin_startup       ] Blockchain started; head block is #1, genesis timestamp is 2018-10-06T08:08:08.888
info  2019-12-08T05:24:02.231 thread-0  net_plugin.cpp:1827           connect              ] host: p2pnode.ccachain.info port: 9876 
info  2019-12-08T05:24:02.231 thread-0  producer_plugin.cpp:752       plugin_startup       ] producer plugin:  plugin_startup() begin
info  2019-12-08T05:24:02.231 thread-0  producer_plugin.cpp:786       plugin_startup       ] producer plugin:  plugin_startup() end
warn  2019-12-08T05:24:02.343 thread-0  transaction_context.cp:108    deadline_timer       ] Using polled checktime; deadline timer too inaccurate: min:36us max:1067us mean:598us stddev:312us
info  2019-12-08T05:24:03.323 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block c2c3c01ca4d38c21... #1000 @ 2018-10-06T09:00:30.000 signed by eosio [trxs: 0, lib: 999, conf: 0, latency: 36966213323 ms]
info  2019-12-08T05:24:05.055 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 52d759a0beeda065... #2000 @ 2018-10-06T09:08:50.000 signed by eosio [trxs: 0, lib: 1999, conf: 0, latency: 36965715055 ms]
info  2019-12-08T05:24:06.588 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 8f17aa0a6b9d4e6d... #3000 @ 2018-10-06T09:17:10.000 signed by eosio [trxs: 0, lib: 2999, conf: 0, latency: 36965216588 ms]
info  2019-12-08T05:24:08.137 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 5742b5c5243b6350... #4000 @ 2018-10-06T09:25:30.000 signed by eosio [trxs: 0, lib: 3999, conf: 0, latency: 36964718137 ms]
info  2019-12-08T05:24:08.892 thread-0  controller.cpp:1154           start_block          ] promoting proposed schedule (set in block 4429) to pending; current block: 4430 lib: 4429 schedule: {"version":1,"producers":[{"producer_name":"caeeaaaaaaaa","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeebbbbbbbb","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeecccccccc","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeedddddddd","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeeeeeeeeee","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeeffffffff","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeegggggggg","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeehhhhhhhh","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeeiiiiiiii","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeejjjjjjjj","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeekkkkkkkk","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeellllllll","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeemmmmmmmm","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeennnnnnnn","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeeoooooooo","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeepppppppp","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeeqqqqqqqq","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeerrrrrrrr","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeessssssss","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeetttttttt","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"},{"producer_name":"caeeuuuuuuuu","block_signing_key":"EOS8HioqjNHA2Lz6fwZ1wPVHyn4ZpDYJuopfHqh9T3tZDvcrJQM9i"}]} 
info  2019-12-08T05:24:10.082 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 74eed0542a3ddeb9... #5000 @ 2018-10-06T09:40:09.500 signed by caeeffffffff [trxs: 0, lib: 4431, conf: 0, latency: 36963840582 ms]
info  2019-12-08T05:24:11.486 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block ea8713b0dc5bff1e... #6000 @ 2018-10-06T09:48:29.500 signed by caeeeeeeeeee [trxs: 0, lib: 5664, conf: 0, latency: 36963341986 ms]
info  2019-12-08T05:24:13.299 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 97c984ad3931eba8... #7000 @ 2018-10-06T09:57:25.000 signed by caeekkkkkkkk [trxs: 0, lib: 6660, conf: 0, latency: 36962808299 ms]
info  2019-12-08T05:24:14.724 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 0b9c6034394214c3... #8000 @ 2018-10-06T10:05:45.000 signed by caeejjjjjjjj [trxs: 0, lib: 7669, conf: 0, latency: 36962309724 ms]
info  2019-12-08T05:24:16.250 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block 450988fde92080cf... #9000 @ 2018-10-06T10:14:05.000 signed by caeeiiiiiiii [trxs: 0, lib: 8665, conf: 0, latency: 36961811250 ms]
info  2019-12-08T05:24:17.792 thread-0  producer_plugin.cpp:345       on_incoming_block    ] Received block c928c21e4c32e02b... #10000 @ 2018-10-06T10:22:25.000 signed by caeeiiiiiiii [trxs: 0, lib: 9673, conf: 0, latency: 36961312792 ms]
```

查看本节点区块数据同步信息：
```
./build/bin/cleos get info
# 注意 head_block_time 字段
```

如果 head_block_time 已经是当前时间（注意是 UTC 时间），则历史区块数据同步完成，节点已经处于实时可用状态。

## Contributing

[Contributing Guide](./CONTRIBUTING.md)

[Code of Conduct](./CONTRIBUTING.md#conduct)

## License

[MIT](./LICENSE)
