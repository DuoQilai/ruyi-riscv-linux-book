# 4.1 文件 I/O 基础与高级操作

## 对应大纲

大纲讲次原文：第1讲 文件 I/O 基础与高级操作。
大纲知识点原文：POSIX 文件 I/O；标准 C 库文件操作；文件属性与元数据；内存映射 I/O（mmap）；文件锁与并发保护；inotify 文件监控。

POSIX 文件 I/O、标准 C 库文件操作、文件属性与元数据、内存映射 I/O、文件锁与并发保护、inotify 文件监控。

## 目标

学生能在 RevyOS 上使用 POSIX I/O 和标准 C 文件接口读写传感器数据，能用 `mmap` 共享数据文件，并使用 `inotify` 监控文件变化触发处理逻辑。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | POSIX 文件 I/O | `open`、`creat`、`read`、`write`、`lseek`、`close` 和文件描述符 |
| 2 | 标准 C 库文件操作 | `fopen`、`fread`、`fwrite`、`fprintf`、缓冲模式和 `setvbuf` |
| 3 | 文件属性与元数据 | `stat`、`lstat`、`fstat`、权限、大小、时间戳和 `access` |
| 4 | 内存映射 I/O | `mmap`、`MAP_SHARED`、`MAP_PRIVATE`、大文件与共享数据 |
| 5 | 文件锁与并发保护 | `fcntl` 劝告锁、`flock`、多进程文件写入保护 |
| 6 | inotify 文件监控 | `inotify_init`、`add_watch`、事件读取和配置热加载 |

## 讲授要点

- 文件描述符是进程访问文件、设备、管道和 socket 的统一句柄，错误处理必须检查返回值和 `errno`。
- 标准 C 文件接口有缓冲，适合文本和格式化输出；POSIX I/O 更贴近系统调用，适合设备、锁和精细控制。
- `mmap` 让文件内容映射到内存，适合固定结构体或大文件随机访问，但需要处理文件大小、同步和并发。
- `MAP_SHARED` 的修改可回写文件，`MAP_PRIVATE` 是写时复制，不会修改原文件。
- 文件锁通常是劝告锁，需要所有参与进程都遵守；不能假设锁会自动阻止不配合的程序写文件。
- `inotify` 只告诉文件发生变化，不替你解析文件；读取配置时要处理半写入和格式错误。

## 操作或演示

1. 使用 POSIX I/O 写入一条 CSV。

```bash
gcc posix_log.c -o posix_log
./posix_log data/sensor.csv
tail -n 3 data/sensor.csv
```

2. 使用 stdio 写入格式化日志。

```bash
gcc stdio_log.c -o stdio_log
./stdio_log data/sensor_text.log
```

3. 创建固定大小 mmap 数据文件。

```bash
truncate -s 4096 data/sensor.dat
gcc mmap_writer.c -o mmap_writer
gcc mmap_reader.c -o mmap_reader
./mmap_writer data/sensor.dat &
./mmap_reader data/sensor.dat
```

4. 加入文件锁。

```bash
gcc lock_writer.c -o lock_writer
./lock_writer data/sensor.dat &
./lock_writer data/sensor.dat &
```

5. 监控文件变化。

```bash
gcc inotify_watch.c -o inotify_watch
./inotify_watch data/sensor.dat
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| POSIX 写入 | `tail data/sensor.csv` | 每行包含时间戳、温度、湿度 |
| 元数据 | `stat data/sensor.csv` | 文件大小和修改时间随写入变化 |
| mmap 更新 | 同时运行 writer/reader | reader 能看到 writer 更新的数据帧 |
| 文件锁 | 并发运行两个 writer | 日志中能看到等待或互斥写入 |
| inotify | 修改数据文件 | 监控程序输出 `IN_MODIFY` 或相关事件 |
| 错误处理 | 传入不存在目录 | 程序输出 `errno` 对应诊断 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| mmap 失败 `EINVAL` | 文件长度为 0 或 offset 不按页对齐 | 先 `ftruncate` 到结构体或页大小 |
| 写入后文件为空 | stdio 缓冲未刷新 | 调用 `fflush/fclose` 或设置行缓冲 |
| 文件锁无效 | 其他程序未遵守劝告锁 | 所有写入程序统一使用同一种锁 |
| inotify 无事件 | 监控的是旧文件 inode，编辑器采用替换写入 | 监控目录并处理 `IN_MOVED_TO` |

## 本讲成果

- 能使用 POSIX I/O 和 stdio 写入传感器数据。
- 能用 `stat` 解释文件权限、大小和时间戳。
- 能用 `mmap` 创建固定数据文件并跨进程读取。
- 能用文件锁和 `inotify` 构建可靠的数据更新通路。
