# 3.3 进程间通信（IPC）

## 对应大纲

大纲讲次原文：第3讲 进程间通信（IPC）。
大纲知识点原文：匿名管道；命名管道（FIFO）；消息队列；共享内存；信号量同步；信号处理进阶。

匿名管道、命名管道、消息队列、共享内存、信号量同步、信号处理进阶。

## 目标

学生能根据数据量、进程关系和同步需求选择合适的 IPC 机制，并实现“采集进程写入共享内存，显示进程读取并刷新输出”的实时传感器数据通路。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 匿名管道 | `pipe`、父子进程单向通信、文件描述符关闭规则 |
| 2 | 命名管道 FIFO | `mkfifo`、无亲缘进程通信、阻塞读写语义 |
| 3 | 消息队列 | `mq_open`、`mq_send`、`mq_receive`、消息优先级与清理 |
| 4 | 共享内存 | `shm_open`、`ftruncate`、`mmap`、结构体布局和大数据传输 |
| 5 | 信号量同步 | `sem_open`、`sem_wait`、`sem_post`，保护共享数据一致性 |
| 6 | 信号处理进阶 | `sigaction`、可靠信号、实时信号、`sigqueue`、`signalfd` |

## 讲授要点

- pipe 适合亲缘进程的小数据流，FIFO 适合无亲缘进程但仍以字节流方式通信的场景。
- 消息队列保留消息边界和优先级，适合传递命令或事件；共享内存适合频繁读写结构化数据。
- 共享内存本身只解决“看见同一块内存”，不保证读写同步，必须配合信号量、互斥机制或版本号。
- 跨进程结构体要避免放入普通指针，建议使用固定长度数组、数值字段和时间戳。
- 信号处理函数中只能做极少量异步信号安全操作，复杂逻辑应通过标志位或 `signalfd` 放回主循环。
- IPC 对象需要清理：`mq_unlink`、`shm_unlink`、`sem_unlink`，否则重复实验会受到旧对象影响。

## 操作或演示

1. 使用 pipe 连接父子进程。

```bash
gcc pipe_demo.c -o pipe_demo
./pipe_demo
```

2. 使用 FIFO 让两个终端通信。

```bash
mkfifo /tmp/sensor_cmd.fifo
cat /tmp/sensor_cmd.fifo
echo "sample" > /tmp/sensor_cmd.fifo
```

3. 使用消息队列发送控制命令。

```bash
gcc mq_sender.c -lrt -o mq_sender
gcc mq_receiver.c -lrt -o mq_receiver
./mq_receiver &
./mq_sender '{"cmd":"set_interval","interval":2}'
```

4. 定义共享内存数据结构。

```c
typedef struct {
    uint32_t version;
    uint64_t timestamp_ms;
    float temperature;
    float humidity;
    uint32_t valid;
} sensor_frame_t;
```

5. 运行采集进程和显示进程。

```bash
gcc sensor_writer.c -pthread -lrt -o sensor_writer
gcc sensor_viewer.c -pthread -lrt -o sensor_viewer
./sensor_writer --shm /sensor_frame --sem /sensor_frame_sem &
./sensor_viewer --shm /sensor_frame --sem /sensor_frame_sem
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| pipe 通信 | `./pipe_demo` | 父进程收到子进程写入的数据 |
| FIFO 通信 | `echo sample > /tmp/sensor_cmd.fifo` | 另一个终端立即读到 `sample` |
| 消息队列 | `ls /dev/mqueue` | 可看到实验队列，接收端输出消息内容 |
| 共享内存 | `ls /dev/shm` | 可看到共享内存对象 |
| 同步效果 | 同时运行 writer/viewer | viewer 输出温湿度连续更新，无半写入数据 |
| 清理对象 | `./sensor_writer --cleanup` | `/dev/shm` 和 `/dev/mqueue` 中对象被移除 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| FIFO 读写卡住 | FIFO 默认阻塞，另一端未打开 | 同时启动读端和写端，或使用非阻塞模式 |
| `mq_open` 失败 | 队列名称缺少 `/` 或系统限制过低 | 使用 `/sensor_cmd` 形式，检查 `/proc/sys/fs/mqueue` |
| viewer 读到异常值 | 共享内存未初始化或缺少同步 | 初始化结构体，读写前后使用信号量 |
| 重复运行提示对象已存在 | 上次实验未 unlink | 增加 `--cleanup` 或手动删除 `/dev/shm` 对象 |

## 本讲成果

- 能说明 pipe、FIFO、消息队列和共享内存的差异。
- 能实现一个共享内存数据帧，并用信号量保护读写。
- 能用信号安全的方式处理退出和清理。
- 为第四章 TCP、HTTP、MQTT 服务读取本地数据提供跨进程数据源。
