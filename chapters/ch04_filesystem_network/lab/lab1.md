# 实验 4.1：mmap 数据文件与 inotify 处理

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第四章 文件系统与网络编程 |
| 讲次 | 第 1 讲 |
| 课程主题 | 文件 I/O 基础与高级操作 |
| 实验类型 | 必做实验 |

大纲讲次原文：第1讲 文件 I/O 基础与高级操作。
大纲实验原文：传感器数据通过 mmap 写入文件，inotify 监控数据文件变化并触发处理
大纲知识点原文：POSIX 文件 I/O；标准 C 库文件操作；文件属性与元数据；内存映射 I/O（mmap）；文件锁与并发保护；inotify 文件监控。

## 实验目标

- 使用 POSIX I/O 创建固定大小的数据文件。
- 使用 `mmap` 将传感器数据帧写入文件。
- 使用文件锁保护并发读写。
- 使用 `inotify` 监控数据文件变化并触发处理程序输出。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | POSIX 文件 I/O | 使用 `open`、`ftruncate`、`write`、`close` 创建和初始化数据文件 |
| 2 | 标准 C 库文件操作 | 使用 `fprintf` 记录 CSV 或文本日志 |
| 3 | 文件属性与元数据 | 使用 `stat` 检查文件大小、权限和修改时间 |
| 4 | 内存映射 I/O | 使用 `mmap` 写入固定结构体数据帧 |
| 5 | 文件锁与并发保护 | 使用 `fcntl` 写锁保护数据更新 |
| 6 | inotify 文件监控 | 使用 `inotify_add_watch` 监听 `IN_MODIFY` 事件 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 能编译并上传程序到 LicheePi 4A |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 或 mock 输入；无传感器时使用程序内模拟数据 |
| 软件依赖 | `gcc`、`make`、`procps`、`coreutils` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 数据文件 | `/home/debian/ch04_lab1/data/sensor.dat` | 保存固定大小 `sensor_frame_t` | 文件大小应不小于结构体大小 |
| 采集程序 | DHT22 GPIO 或 mock | 通过 mmap 写入数据文件 | 写入前加文件锁 |
| 监控程序 | inotify | 监控数据文件所在目录或文件 | 编辑器替换写入时建议监控目录 |
| 文本记录 | `data/sensor.csv` | 保存可读采样记录 | 长时间运行需控制文件大小 |

## 实验任务

### 任务 1：定义数据帧

定义包含版本号、时间戳、温度、湿度、有效标志和错误码的 `sensor_frame_t`。

### 任务 2：实现 mmap 写入端

程序启动后创建或打开数据文件，使用 `ftruncate` 设置大小，用 `mmap` 映射后周期性更新数据帧。

### 任务 3：实现 inotify 监控端

监控数据文件变化，收到修改事件后读取并打印最新数据帧。

### 任务 4：补充并发保护

写入端更新数据前加写锁，读取端读取前加读锁，避免结构体读到一半更新。

## 实验步骤

```bash
mkdir -p ch04_lab1/{include,src,build,bin,data}
cd ch04_lab1
gcc -Iinclude src/mmap_writer.c -o bin/mmap_writer
gcc -Iinclude src/inotify_reader.c -o bin/inotify_reader
```

```bash
# 初始化并持续写入，--mock 表示使用模拟数据
./bin/mmap_writer --file data/sensor.dat --csv data/sensor.csv --interval 1000 --mock
```

```bash
# 另一个终端监控文件变化
./bin/inotify_reader --file data/sensor.dat
```

```bash
# 查看文件属性和文本记录
stat data/sensor.dat
tail -n 5 data/sensor.csv
hexdump -C data/sensor.dat | head
```

```bash
# 并发验证：启动两个读取端
./bin/inotify_reader --file data/sensor.dat &
./bin/inotify_reader --file data/sensor.dat &
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 数据文件创建 | `data/sensor.dat` 存在，大小不为 0 |  |
| mmap 写入 | 版本号和时间戳持续更新 |  |
| inotify 触发 | 写入端更新后读取端输出 `IN_MODIFY` 和最新数据 |  |
| CSV 记录 | `sensor.csv` 追加可读采样行 |  |
| 并发保护 | 多读取端运行时无结构体撕裂或异常跳变 |  |
| 错误处理 | 传入不可写目录时输出明确错误 |  |

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
| `mmap` 返回失败 | 文件未设置大小或权限不足 | 先 `ftruncate`，确认目录可写 |
| 监控端无事件 | 监控路径错误或写入端未更新文件 | 检查路径，使用 `stat` 观察修改时间 |
| 读取到异常数值 | 未加锁或结构体未初始化 | 初始化整个数据帧，读写统一使用锁 |
| CSV 文件快速变大 | 采样频率过高且未轮转 | 降低频率或在后续实验加入日志轮转 |

## 提交要求

- 实验记录：数据文件路径、结构体字段、写入和监控命令。
- 运行截图：`stat`、写入端输出、监控端输出、CSV 内容。
- 源码或配置文件：`mmap_writer.c`、`inotify_reader.c`、公共头文件。
- 简短说明：说明 `mmap`、文件锁和 `inotify` 的分工。
