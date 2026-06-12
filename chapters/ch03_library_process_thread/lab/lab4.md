# 实验 3.4：多线程传感器终端

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第三章 程序库、进程与线程 |
| 讲次 | 第 4 讲 |
| 课程主题 | 多线程编程与同步 |
| 实验类型 | 必做实验 |

大纲讲次原文：第4讲 多线程编程与同步。
大纲实验原文：多线程传感器终端：采集线程->共享队列->OLED 显示线程，mutex+条件变量同步
大纲知识点原文：pthread 线程生命周期；互斥量（mutex）；条件变量；读写锁与自旋锁；生产者-消费者模型；线程池设计与实现。

## 实验目标

- 创建采集、处理、显示三个线程。
- 实现固定容量 `sample_queue_t`，避免无限占用内存。
- 使用 `pthread_mutex_t` 和 `pthread_cond_t` 实现阻塞等待。
- 支持 `SIGTERM/SIGINT` 优雅退出并释放资源。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | pthread 线程生命周期 | 创建三个线程并在退出时 `pthread_join` |
| 2 | 互斥量 mutex | 保护队列 head、tail、count |
| 3 | 条件变量 | 队列空时等待 `not_empty`，队列满时等待 `not_full` |
| 4 | 读写锁与自旋锁 | 可选：用读写锁保护运行配置读取 |
| 5 | 生产者-消费者模型 | 采集线程生产样本，处理/显示线程消费样本 |
| 6 | 线程池设计与实现 | 可选：把日志写入或网络发送任务放入线程池 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 能编译 pthread 程序并部署到板端 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 或 mock 输入；可选 OLED 显示 |
| 软件依赖 | `gcc`、`make`、`pthread`、`procps`、`top` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 采集线程 | DHT22 GPIO 或 mock | 周期性产生 `sensor_sample_t` | 硬件读取失败时仍要写错误状态 |
| 处理线程 | 内存队列 | 计算平均值、阈值或格式化 JSON | 不直接访问硬件 |
| 显示线程 | 终端或 OLED | 输出最新结果 | 只有一个线程访问 OLED |

## 实验任务

### 任务 1：实现线程安全队列

实现 `queue_init`、`queue_push`、`queue_pop`、`queue_stop`、`queue_destroy`。

### 任务 2：实现采集线程

按 `--interval` 参数读取传感器或 mock 数据，写入队列。

### 任务 3：实现处理和显示线程

处理线程计算移动平均或阈值状态；显示线程输出当前值和队列状态。

### 任务 4：实现退出流程

捕获退出信号，设置停止标志，广播条件变量并等待所有线程结束。

## 实验步骤

```bash
mkdir -p ch03_lab4/{include,src,build,bin}
cd ch03_lab4
gcc -Iinclude src/sample_queue.c src/sensor_backend.c src/thread_terminal.c -pthread -o bin/thread_terminal
```

```bash
# 正常运行
./bin/thread_terminal --interval 1000 --queue-size 16 --display console --mock
```

```bash
# 压力观察：提高采样频率，检查队列是否保持边界
./bin/thread_terminal --interval 100 --queue-size 4 --display console --mock
```

```bash
# 查看线程
PID=$(pgrep -n thread_terminal)
ps -L -p $PID -o pid,tid,stat,comm
top -H -p $PID
```

```bash
# 退出验证
kill -TERM $PID
sleep 1
pgrep -a thread_terminal || echo "thread_terminal stopped"
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 线程创建 | `ps -L` 能看到多个线程 |  |
| 队列边界 | 日志中的 `queue_count` 不小于 0 且不大于容量 |  |
| 条件变量 | 空闲时 CPU 占用低，无持续忙等 |  |
| 数据输出 | 终端或 OLED 周期性刷新传感器数据 |  |
| 压力运行 | 高频采样下程序不崩溃，可记录丢弃或等待策略 |  |
| 优雅退出 | `SIGTERM` 后所有线程退出，进程消失 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 |  |
| 关键输出 |  |
| 截图或照片 |  |
| 异常处理 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 程序卡住无输出 | 条件变量等待条件写错或未 signal | 检查 `while` 条件和 push/pop 后的 signal |
| 队列计数越界 | head/tail/count 未在同一把锁内更新 | 把队列状态更新放入同一临界区 |
| 退出时不结束 | 等待线程未被唤醒 | 在 `queue_stop` 中 broadcast 所有条件变量 |
| OLED 闪烁或乱码 | 多线程并发写显示设备 | 只允许显示线程操作 OLED |

## 提交要求

- 实验记录：正常运行、压力运行、线程观察、退出验证。
- 运行截图：`ps -L`、`top -H`、程序关键日志。
- 源码或配置文件：队列实现、线程入口函数、退出处理代码。
- 简短说明：说明 mutex 与条件变量在队列中的分工。
