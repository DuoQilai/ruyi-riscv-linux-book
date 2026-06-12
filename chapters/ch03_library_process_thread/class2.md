# 3.2 Linux 进程模型与控制

## 对应大纲

大纲讲次原文：第2讲 Linux 进程模型与控制。
大纲知识点原文：进程生命周期；exec 函数族；进程退出与回收；僵尸进程与孤儿进程；守护进程编写；进程资源与限制。

进程生命周期、exec 函数族、进程退出与回收、僵尸进程与孤儿进程、守护进程编写、进程资源与限制。

## 目标

学生能解释 Linux 进程从创建、替换、运行到退出回收的完整生命周期，能将传感器采集程序改造成后台守护进程，并能在 RevyOS 上通过 `/proc`、`pstree` 和 journal 日志观察运行状态。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 进程生命周期 | `fork`、`vfork`、`clone`、写时复制、PID/PPID 和进程树 |
| 2 | exec 函数族 | `execl`、`execv`、`execle`、`execve` 的参数、环境变量和错误处理 |
| 3 | 进程退出与回收 | `exit`、`_exit`、`return`、`wait`、`waitpid` 和退出状态宏 |
| 4 | 僵尸进程与孤儿进程 | 产生条件、危害、`SIGCHLD` 处理和服务化规避方式 |
| 5 | 守护进程编写 | `setsid`、`chdir`、`umask`、关闭文件描述符、syslog 输出 |
| 6 | 进程资源与限制 | `getrlimit`、`setrlimit`、`/proc/PID/status`、`maps`、`fd`、`top` |

## 讲授要点

- `fork` 复制当前进程上下文，父子进程从同一行之后继续执行；返回值决定当前代码处于父进程还是子进程。
- `exec` 不创建新进程，而是用新程序映像替换当前进程，因此 `exec` 成功后不会返回。
- 父进程必须回收子进程退出状态，否则子进程会短暂或长期停留为僵尸进程。
- 守护进程不是简单地在命令后加 `&`，还需要脱离终端、处理工作目录、权限掩码、标准文件描述符和日志输出。
- 课程实践中优先让 systemd 管理长期服务；手写 daemon 用于理解进程模型和传统后台程序结构。
- `/proc` 是进程调试入口，学生应能通过 `status` 看状态和内存，通过 `fd` 看打开文件，通过 `maps` 看动态库映射。

## 操作或演示

1. 演示父子进程关系。

```bash
gcc process_tree.c -o process_tree
./process_tree &
pstree -p $!
```

2. 演示 `fork + exec` 启动外部命令。

```c
pid_t pid = fork();
if (pid == 0) {
    execl("/bin/date", "date", "+%F %T", NULL);
    _exit(127);
}
waitpid(pid, &status, 0);
```

3. 演示未回收子进程产生僵尸状态。

```bash
gcc zombie_demo.c -o zombie_demo
./zombie_demo
ps -o pid,ppid,state,cmd -p <child-pid>
```

4. 将采集程序加入 daemon 化选项。

```bash
./sensor_daemon --foreground --interval 2
sudo ./sensor_daemon --daemon --interval 2
journalctl -t sensor-daemon -n 20
```

5. 查看进程资源和打开文件。

```bash
pidof sensor_daemon
cat /proc/<pid>/status
ls -l /proc/<pid>/fd
cat /proc/<pid>/limits
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| fork 行为 | `./process_tree` | 父子进程输出不同 PID，PPID 对应 |
| exec 行为 | `./exec_demo` | 子进程输出 `date` 结果，父进程拿到退出码 |
| wait 回收 | `ps -o state -p <pid>` | 正确回收后无 `Z` 状态残留 |
| daemon 运行 | `pgrep -a sensor_daemon` | 后台存在采集进程 |
| 日志输出 | `journalctl -t sensor-daemon -n 20` | 能看到周期性采集日志 |
| 资源观察 | `cat /proc/<pid>/limits` | 能看到文件数、栈大小等限制 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| daemon 启动后没有输出 | 标准输出已关闭，日志写入 syslog/journal | 使用 `journalctl -t sensor-daemon` 查看 |
| 进程无法退出 | 未处理 `SIGTERM` 或循环条件不受控 | 添加信号处理函数，将退出标志设为 1 |
| 出现僵尸进程 | 父进程未调用 `wait/waitpid` | 在父进程中回收，或处理 `SIGCHLD` |
| 读取传感器失败但服务仍在 | 后台进程缺少 GPIO 权限或硬件未连接 | 检查用户组、设备节点权限和接线 |

## 本讲成果

- 能编写并解释 `fork`、`exec`、`waitpid` 示例。
- 能识别僵尸进程和孤儿进程，并说明处理策略。
- 能将采集程序以前台调试模式和后台 daemon 模式运行。
- 能通过 `/proc` 与 journal 定位后台进程状态。
