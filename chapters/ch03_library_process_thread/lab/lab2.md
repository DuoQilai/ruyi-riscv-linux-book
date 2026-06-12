# 实验 3.2：守护进程化传感器采集程序

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第三章 程序库、进程与线程 |
| 讲次 | 第 2 讲 |
| 课程主题 | Linux 进程模型与控制 |
| 实验类型 | 必做实验 |

大纲讲次原文：第2讲 Linux 进程模型与控制。
大纲实验原文：编写守护进程化的传感器采集程序（fork+setsid），后台运行并输出 syslog 日志
大纲知识点原文：进程生命周期；exec 函数族；进程退出与回收；僵尸进程与孤儿进程；守护进程编写；进程资源与限制。

## 实验目标

- 实现 `--foreground` 和 `--daemon` 两种运行模式。
- 使用 syslog 或 systemd journal 记录采集状态。
- 使用 `SIGTERM/SIGINT` 触发优雅退出。
- 使用 `/proc` 和 `pstree` 分析进程状态、打开文件和资源限制。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 进程生命周期 | 观察 daemon 启动前后的 PID、PPID 和进程树 |
| 2 | exec 函数族 | 可选：父进程启动外部 `logger` 或测试命令 |
| 3 | 进程退出与回收 | 用 `waitpid` 回收演示子进程并输出退出状态 |
| 4 | 僵尸进程与孤儿进程 | 运行演示程序观察 `Z` 状态，再修复回收逻辑 |
| 5 | 守护进程编写 | 使用 `fork`、`setsid`、`chdir`、`umask` 和关闭 fd |
| 6 | 进程资源与限制 | 查看 `/proc/PID/status`、`fd`、`limits` |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 能编译并上传程序到 LicheePi 4A |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 或模拟采集输入；无硬件时允许使用 `--mock` |
| 软件依赖 | `gcc`、`make`、`procps`、`systemd`、`journalctl`、`logger` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| DHT22 | 课程指定 GPIO | 与实验 3.1 相同 | 先验证库函数可读取 |
| 日志系统 | syslog/journal | 程序使用 `openlog("sensor-daemon", ...)` | 运行用户需有读 journal 权限或使用 sudo |
| 部署目录 | `/opt/rv-course/ch03_lab2` | 存放程序、库和配置 | 课堂可用用户目录替代 |

## 实验任务

### 任务 1：实现前台采集

程序支持 `--foreground --interval 2 --mock`，每 2 秒输出一次采集结果。

### 任务 2：实现 daemon 模式

程序支持 `--daemon`，后台运行后将日志写入 syslog/journal。

### 任务 3：实现信号退出

收到 `SIGTERM` 或 `SIGINT` 时设置退出标志，完成最后一条日志并释放传感器库。

### 任务 4：观察进程状态

使用 `pstree`、`ps`、`/proc/PID` 和 `journalctl` 完成验收记录。

## 实验步骤

```bash
mkdir -p ch03_lab2/{src,include,build,bin}
cd ch03_lab2
gcc -Iinclude src/sensor_daemon.c src/sensor_backend.c -o bin/sensor_daemon
```

```bash
# 前台模式
./bin/sensor_daemon --foreground --interval 2 --mock
```

```bash
# 后台模式
sudo ./bin/sensor_daemon --daemon --interval 2 --mock
pgrep -a sensor_daemon
pstree -p $(pgrep -n sensor_daemon)
```

```bash
# 查看日志
journalctl -t sensor-daemon -n 20 --no-pager
logger -t sensor-daemon-check "manual log check"
journalctl -t sensor-daemon-check -n 5 --no-pager
```

```bash
# 观察 /proc
PID=$(pgrep -n sensor_daemon)
cat /proc/$PID/status
ls -l /proc/$PID/fd
cat /proc/$PID/limits
```

```bash
# 退出验证
sudo kill -TERM $PID
sleep 1
pgrep -a sensor_daemon || echo "sensor_daemon stopped"
journalctl -t sensor-daemon -n 20 --no-pager
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 前台运行 | 终端周期性输出温湿度或 mock 数据 |  |
| 后台运行 | `pgrep -a sensor_daemon` 能看到进程 |  |
| 进程脱离终端 | `PPid` 和 `TTY` 符合 daemon 或 systemd 托管特征 |  |
| 日志输出 | `journalctl -t sensor-daemon` 有采集、错误和退出日志 |  |
| 资源观察 | `/proc/PID/fd` 中无多余终端 fd 泄漏 |  |
| 优雅退出 | `SIGTERM` 后进程消失并记录退出日志 |  |

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
| `journalctl` 看不到日志 | 标识 tag 不一致或日志服务未刷新 | 检查 `openlog` tag，使用 `journalctl -f` 实时观察 |
| daemon 后立即退出 | 工作目录、库路径或 GPIO 权限错误 | 在前台模式复现错误，再修正路径和权限 |
| kill 后进程仍在 | 信号处理只打印日志未改变循环条件 | 使用全局 `volatile sig_atomic_t stop` 控制循环 |
| 多次启动出现多个实例 | 未做 pidfile 或 systemd 限制 | 实验记录现象，拓展实现 pidfile 或服务单实例 |

## 提交要求

- 实验记录：前台运行、后台运行、`/proc` 观察、退出验证。
- 运行截图：`pgrep`、`pstree`、`journalctl` 关键输出。
- 源码或配置文件：daemon 主程序、信号处理代码、可选 systemd service。
- 简短说明：解释为什么后台程序需要日志和优雅退出。
