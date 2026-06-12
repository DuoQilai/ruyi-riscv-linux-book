# 3.4 多线程编程与同步

## 对应大纲

大纲讲次原文：第4讲 多线程编程与同步。
大纲知识点原文：pthread 线程生命周期；互斥量（mutex）；条件变量；读写锁与自旋锁；生产者-消费者模型；线程池设计与实现。

pthread 线程生命周期、互斥量、条件变量、读写锁与自旋锁、生产者-消费者模型、线程池设计与实现。

## 目标

学生能使用 pthread 将传感器终端拆分为采集线程、处理线程和显示线程，能用 mutex 与条件变量保护共享队列，并能定位死锁、忙等和退出不完整等常见问题。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | pthread 线程生命周期 | `pthread_create`、`join`、`detach`、线程属性、TLS |
| 2 | 互斥量 mutex | 初始化、加锁、解锁、销毁、死锁条件和预防 |
| 3 | 条件变量 | `pthread_cond_wait`、`signal`、`broadcast` 和虚假唤醒 |
| 4 | 读写锁与自旋锁 | 多读单写场景、短临界区与普通 mutex 的取舍 |
| 5 | 生产者-消费者模型 | 有界队列、采集线程到处理线程再到显示线程 |
| 6 | 线程池设计与实现 | 任务队列、worker 循环、提交任务和优雅退出 |

## 讲授要点

- 线程共享进程地址空间，因此访问全局变量和堆对象更方便，也更容易发生数据竞争。
- mutex 保护临界区，条件变量用于“等待某个条件成立”；`pthread_cond_wait` 必须放在 while 循环中检查条件。
- 有界队列比无限增长的链表更适合嵌入式场景，可以限制内存占用并提供背压。
- 采集线程不应直接刷新 OLED 或执行网络发送，避免慢操作阻塞采样；应把数据放入队列交给其他线程。
- 退出流程要设计清楚：设置停止标志、广播条件变量、等待线程退出、销毁锁和释放硬件资源。
- 线程池适合处理多个短任务，例如网络连接或日志写入；传感器固定流水线则可以用少量长期线程。

## 操作或演示

1. 创建采集线程并传入上下文。

```c
typedef struct {
    int gpio;
    int interval_ms;
    volatile int stop;
} app_context_t;

pthread_create(&tid, NULL, sensor_thread, &ctx);
```

2. 定义有界队列。

```c
typedef struct {
    sensor_sample_t items[16];
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} sample_queue_t;
```

3. 编译多线程终端。

```bash
gcc -Iinclude src/*.c app/thread_terminal.c -pthread -o build/thread_terminal
./build/thread_terminal --interval 1000 --display console
```

4. 观察线程和 CPU 占用。

```bash
ps -L -p $(pidof thread_terminal) -o pid,tid,stat,comm
top -H -p $(pidof thread_terminal)
```

5. 触发优雅退出。

```bash
kill -TERM $(pidof thread_terminal)
journalctl -t thread-terminal -n 20
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 线程启动 | `ps -L -p <pid>` | 可看到主线程、采集线程、显示线程 |
| 队列同步 | 程序日志 | 队列长度在 0 到容量之间变化，无越界 |
| 条件变量 | 降低采样频率后观察 CPU | 空闲时 CPU 占用低，无忙等 |
| 数据输出 | `./build/thread_terminal --display console` | 周期性输出温湿度和时间戳 |
| 退出流程 | `kill -TERM <pid>` | 各线程输出退出日志，进程结束 |
| 死锁排查 | 人为注释 unlock 后运行调试版 | 能用日志或 GDB 定位卡住位置 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 程序偶发卡死 | 加锁顺序不一致或异常路径未解锁 | 统一锁顺序，使用单出口清理 |
| CPU 占用很高 | 使用轮询等待队列数据 | 改用条件变量等待 |
| 退出时卡在 join | 子线程未收到停止信号或仍在阻塞 I/O | 设置停止标志，广播条件变量，给阻塞 I/O 设置超时 |
| OLED 输出错乱 | 多线程同时访问同一显示设备 | 只允许显示线程访问 OLED，或用专用 mutex 保护 |

## 本讲成果

- 能创建、等待和结束 pthread 线程。
- 能实现 mutex + 条件变量保护的有界队列。
- 能搭建采集、处理、显示三线程流水线。
- 能解释生产者-消费者模型与嵌入式资源限制之间的关系。
