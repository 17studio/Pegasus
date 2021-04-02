# 设计

## 技术架构

```
http://asciiflow.com/
                                                          +---------------+
                                                          |               |
                                                          |      web      |
                                                          |               |
                                                          +-------+-------+
                                                                  |
              +----------------+    +----------------+    +-------v-------+
          ---->                |    |                |    |               |
TradeSrv      |    gateway     +---->    datacube    +---->    database   |
          <---+                |    |                |    |               |
              +----+-----^-----+    +------------^---+    +---------------+
                   |     |                       |
                   |     |                       |
              +----v-----+-----+                 |
              |                |                 |
              |     Trade      <---------+       |
              |                |         |       |
              +-------+--------+         |       |
                      |                  |       |
              +-------v--------+         |       |
              |                |         |       |
              |    Strategy    |         |       |
              |                |         |       |
              +-------+--------+         |       |
                      |                  |       |
              +-------v--------+         |       |
              |                +---------+       |
              |      Risk      |                 |
              |                +-----------------+
              +----------------+
```

## 技术要点

* 基于软件实现，提供软实时性低延时响应
* 基于高性能INPROC/TCP的松耦合架构
  - https://github.com/zhiyuan2007/IPC-performance-compare
  - http://stackoverflow.com/questions/2635272/fastest-low-latency-method-for-inter-process-communication-between-java-and-c
  - https://github.com/rigtorp/ipc-bench
  - https://www.zhihu.com/question/25303706
  - http://nanomsg.org/documentation-zeromq.html
* 业务逻辑采用Pony开发，保证安全性
* 基于Web的前端方案，提供跨平台的客户端操作入口

## 模块

### 模块设计

* GateWay In/Out:
  * (PUB)接受行情更新信息并广播
  * (SUB)接收交易请求，含信息查询，并处理
* DataCube:
  * (SUB)接收行情信息并记录，提供缓存服务
  * (PUB)接收历史行情访问请求信息并返回
* Trade:
  * (SUB)接收行情信息并处理
  * (PUB)发送交易请求
* Strategy:
  * 被Trade调用的策略实现
* Risk:
  * (SUB)接收交易请求，并监督是否合规
  * (PUB)发送交易请求，如撤单
* WEB:
  * (SUB)接收行情信息并显示
  * (SUB)请求历史行情/交易数据

### 模块工作原理

#### Gateway

#### Web
采用传统的web应用开发框架，实现web界面
