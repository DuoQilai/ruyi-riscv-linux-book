# tri-thread · 第六章实验三（主实验）

荔枝派 4A：采集 ∥ 控制 ∥ 通信(MQTT)。先完成实验一 race-demo、实验二 snapshot-lock，再做本实验。

```bash
# 板端原生（推荐）
make clean && make
BROKER_HOST=<主机局域网IP> ./tri-thread

# 可选：主机交叉后再 scp
make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
scp tri-thread user@board-ip:~/
```

Broker 用环境变量 `BROKER_HOST`（优先于源码默认值）。可选 `MQTT_CLIENT_ID`；默认 `tri-thread-<主机名>`。

默认 `USE_LOCK 0` 必现 `[RACE]`；验收改为 `USE_LOCK 1`（与实验二成对加锁同一思路）：

```bash
# 先看无锁：默认即 USE_LOCK 0
make clean && make
BROKER_HOST=<主机局域网IP> ./tri-thread

# 再加锁重编（改 USE_LOCK 后必须 clean，否则目标文件不会重编）
make clean && make USE_LOCK=1
BROKER_HOST=<主机局域网IP> ./tri-thread   # 无 [RACE]，race_hits=0
```

## 命令校验与 MQTT 行为

- **命令按 `payloadlen` 精确匹配**：载荷必须与命令等长且逐字节相同（`payload_is`）。不能先转成
  C 字符串再 `strcmp`——`fan on\0junk` 会被截断成合法的 `fan on`。`set high` / `set low` 的数值
  也做有界校验（内嵌 NUL、超长、非数字一律判非法）。非法载荷打印
  `[ERR] unknown command payload (len=N), ignored`，不改风扇、不发 status。
- **订阅结果要检查**：`mosquitto_subscribe` 失败会报错并置标志，通信线程按断线路径重连重订，
  不会只打一行 `subscribed` 就当成连上了。
- **status 发布结果要检查**：`mosquitto_publish` 失败打印 `[ERR] status publish: ...`，
  不再输出假的成功行。
- **Broker 不可达不拖死程序**：连续失败 `MQTT_MAX_RETRY`（5）次后只停 MQTT 这条腿，
  采集/控制线程继续跑本地演示（打印 `MQTT disabled; sense/control keep running`），
  Ctrl+C 再退出。
- **GPIO 初始化失败即失败**：申请线之后初次 `set_value` 写失败会释放请求并让 `fan_init` 失败，
  不带着「风扇状态未知」继续跑。

## 并发行为

- **控制线程**：读状态、判断、写 GPIO 都在同一把锁内完成。MQTT 刚下发的 `fan on` / `fan off`
  不会被基于旧快照的自动控制覆盖。
- **通信线程**：`mosquitto_loop` 的返回值会被检查。断线类错误（`CONN_LOST` / `NO_CONN` /
  `CONN_REFUSED` / `PROTOCOL`）会重连，连续失败 `MQTT_MAX_RETRY`（5）次后该线程退出并置停，
  程序不再空转。
