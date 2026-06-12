# 实验 1.4：自动化部署脚本与 systemd 服务

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 1 章 RISC-V C语言开发与调试 |
| 讲次 | 第 4 讲 |
| 课程主题 | Shell 编程与自动化部署 |
| 实验类型 | 必做实验 |

大纲讲次原文：第4讲 Shell 编程与自动化部署。
大纲实验原文：编写自动化部署脚本，实现编译->SCP 推送->远程执行一键流程，配置 systemd 服务
大纲知识点原文：Shell 脚本基础；条件判断与循环；函数与参数处理；文本处理工具链；自动化部署脚本；systemd 服务管理。

## 实验目标

- 编写带参数解析和错误处理的部署脚本。
- 使用 `scp` 或 `rsync` 把构建产物同步到 LicheePi 4A。
- 在板端远程执行程序并收集日志。
- 编写 systemd unit，完成启动、停止、状态查看和开机自启验证。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | Shell 脚本基础 | 使用 shebang、变量、退出码、`set -euo pipefail` |
| 2 | 条件判断与循环 | 检查参数、路径、工具是否存在 |
| 3 | 函数与参数处理 | 用函数拆分 build/deploy/run/service，用 `getopts` 解析参数 |
| 4 | 文本处理工具链 | 用 `grep`/`awk` 提取日志和服务状态 |
| 5 | 自动化部署脚本 | 一键完成编译、传输和远程执行 |
| 6 | systemd 服务管理 | 安装 unit，执行 `enable/start/status/journalctl` |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux x86_64，已安装交叉编译器、`make`、`ssh`、`scp`、可选 `rsync` |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 主机可 SSH 登录开发板 |
| 软件依赖 | 板端 `systemd`、`journalctl`，程序具备长驻运行模式 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机工程 | 网络 | 构建并上传程序 | SSH 免密可减少部署中断 |
| 板端部署目录 | `/opt/rv-course` | 存放二进制和配置 | 创建目录可能需要 sudo |
| systemd unit | `/etc/systemd/system/` | 管理课程程序 | 修改后必须 `daemon-reload` |

## 实验任务

### 任务 1：编写部署脚本

脚本支持 `-t <board-ip>`、`-u <user>`、`-m <mode>`，其中 mode 至少包含 `run` 和 `service`。

### 任务 2：远程部署和运行

脚本执行 `make all` 后同步到板端，并通过 SSH 运行程序。

### 任务 3：配置 systemd 服务

编写 unit 文件，安装后能启动、停止、查看日志，并按需启用开机自启。

## 实验步骤

1. 准备长驻示例程序，程序每 5 秒输出一次时间或计数，便于查看日志。

```bash
make clean all
```

2. 编写并测试部署脚本。

```bash
chmod +x scripts/deploy.sh
./scripts/deploy.sh -h
./scripts/deploy.sh -t <board-ip> -u <user> -m run
```

3. 板端创建部署目录。

```bash
ssh <user>@<board-ip> 'sudo mkdir -p /opt/rv-course/bin && sudo chown -R $USER:$USER /opt/rv-course'
```

4. 同步文件并远程运行。

```bash
rsync -av build/ <user>@<board-ip>:/opt/rv-course/bin/
ssh <user>@<board-ip> '/opt/rv-course/bin/hello-loop --count 3'
```

5. 安装 systemd unit。

```bash
scp packaging/rv-course.service <user>@<board-ip>:/tmp/rv-course.service
ssh <user>@<board-ip> 'sudo install -m 0644 /tmp/rv-course.service /etc/systemd/system/rv-course.service'
ssh <user>@<board-ip> 'sudo systemctl daemon-reload && sudo systemctl enable --now rv-course.service'
```

6. 查看服务状态和日志。

```bash
ssh <user>@<board-ip> 'systemctl status rv-course.service --no-pager'
ssh <user>@<board-ip> 'journalctl -u rv-course.service -n 30 --no-pager'
ssh <user>@<board-ip> 'systemctl is-enabled rv-course.service'
```

## 运行验证

脚本能一键部署；`systemctl status` 能显示服务状态；重启后服务可按要求启动。

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 帮助信息 | `deploy.sh -h` 显示参数说明 |  |
| 一键运行 | `-m run` 完成编译、同步、远程运行 |  |
| 服务安装 | `/etc/systemd/system/rv-course.service` 存在 |  |
| 服务状态 | `systemctl status` 显示 `active` 或明确日志 |  |
| 日志输出 | `journalctl` 能看到程序周期输出 |  |
| 开机自启 | `systemctl is-enabled` 输出 `enabled` |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `deploy.sh -m run`、`systemctl status`、`journalctl` |
| 关键输出 | 脚本最后 20 行、服务状态、日志片段 |
| 截图或照片 | 一键部署成功、服务状态界面 |
| 异常处理 | 记录权限、路径、服务启动失败和处理方式 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 脚本参数为空仍继续执行 | 未做参数校验 | 在脚本入口检查目标 IP 和用户 |
| 远程目录无权限 | `/opt` 需要 root | 使用 `sudo mkdir` 后 `chown` 给实验用户 |
| 服务启动失败 | `ExecStart` 路径错误或程序缺依赖 | 查看 `journalctl -xeu rv-course.service` |
| 重启后服务没启动 | 未 enable 或依赖网络未就绪 | 执行 `enable`，必要时加 `After=network-online.target` |

## 提交要求

- 实验记录：脚本参数说明、部署目录、服务名称、验证命令。
- 运行截图：一键部署输出、systemd 状态、journalctl 日志。
- 源码或配置文件：`scripts/deploy.sh`、`packaging/rv-course.service`、示例程序源码。
- 简短说明：说明脚本失败时如何定位到具体步骤。

