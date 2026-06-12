# 实验 3.3：共享内存传感器数据通路

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第三章 程序库、进程与线程 |
| 讲次 | 第 3 讲 |
| 课程主题 | 进程间通信（IPC） |
| 实验类型 | 必做实验 |

大纲讲次原文：第3讲 进程间通信（IPC）。
大纲实验原文：采集进程->共享内存->显示进程 架构，信号量同步，实现传感器数据跨进程实时显示
大纲知识点原文：匿名管道；命名管道（FIFO）；消息队列；共享内存；信号量同步；信号处理进阶。

## 实验目标

- 定义固定布局的 `sensor_frame_t` 共享数据结构。
- 使用 `shm_open`、`ftruncate`、`mmap` 创建共享内存。
- 使用 `sem_open`、`sem_wait`、`sem_post` 保护读写。
- 实现 `sensor_writer`、`sensor_viewer` 和 `cleanup` 操作。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 匿名管道 | 可选演示父子进程传递一条采样数据 |
| 2 | 命名管道 FIFO | 用 FIFO 发送 `quit` 或 `sample` 控制命令 |
| 3 | 消息队列 | 可选用消息队列发送采样间隔调整命令 |
| 4 | 共享内存 | 数据帧通过 `/dev/shm` 对象在两个进程间共享 |
| 5 | 信号量同步 | 读写共享结构体前后使用命名信号量 |
| 6 | 信号处理进阶 | 捕获 `SIGTERM`，退出前关闭映射和信号量 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 能编译或交叉编译 POSIX IPC 程序 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 或 mock 输入；可选 OLED 显示 |
| 软件依赖 | `gcc`、`make`、`pthread`、`rt`、`procps` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 采集进程 | DHT22 GPIO 或 mock | 写入 `/sensor_frame` | 采集失败时写入 `valid=0` |
| 显示进程 | 终端或 OLED I2C | 读取 `/sensor_frame` | OLED 实测以第二章地址为准 |
| 同步对象 | `/sensor_frame_sem` | 命名信号量保护共享内存 | 程序异常退出后需要 cleanup |

## 实验任务

### 任务 1：定义共享数据帧

字段包括版本号、毫秒时间戳、温度、湿度、有效标志和错误码。

### 任务 2：实现采集进程

按固定周期读取传感器，把结果写入共享内存，并递增版本号。

### 任务 3：实现显示进程

定时读取共享内存，只在版本号变化时刷新终端或 OLED。

### 任务 4：实现清理命令

提供 `--cleanup` 删除共享内存和信号量对象。

## 实验步骤

```bash
mkdir -p ch03_lab3/{include,src,build,bin}
cd ch03_lab3
gcc -Iinclude src/ipc_common.c src/sensor_writer.c -pthread -lrt -o bin/sensor_writer
gcc -Iinclude src/ipc_common.c src/sensor_viewer.c -pthread -lrt -o bin/sensor_viewer
```

```bash
# 清理旧对象，避免上次实验影响
./bin/sensor_writer --cleanup || true
ls /dev/shm | grep sensor || true
```

```bash
# 终端 1：启动写入端
./bin/sensor_writer --shm /sensor_frame --sem /sensor_frame_sem --interval 1000 --mock
```

```bash
# 终端 2：启动读取端
./bin/sensor_viewer --shm /sensor_frame --sem /sensor_frame_sem --display console
```

```bash
# 检查 IPC 对象
ls -l /dev/shm | grep sensor_frame
ps -ef | grep 'sensor_'
```

```bash
# 停止与清理
pkill -TERM sensor_writer
pkill -TERM sensor_viewer
./bin/sensor_writer --cleanup
ls /dev/shm | grep sensor || echo "ipc objects removed"
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 共享内存创建 | `/dev/shm` 中出现 `sensor_frame` 对象 |  |
| 信号量同步 | 显示端无半行、异常跳变或结构体撕裂现象 |  |
| 数据更新 | viewer 输出的版本号和时间戳持续递增 |  |
| 异常停止 | writer 停止后 viewer 能提示超时或保持最后状态 |  |
| 对象清理 | cleanup 后 `/dev/shm` 中无实验对象残留 |  |
| 板端运行 | LicheePi 4A 上两个进程可同时稳定运行 3 分钟 |  |

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
| `Permission denied` | 旧共享内存对象由 root 创建 | 使用同一用户运行，或 cleanup 后重建 |
| viewer 一直显示无数据 | writer 未启动或名称不一致 | 确认 `--shm` 和 `--sem` 参数完全一致 |
| 数据偶发异常 | 未加锁或锁范围太小 | 写完整个结构体和读完整个结构体时都持有信号量 |
| cleanup 失败 | 仍有进程映射对象 | 先停止 writer/viewer，再执行 cleanup |

## 提交要求

- 实验记录：共享内存名称、信号量名称、两个进程的运行命令。
- 运行截图：writer 输出、viewer 输出、`ls /dev/shm`、cleanup 结果。
- 源码或配置文件：共享结构体头文件、writer、viewer、cleanup 实现。
- 简短说明：说明为什么共享内存需要同步机制。
