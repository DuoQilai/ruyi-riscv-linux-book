# Class 4：SSH/SCP/串口登录与文件传输

## 本讲目标

- 能说明串口登录与 SSH 登录的差异，并为故障排查选择合适通道。
- 能完成 SSH 首次连接、主机指纹核对和密钥登录配置。
- 能使用 SCP 在主机与开发板之间双向传输文件并校验内容。

## 前置条件

- LicheePi 4A 已启动 RevyOS。
- 主机与开发板已接入同一可达网络。
- 已准备 3.3 V USB-TTL 串口模块，并确认 TX、RX、GND 接法。

## 知识简介

串口是直接连接开发板的本地调试通道，不依赖网络，适合观察启动日志和修复网络配置。SSH（Secure Shell）通过网络提供加密远程终端，适合日常开发。SCP（Secure Copy）复用 SSH 连接传输文件。稳定课程环境应同时保留串口备用通道和 SSH 主工作通道。

图 1-4 建议展示：

```text
主机
 ├─ USB-TTL ── 串口 ──> 启动日志与本地 shell
 └─ Ethernet/Wi-Fi ── SSH ──> 远程 shell
                       └─ SCP ──> 文件传输
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 网络 | 主机可到达板端 IP | `ping <board-ip>` 或路由检查 |
| SSH 客户端 | 主机具备 `ssh`、`scp` | `ssh -V` |
| SSH 服务 | 板端服务已安装并运行 | `systemctl status ssh` |
| 串口 | 3.3 V USB-TTL 与终端程序 | 检查设备节点 |

## 操作步骤

### 步骤 1：建立串口连接

断电接线，连接 `GND-GND`、主机模块 `TX` 到板端 `RX`、主机模块 `RX` 到板端 `TX`，不要连接 5 V 电源脚。打开终端工具并设置板卡资料指定的波特率。

### 步骤 2：检查板端 SSH 服务与地址

```bash
ip -br addr
sudo systemctl enable --now ssh
sudo systemctl status ssh --no-pager
```

记录板端 IP。若系统服务名不同，应使用 `systemctl list-unit-files | grep -i ssh` 查询。

### 步骤 3：首次 SSH 登录

```bash
ssh <user>@<board-ip>
```

首次连接会显示主机指纹。应通过串口或受信任渠道核对，而不是习惯性输入 `yes`。登录后执行 `hostname` 和 `uname -m`，确认进入的是目标板。

### 步骤 4：配置密钥登录

```bash
ssh-keygen -t ed25519 -C "riscv-course"
ssh-copy-id <user>@<board-ip>
ssh <user>@<board-ip> 'hostname; uname -m'
```

私钥留在主机，不能复制给他人。公钥写入板端用户的 `~/.ssh/authorized_keys`。

### 步骤 5：双向传输并校验

```bash
printf 'RISC-V course transfer test\n' > transfer-test.txt
sha256sum transfer-test.txt
scp transfer-test.txt <user>@<board-ip>:/tmp/
ssh <user>@<board-ip> 'sha256sum /tmp/transfer-test.txt'
scp <user>@<board-ip>:/etc/os-release ./board-os-release.txt
```

主机和板端哈希一致，说明文件内容未变化。

## 课堂练习

分别说明以下故障应优先使用串口还是 SSH：板端没有 IP、sshd 配置错误、系统启动失败、需要批量上传文件。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 串口 | 能看到启动日志或登录提示 | 保存截图 |
| SSH 服务 | 状态为 active | 保存状态 |
| SSH 登录 | 远程命令返回板端主机名和 `riscv64` | 保存输出 |
| 密钥登录 | 不再要求账户密码或按课程策略使用密钥 | 记录结果 |
| SCP 上传 | 板端文件哈希与主机一致 | 保存哈希 |
| SCP 下载 | 主机获得板端系统信息文件 | 保存文件 |

## 常见问题

### SSH 连接超时

通常是 IP、路由、网段或防火墙问题。先在串口中检查 `ip addr`、`ip route` 和 SSH 服务状态。

### 主机指纹发生变化

可能是开发板重装系统，也可能连接了另一台设备。先核对 IP 和串口中的主机指纹，再清理旧记录。

### SCP 提示权限不足

先传到用户有写权限的目录，如用户家目录或 `/tmp`，再在板端使用受控权限移动文件。

## 本讲成果

- 串口接线与终端参数记录。
- SSH 服务状态、板端 IP 和主机指纹记录。
- SSH 密钥登录验证。
- SCP 双向传输与哈希校验结果。
