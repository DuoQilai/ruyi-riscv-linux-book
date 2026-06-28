# 实验 1.4 建立串口、SSH 与 SCP 开发通道

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第一章 RISC-V 开发生态与环境准备 |
| 讲次 | 1.4 |
| 课程主题 | SSH/SCP/串口登录与文件传输 |
| 实验类型 | 必做实验 |

## 实验目标

- 建立不依赖网络的串口备用调试通道。
- 建立可核验目标身份的 SSH 登录通道。
- 完成 SCP 双向传输与文件完整性校验。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 串口接线与终端参数 | 保存接线图和登录截图 |
| 2 | 板端网络地址 | 保存 `ip -br addr` |
| 3 | SSH 服务管理 | 保存服务状态 |
| 4 | 主机指纹 | 记录并核验指纹 |
| 5 | 密钥登录 | 执行远程命令验证 |
| 6 | SCP 与哈希 | 完成双向传输和 SHA-256 比对 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 安装 `ssh`、`scp`、串口终端工具 |
| 目标板 | LicheePi 4A |
| 目标系统 | 已启动的 RevyOS |
| 硬件连接 | 3.3 V USB-TTL、网络连接 |
| 软件依赖 | OpenSSH 客户端，板端 SSH 服务 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| USB-TTL | TX、RX、GND | TX/RX 交叉，GND 共地 | 断电接线，不接 5 V |
| 网络 | Ethernet 或 Wi-Fi | 主机与板端可互访 | 记录板端 IP |
| SSH | TCP 连接 | 主机登录板端 | 首次连接核对指纹 |

## 实验任务

### 任务 1：验证串口

观察启动日志或登录提示，执行系统信息命令。

### 任务 2：配置 SSH 密钥登录

确认服务和板端身份后配置公钥。

### 任务 3：完成 SCP 双向传输

上传测试文件、下载系统信息文件，并检查内容。

## 实验步骤

1. 断电完成串口接线，打开终端后启动开发板。
2. 在板端执行：

```bash
ip -br addr
sudo systemctl enable --now ssh
sudo systemctl status ssh --no-pager
```

3. 主机首次登录：

```bash
ssh <user>@<board-ip>
```

4. 配置密钥：

```bash
ssh-keygen -t ed25519 -C "riscv-course"
ssh-copy-id <user>@<board-ip>
ssh <user>@<board-ip> 'hostname; uname -m'
```

5. 上传并校验：

```bash
printf 'RISC-V course transfer test\n' > transfer-test.txt
sha256sum transfer-test.txt
scp transfer-test.txt <user>@<board-ip>:/tmp/
ssh <user>@<board-ip> 'sha256sum /tmp/transfer-test.txt'
```

6. 下载板端文件：

```bash
scp <user>@<board-ip>:/etc/os-release ./board-os-release.txt
sed -n '1,10p' board-os-release.txt
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 串口 | 能看到日志并进入 shell |  |
| SSH 服务 | 状态为 active |  |
| SSH 登录 | 返回正确主机名和 `riscv64` |  |
| 主机指纹 | 已通过可信通道核对 |  |
| 密钥认证 | 远程命令执行成功 |  |
| 上传校验 | 两端哈希一致 |  |
| 下载验证 | 获得正确的 RevyOS 信息 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | 串口参数、SSH、SCP 与哈希命令 |
| 关键输出 | IP、服务状态、主机名、架构、哈希 |
| 截图或照片 | 串口登录、SSH 登录、传输成功 |
| 异常处理 | 超时、拒绝连接、指纹变化或权限问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 串口乱码 | 波特率错误或线路不稳 | 按板卡资料重新设置并检查接线 |
| SSH 超时 | IP、网段、路由或防火墙问题 | 从串口检查地址、路由和服务 |
| 指纹冲突 | 重装系统或 IP 被复用 | 先核对目标身份，再更新记录 |
| SCP 权限失败 | 目标目录不可写 | 传到用户目录或 `/tmp` |

## 提交要求

- 串口接线图、终端参数和登录截图。
- 板端 IP、SSH 服务状态及主机指纹记录。
- 密钥登录远程命令输出。
- SCP 双向传输记录和两端哈希。
