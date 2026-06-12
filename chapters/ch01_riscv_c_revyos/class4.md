# 1.4 Shell 编程与自动化部署

## 对应大纲

大纲讲次原文：第4讲 Shell 编程与自动化部署。
大纲知识点原文：Shell 脚本基础；条件判断与循环；函数与参数处理；文本处理工具链；自动化部署脚本；systemd 服务管理。

Shell 脚本基础、条件判断与循环、函数与参数处理、文本处理工具链、自动化部署脚本、systemd 服务管理。

## 目标

学生能写出一键构建、传输、运行、查看状态的部署脚本，并将程序注册为 LicheePi 4A 上的 systemd 服务。

## 知识点

| # | 知识点 | 本讲说明 |
| --- | --- | --- |
| 1 | Shell 脚本基础 | Shebang、变量、引号、退出码、`set -e/-u/-x` |
| 2 | 条件判断与循环 | `if`、`case`、`for`、`while`、`test` 条件表达式 |
| 3 | 函数与参数处理 | 函数、`$1`、`$@`、`$#`、`getopts` |
| 4 | 文本处理工具链 | `grep`、`sed`、`awk`、管道、重定向和日志提取 |
| 5 | 自动化部署脚本 | 编译、SCP/rsync、远程执行、失败处理和部署目录约定 |
| 6 | systemd 服务管理 | unit 文件、`enable/start/status`、日志查看和开机自启 |

## 讲授要点

- 脚本的第一目标是可重复和可诊断：每一步失败要能看到原因，而不是静默跳过。
- `scp` 适合简单传输，`rsync` 适合频繁同步构建产物和资源文件。
- systemd 服务要明确 `ExecStart`、工作目录、运行用户、重启策略和日志查看方式。
- 自动化部署不应隐藏验证；脚本结尾要输出版本、路径、服务状态或运行结果。

## 操作或演示

1. 演示部署脚本参数。

```bash
./deploy.sh -h
./deploy.sh -t <board-ip> -u <user> -m run
./deploy.sh -t <board-ip> -u <user> -m service
```

2. 脚本核心流程建议。

```bash
set -euo pipefail
make clean all
rsync -av build/ <user>@<board-ip>:/opt/rv-course/bin/
ssh <user>@<board-ip> '/opt/rv-course/bin/hello'
```

3. systemd unit 示例。

```ini
[Unit]
Description=RV Course Demo Service
After=network.target

[Service]
Type=simple
WorkingDirectory=/opt/rv-course
ExecStart=/opt/rv-course/bin/hello-loop
Restart=on-failure
User=%i

[Install]
WantedBy=multi-user.target
```

4. 板端安装、启动和查看日志。

```bash
sudo install -m 0644 rv-course@.service /etc/systemd/system/rv-course@.service
sudo systemctl daemon-reload
sudo systemctl enable --now rv-course@<user>.service
systemctl status rv-course@<user>.service
journalctl -u rv-course@<user>.service -n 50 --no-pager
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 参数帮助 | `./deploy.sh -h` | 输出目标 IP、用户、模式说明 |
| 一键部署 | `./deploy.sh -t <board-ip> -u <user> -m run` | 完成编译、同步和远程执行 |
| 服务启动 | `systemctl status rv-course@<user>.service` | 状态为 `active` 或能看到明确失败原因 |
| 日志查看 | `journalctl -u rv-course@<user>.service -n 50` | 能看到程序输出 |
| 开机自启 | `systemctl is-enabled rv-course@<user>.service` | 输出 `enabled` |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 脚本中变量含空格导致失败 | 未正确加引号 | 变量展开使用 `"${var}"` |
| `rsync` 提示命令不存在 | 主机或板端未安装 rsync | 安装 rsync，或临时改用 `scp -r` |
| systemd 找不到可执行文件 | 路径错误或权限不足 | 使用绝对路径，确认 `chmod +x` |
| 服务启动后立刻退出 | 程序不是长驻进程或参数错误 | 检查 `ExecStart`，用 `journalctl` 查看退出原因 |
| 开机后网络未就绪 | 服务依赖不足 | 增加 `After=network-online.target` 和相应 wants 配置 |

## 本讲成果

- 一个支持参数的 `deploy.sh`，能完成构建、传输和远程运行。
- 一个 systemd unit 文件，能管理课程示例程序。
- 一份部署验证记录，包含脚本输出、服务状态和日志片段。

