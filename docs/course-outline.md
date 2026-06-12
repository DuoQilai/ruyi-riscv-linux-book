## 《RISC-V 嵌入式应用编程技术》课程大纲

> **目标平台**：LicheePi 4A（TH1520，RISC-V 64 位，4×C910）

> **操作系统**：RevyOS

> **外设**：面包板、LED 灯珠、按键、继电器模块、风扇模块、光敏/电位器、DHT22 温湿度传感器、OLED 显示屏（SSD1306）、USB 摄像头 、杜邦线 

> **总学时**：48 课时（6 章 × 4 讲 × 2 课时）  

> **知识点**：144 个（6 章 × 4 讲 × 6 知识点）

---

## 第一章 RISC-V C语言开发与调试

### 第1讲 RISC-V 生态与开发环境搭建

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | RISC-V 指令集架构概述 | 模块化 ISA 设计（RV64GC）、特权级别、与 x86/ARM 的异同 |
| 2 | RuyiSDK 与 开发板矩阵 | Sipeed LicheePi 4A、VisionFive 2、Milk-V Duo 等主流开发板对比 |
| 3 | LicheePi 4A 硬件认知 | TH1520 SoC 参数、40pin GPIO 引脚图、板载资源（USB/HDMI/Ethernet/MIPI） |
| 4 | RevyOS 镜像烧录 | 使用 fastboot/ruyi 烧录 RevyOS 镜像、分区结构 |
| 5 | 首次启动与系统配置 | 串口+HDMI 登录、网络配置、apt 换源、locale 与时区设置 |
| 6 | SSH/SCP 远程访问 | sshd 配置、免密登录（密钥对）、SCP 文件传输、串口终端（minicom/screen） |
| 🧪 实验 | LicheePi 4A 镜像烧录、网络配置与 SSH 登录 |

### 第2讲 交叉编译工具链与工程构建

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 交叉编译原理 | host/target 概念、RISC-V GNU Toolchain 组成（gcc/binutils/glibc） |
| 2 | RuyiSDK RISC-V 工具链安装与验证 | ruyi 包管理器安装 toolchain、环境变量、riscv64-unknown-linux-gnu-gcc --version |
| 3 | 第一个 RISC-V 程序 | Hello World 的编译->传输->运行全流程，file/readelf 观察 ELF 属性 |
| 4 | GCC 编译选项精讲 | -O0/-O2/-O3/-Os、-march=rv64gc、-mabi=lp64d、-static 与动态链接对比 |
| 5 | Makefile 编写与工程管理 | 规则/变量/自动变量、伪目标 clean/dist、多目录工程组织 |
| 6 | CMake 入门 | CMakeLists.txt 编写、out-of-source build、交叉编译 toolchain file |
| 🧪 实验 | 编写 Makefile 工程模板，交叉编译 Hello World 并部署到 LicheePi 4A |

### 第3讲 调试技术与版本控制

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | GDB 本地调试 | 断点/单步/监视变量/backtrace、TUI 模式、.gdbinit 配置 |
| 2 | GDB 远程交叉调试 | gdbserver 部署到 LicheePi 4A、gdb-multiarch 远程连接、sysroot 配置 |
| 3 | 段错误与内存问题排查 | core dump 生成与分析、AddressSanitizer（-fsanitize=address）使用 |
| 4 | strace/ltrace 系统调用追踪 | 程序行为观测、性能瓶颈定位、文件/网络调用追踪 |
| 5 | Git 基础操作 | init/clone/add/commit/push/pull、.gitignore、提交信息规范 |
| 6 | Git 分支管理与协作 | branch/checkout/merge/rebase、冲突解决、PR 流程 |
| 🧪 实验 | 远程 GDB 调试 LicheePi 4A 上的程序 + Git 分支管理实践 |

### 第4讲 Shell 编程与自动化部署

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | Shell 脚本基础 | Shebang、变量/引号/转义、exit code、set -e/-x 调试开关 |
| 2 | 条件判断与循环 | if/else/case、for/while/until、test/[ ] 条件表达式 |
| 3 | 函数与参数处理 | 函数定义/调用、$1-$n/$@/$#、getopts 命令行参数解析 |
| 4 | 文本处理工具链 | grep/sed/awk 实战、管道与重定向、日志分析与提取 |
| 5 | 自动化部署脚本 | 一键编译->传输->部署到 LicheePi 4A、rsync 增量同步 |
| 6 | systemd 服务管理 | unit 文件编写（.service）、systemctl enable/start/status、开机自启 |
| 🧪 实验 | 编写自动化部署脚本，实现编译->SCP 推送->远程执行一键流程，配置 systemd 服务 |

---

## 第二章 GPIO、UART、常用外设（传感器的读取与控制）

### 第1讲 面包板基础与 GPIO 输出

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 面包板接线规范 | 面包板内部连通结构、跳线颜色约定、排针排母使用、万用表测通断 |
| 2 | 电路安全基础 | 限流电阻选型（欧姆定律）、LED 极性识别、3.3V 与 5V 电平兼容 |
| 3 | Linux GPIO 子系统 | /sys/class/gpio 导出与控制（export/direction/value）、GPIO 编号计算 |
| 4 | 字符设备方式控制 GPIO | gpiod 库使用（gpiod_chip_open/gpiod_line_request_output）、与 sysfs 对比 |
| 5 | LED 闪烁（Blink） | C 程序控制 GPIO 输出高低电平、usleep 延时、频率控制 |
| 6 | 多路 LED 流水灯 | 数组化 GPIO 管理、时序控制、常见点亮模式（轮流/来回/随机） |
| 🧪 实验 | 面包板搭建 4 路 LED 流水灯电路，编写 gpiod 控制程序实现多种点亮模式 |

### 第2讲 GPIO 输入与交互逻辑

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 按键电路接线 | 上拉/下拉电阻原理、内部 pull-up 使能、按键抖动波形观测（逻辑分析仪/示波器） |
| 2 | GPIO 输入读取 | gpiod_line_request_input、轮询方式检测按键、active_low 配置 |
| 3 | 中断方式检测按键 | gpiod_line_request_rising_edge_events、epoll/poll 事件等待、边沿触发 vs 电平触发 |
| 4 | 按键消抖算法 | 软件延时消抖、多次采样确认、状态机消抖（IDLE->DOWN->HOLD->UP） |
| 5 | 组合按键与长按/短按识别 | 多按键状态矩阵、计时器区分长按/短按/双击 |
| 6 | 按键控制 LED 实战 | 单击切换、长按调亮度/速度、双击切换模式 |
| 🧪 实验 | 面包板搭建 2 按键 + 4 LED 电路，实现按键消抖、单击切换 LED 模式、长按调节流水灯速度 |

### 第3讲 UART 串口通信

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | UART 通信协议基础 | 起始位/数据位/校验位/停止位、TTL 电平 vs RS-232、全双工/半双工 |
| 2 | LicheePi 4A UART 引脚识别 | 40pin 中 UART 引脚分布（/dev/ttyS0、/dev/ttyTHS0）、设备树确认 |
| 3 | termios 串口配置 | 波特率/数据位/校验位/流控设置、tcgetattr/tcsetattr、cfsetspeed |
| 4 | 串口收发程序 | open/read/write UART 设备、阻塞/非阻塞模式、select 多路复用 |
| 5 | 环形缓冲区设计 | FIFO 循环队列实现、防止数据丢失/覆盖、生产者-消费者雏形 |
| 6 | 简易串口调试终端 | 串口命令解析（类似 AT 指令）、命令回显、帮助菜单 |
| 🧪 实验 | USB-TTL 模块连接 LicheePi 4A UART，编写双向串口通信程序，实现简易命令终端 |

### 第4讲 PWM/ADC/I2C/SPI 与综合传感器

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | PWM 输出控制 | /sys/class/pwm 导出与配置（period/duty_cycle）、呼吸灯实现、舵机角度控制、风扇调速 |
| 2 | ADC 模拟量采集 | IIO 子系统（/sys/bus/iio）、光敏电阻/电位器读数、电压换算与阈值检测 |
| 3 | I2C 总线与设备扫描 | i2cdetect 扫描设备地址、I2C 设备树节点、ioctl I2C_RDWR 通信 |
| 4 | OLED 显示屏驱动（SSD1306） | I2C 初始化序列、显存映射、ASCII 字符显示、C 库封装 |
| 5 | DHT22 温湿度传感器 | 单总线协议时序、握手指令、数据校验、结构体封装、多传感器编号与数据结构 |
| 6 | 温控风扇综合控制 | 双温度传感器采集、室内温度阈值与回差控制、GPIO/继电器或 MOSFET 驱动风扇、异常读数保护 |
| 🧪 综合项目 | 搭建 2 路温度传感器 + 风扇控制电路，检测室内温度低于/高于阈值时自动控制风扇转动，并在 OLED/串口输出实时状态 |

---

## 第三章 程序库、进程与线程

### 第1讲 静态库与共享库

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 库的概念与分类 | 静态库(.a)、共享库(.so)、运行时加载、头文件/二进制分离 |
| 2 | 静态库创建与使用 | ar 打包、ranlib 索引、-l-L 链接、与直接编译的体积/速度对比 |
| 3 | 共享库编译与版本管理 | -fPIC -shared、SONAME 与符号版本（libxxx.so.1.2.3）、ldconfig 缓存 |
| 4 | 动态链接过程 | ldd 查看依赖、LD_LIBRARY_PATH/rpath/runpath 搜索顺序、-rpath=$ORIGIN |
| 5 | 运行时动态加载 | dlopen/dlsym/dlclose/dlerror 使用、插件式架构设计 |
| 6 | LD_PRELOAD 注入与符号覆盖 | 函数劫持原理、malloc/free 追踪示例、调试/测试应用 |
| 🧪 实验 | 将 DHT22 驱动封装为共享库（libdht22.so），主程序通过 dlopen 动态加载并调用 |

### 第2讲 Linux 进程模型与控制

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 进程生命周期 | fork/vfork/clone、写时复制(CoW)、进程树（pstree）、PID/PPID 关系 |
| 2 | exec 函数族 | execl/execv/execle/execve 区别与用法、环境变量传递、脚本与二进制执行 |
| 3 | 进程退出与回收 | exit/_exit/return 区别、wait/waitpid 回收子进程、WIFEXITED/WIFSIGNALED 宏 |
| 4 | 僵尸进程与孤儿进程 | 产生条件、危害、预防策略（double fork 守护进程化、SIGCHLD 处理） |
| 5 | 守护进程编写 | setsid/chdir/umask/关闭 fd 标准步骤、syslog 日志输出 |
| 6 | 进程资源与限制 | getrlimit/setrlimit、/proc/PID/ 信息查看（maps/status/fd）、top/htop 监控 |
| 🧪 实验 | 编写守护进程化的传感器采集程序（fork+setsid），后台运行并输出 syslog 日志 |

### 第3讲 进程间通信（IPC）

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 匿名管道 | pipe/fd[0]/fd[1]、父子进程单向/双向通信、shell 管道的 C 实现 |
| 2 | 命名管道（FIFO） | mkfifo 创建、无亲缘进程通信、阻塞读写语义 |
| 3 | 消息队列 | POSIX mq_open/mq_send/mq_receive/mq_unlink、消息优先级、内核持久化 |
| 4 | 共享内存 | shm_open/ftruncate/mmap、与信号量配合使用、跨进程大数据传输 |
| 5 | 信号量同步 | POSIX sem_init/sem_wait/sem_post、命名信号量 sem_open、生产者-消费者保护 |
| 6 | 信号处理进阶 | sigaction 替代 signal、可靠信号/实时信号、sigqueue 发送带值信号、signalfd |
| 🧪 实验 | 采集进程->共享内存->显示进程 架构，信号量同步，实现传感器数据跨进程实时显示 |

### 第4讲 多线程编程与同步

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | pthread 线程生命周期 | pthread_create/join/detach、线程属性（栈大小/调度策略）、线程局部存储(TLS) |
| 2 | 互斥量（mutex） | pthread_mutex_init/lock/unlock/destroy、死锁条件与预防、lockdep 分析 |
| 3 | 条件变量 | pthread_cond_wait/signal/broadcast、与 mutex 配合、虚假唤醒处理 |
| 4 | 读写锁与自旋锁 | pthread_rwlock_rdlock/wrlock（多读单写）、pthread_spin_lock（短临界区） |
| 5 | 生产者-消费者模型 | 线程安全有界队列实现、数据采集线程->处理线程->显示线程流水线 |
| 6 | 线程池设计与实现 | 任务队列、worker 线程循环、submit/wait 接口、动态扩缩思路 |
| 🧪 实验 | 多线程传感器终端：采集线程->共享队列->OLED 显示线程，mutex+条件变量同步 |

---

## 第四章 文件系统与网络编程

### 第1讲 文件 I/O 基础与高级操作

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | POSIX 文件 I/O | open/creat/read/write/lseek/close 详解、文件描述符本质、O_APPEND/O_TRUNC 语义 |
| 2 | 标准 C 库文件操作 | fopen/fread/fwrite/fprintf/fscanf、缓冲模式（全缓冲/行缓冲/无缓冲）、setvbuf |
| 3 | 文件属性与元数据 | stat/lstat/fstat、文件类型/权限/大小/时间戳、access 权限检查 |
| 4 | 内存映射 I/O（mmap） | 文件映射到内存、MAP_SHARED/MAP_PRIVATE、与 read/write 性能对比、大文件处理 |
| 5 | 文件锁与并发保护 | fcntl F_SETLK/F_GETLK 劝告锁、flock、多进程共享文件安全 |
| 6 | inotify 文件监控 | inotify_init/add_watch/read 事件循环、配置文件热加载实现 |
| 🧪 实验 | 传感器数据通过 mmap 写入文件，inotify 监控数据文件变化并触发处理 |

### 第2讲 配置管理与日志系统

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 命令行参数解析 | getopt/getopt_long、参数验证、--help 帮助信息生成 |
| 2 | INI 配置文件解析 | 自解析或 inih 库、section/key 结构、默认值与校验 |
| 3 | JSON 配置文件解析 | cJSON/json-c 库使用、schema 验证、配置热加载 |
| 4 | 日志级别设计 | DEBUG/INFO/WARN/ERROR/FATAL、编译期级别开关、函数/文件/行号宏 |
| 5 | 日志输出与轮转 | 同时输出 stdout + 文件、按大小/时间轮转（logrotate 或自实现）、syslog 集成 |
| 6 | 工程目录结构规范 | src/include/build/doc/log/config/ 标准布局、头文件保护与模块化 |
| 🧪 实验 | 为传感器终端添加 JSON 配置（采样频率/阈值/OLED 开关）+ 多级日志系统 |

### 第3讲 网络编程基础

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 网络协议栈概述 | OSI/TCP-IP 模型、IP 地址/子网掩码、TCP vs UDP 适用场景、端口概念 |
| 2 | TCP Socket 编程 | socket/bind/listen/accept/connect、流式传输、粘包/拆包处理、优雅关闭 |
| 3 | UDP Socket 编程 | sendto/recvfrom、无连接特性、广播/组播、丢包应对 |
| 4 | 字节序与数据封包 | htonl/htons、结构体对齐与序列化、定长/变长协议设计 |
| 5 | 主机名与地址解析 | getaddrinfo/getnameinfo（取代 gethostbyname）、IPv4/IPv6 双栈兼容 |
| 6 | Socket 选项与超时 | SO_REUSEADDR/SO_KEEPALIVE/TCP_NODELAY、setsockopt 超时、非阻塞模式 |
| 🧪 实验 | LicheePi 4A 作为 TCP Server，发送传感器 JSON 数据，PC 端编写 Client 接收并显示 |

### 第4讲 并发服务器、HTTP 与 MQTT 服务

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 多进程并发服务器 | fork-per-connection 模型、SIGCHLD 处理僵尸进程、prefork 进程池 |
| 2 | 多线程并发服务器 | thread-per-connection、线程池模型、连接数上限与拒绝服务防护 |
| 3 | I/O 多路复用 | select/poll/epoll 原理对比、epoll_create/epoll_ctl/epoll_wait、边缘触发 vs 水平触发 |
| 4 | HTTP 协议基础 | 请求/响应格式、GET/POST 方法、状态码、Content-Type、URL 解码 |
| 5 | 嵌入式 HTTP 服务器实现 | 基于 epoll 的单线程事件驱动 HTTP 服务器、路由分发、静态文件服务 |
| 6 | MQTT 与 mosquitto 消息通信 | mosquitto broker/client、topic 设计、publish/subscribe、JSON 指令、状态回传、手机 MQTT 客户端接入 |
| 🧪 综合项目 | LicheePi 4A 连接灯光模块，基于 mosquitto 接收手机 MQTT 程序发出的指令，结合 GPIO/PWM 实现远程开关与亮度调节 |

---

## 第五章 RISC-V 向量扩展（RVV）加速编程

### 第1讲 RISC-V 向量扩展体系结构

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | RVV 设计哲学与优势 | 与 SIMD（NEON/SSE/AVX）对比、向量长度无关(VLA)设计、寄存器重命名 |
| 2 | 向量寄存器与 CSR | v0-v31 向量寄存器、vtype/vl/vstart/vxsat 控制寄存器、vlenb/vlen 概念 |
| 3 | 向量指令分类 | 算术/逻辑/比较/归约/排列/加载存储、掩码(mask)操作、向量长度 vl 管理 |
| 4 | LicheePi 4A 的 RVV 能力 | C910 向量单元参数（VLEN=128bit）、TH1520 RVV 0.7.1 与 1.0 兼容性 |
| 5 | RVV 汇编基础 | vsetvli/vadd.vv/vle.v/vse.v 指令格式、stripmining 循环模式 |
| 6 | GCC/Clang RVV 编译支持 | -march=rv64gcv 编译选项、自动向量化 -ftree-vectorize、汇编输出 -S 观察 |
| 🧪 实验 | 编写 RVV 汇编版 memcpy/saxpy，对比标量版本性能 |

### 第2讲 RVV Intrinsics 编程

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | Intrinsics 编程模型 | riscv_vector.h 头文件、vuint8m1_t/vfloat32m4_t 等向量类型、__riscv_ 前缀 API |
| 2 | 向量长度设置与加载 | vsetvl_e8m1/vsetvlmax_e32m4、vle8_v_u8m1/vle32_v_f32m1 加载数据 |
| 3 | 向量算术与逻辑运算 | vadd/vsub/vmul/vfadd、带掩码运算 (vadd_vx_i32m1_m)、fma 融合乘加 |
| 4 | 向量存储与段操作 | vse8/vse32 存储结果、vlsseg/vssseg 跨步/分段加载存储（矩阵场景） |
| 5 | 向量归约与排列 | vredsum/vredmax 归约、vrgather/vslideup/vslidedown 数据重排 |
| 6 | 向量与标量混合编程 | C 函数中嵌入 intrinsics、编译器优化屏障、volatile 与内存序 |
| 🧪 实验 | 使用 RVV intrinsics 编写图像处理算法加速，对比标量版本性能并在 LicheePi 4A 上记录加速比 |

### 第3讲 常用算法向量化优化

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 向量化 memcpy/memset | 对齐检测、尾量处理、大块/小块分支、vs8 全寄存器宽度 |
| 2 | 向量化矩阵乘法 | 分块策略、寄存器驻留 tiles、vfmacc 融合乘加、行优先/列优先布局 |
| 3 | 向量化卷积运算 | 1D 卷积展开、2D im2col + GEMM 转换、vsilde 窗口滑动 |
| 4 | 向量化激活函数 | ReLU（vmax + 掩码）、Sigmoid 查表法、Tanh 多项式逼近 |
| 5 | 向量化图像处理 | RGB 转灰度（加权和）、Sobel 边缘检测、直方图统计（vredsum） |
| 6 | 定点量化推理加速 | int8 向量乘加、vredsum 累加、量化反量化 SIMD 优化 |
| 🧪 实验 | 实现 RVV 加速的矩阵乘法（SGEMM），对比 naive C 版本在 LicheePi 4A 上的吞吐量 |

### 第4讲 向量化性能分析与调优

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 性能测量方法论 | rdcycle/rdtime 读周期计数器、重复+预热消除噪声、统计显著性 |
| 2 | perf 工具使用 | perf stat 事件计数、perf record/report 热点分析、cache miss/branch miss 解读 |
| 3 | 内存带宽瓶颈分析 | Roofline 模型、stream benchmark、LicheePi 4A 实测带宽与峰值算力 |
| 4 | 循环展开与软件流水 | 编译器展开 #pragma GCC unroll、手工展开 trade-off、指令级并行 |
| 5 | 数据对齐与预取 | __attribute__((aligned(128)))、手动预取 vprefetch、cache line 对齐避免 false sharing |
| 6 | RVV 与 OpenBLAS 集成 | 编译 RVV 后端 OpenBLAS、调用 GEMM/GEMV、与标量 BLAS 性能对比 |
| 🧪 实验 | 使用 perf 分析上一讲 SGEMM 性能瓶颈，逐项优化并记录加速比变化 |

---

## 第六章 机器学习（OpenCV、TinyML 边缘推理、大语言模型）

### 第1讲 摄像头接入与 OpenCV 图像处理

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | USB 摄像头驱动与检测 | V4L2 设备模型（/dev/video0）、v4l2-ctl 查询格式与分辨率、guvcview 预览 |
| 2 | V4L2 C 语言图像采集 | open/ioctl(VIDIOC_QUERYCAP/S_FMT/STREAMON)、mmap 映射帧缓冲、YUV->RGB 转换 |
| 3 | OpenCV RISC-V 编译部署 | 源码编译（-DWITH_OPENMP=ON）、CMake 交叉编译、依赖库（libjpeg/libpng） |
| 4 | OpenCV 基础图像处理 | cv::imread/imshow、cvtColor 色彩空间、GaussianBlur/Canny 边缘检测、resize |
| 5 | 实时视频流处理 | cv::VideoCapture 帧循环、FPS 计算、ROI 区域提取 |
| 6 | 图像保存与网络传输 | cv::imencode -> JPEG 压缩 -> Socket 发送（MJPEG 流）、LicheePi 4A 视频推流 |
| 🧪 实验 | USB 摄像头连接 LicheePi 4A，OpenCV 实时采集+Canny 边缘检测，MJPEG 推流到 PC 浏览器 |

### 第2讲 目标检测与推理框架

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | 目标检测算法概述 | 两阶段 vs 单阶段、Haar Cascade->HOG+SVM->YOLO 演进、mAP 评估指标 |
| 2 | OpenCV DNN 模块 | cv::dnn::readNet 加载模型、blobFromImage 预处理、forward 推理、NMS 后处理 |
| 3 | ncnn 推理框架 | RISC-V 后端编译、模型转换（ONNX->ncnn）、param/bin 加载、extract 取输出 |
| 4 | 轻量级检测模型部署 | NanoDet/MobileNet-SSD 部署到 LicheePi 4A、推理耗时测量 |
| 5 | 模型量化与优化 | FP32->FP16->INT8 量化流程、校准数据集、精度损失评估 |
| 6 | 实时人脸检测实战 | Haar Cascade 人脸检测、帧率优化、检测框叠加显示 |
| 🧪 实验 | 在 LicheePi 4A 上部署 NanoDet（ncnn），USB 摄像头实时目标检测并显示 FPS |

### 第3讲 TinyML 边缘推理实战

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | TinyML 概念与适用场景 | MCU/嵌入式 NPU 推理、模型 <100KB、功耗约束、唤醒词/手势/异常检测 |
| 2 | TensorFlow Lite Micro 编译 | tflite-micro RISC-V 移植、Makefile 集成、AllOpsResolver vs 自定义 ops |
| 3 | 模型训练与导出 | Google Colab 训练 KWS 模型、TF->TFLite->C array（xxd）转换、weights.h 生成 |
| 4 | 语音特征提取（MFCC） | 预加重->分帧->加窗->FFT->Mel 滤波器组->DCT、C 语言实现 |
| 5 | int8 推理引擎集成 | 模型加载器、Conv2D/DepthwiseConv/FC 层实现、量化反量化节点 |
| 6 | 关键词唤醒控灯系统 | 麦克风采集->MFCC 实时提取->TinyML 推理->GPIO 控 LED 完整 pipeline |
| 🧪 实验 | USB 麦克风 + LicheePi 4A 实现"开灯""关灯"语音关键词唤醒，GPIO 控制 LED/继电器 |

### 第4讲 大语言模型与视觉大语言模型

| # | 知识点 | 内容 |
|---|--------|------|
| 1 | LLM 推理基础 | Transformer 架构回顾、自回归解码、KV Cache 机制、tokenizer 原理 |
| 2 | llama.cpp RISC-V 编译 | RVV 加速后端（GGML RISC-V kernels）、量化格式（Q4_0/Q4_K_M）、cmake 编译 |
| 3 | 小型 LLM 本地部署 | Qwen2.5-0.5B/SmolLM2-135M 等轻量模型、Prompt 模板、流式输出 |
| 4 | LLM + 传感器融合 | 自然语言查询传感器数据、LLM 生成 GPIO 控制指令、Agent 工具调用 |
| 5 | 视觉大语言模型（VLM）概述 | LLaVA/MiniCPM-V 架构、vision encoder + projection + LLM、多模态输入 |
| 6 | VLM RISC-V 部署与调优 | llama.cpp llava 支持、tiny-vision 模型、图像理解与问答 |
| 🧪 实验 | LicheePi 4A 部署视觉大语言模型（llama.cpp），完成图像理解与问答演示 |

---

## 附录

### A. 课程实验硬件清单

| 类别 | 物品 | 数量 | 用途 |
|------|------|------|------|
| 核心板 | Sipeed LicheePi 4A（8GB RAM） | 1 | 主控开发板 |
| 配件 | USB-TTL 串口模块、Type-C 供电线、MicroSD 卡（>=32GB） | 各1 | 调试/存储 |
| 面包板 | 830 孔面包板 + 公母杜邦线若干 | 1 套 | 电路搭建 |
| 显示 | SSD1306 OLED 模块（I2C） | 1 | 数据可视化 |
| 传感器 | DHT22 温湿度 x2、光敏电阻模块、电位器 | 若干 | 环境采集、双点温度比较 |
| 执行器 | LED（红绿蓝白各 5）+ 220R/1kR 电阻包、继电器模块、风扇模块、可调光灯光模块 | 若干 | 输出控制、温控风扇、远程灯光控制 |
| 输入 | 轻触按键 x4、USB 麦克风 | 各1 | 交互输入/语音 |
| 摄像 | USB 摄像头（UVC 免驱） | 1 | 图像采集 |

### B. 各章学时与实验分布

| 章节 | 讲数 | 课时 | 知识点 | 实验数 |
|------|------|------|--------|--------|
| 第一章 开发与调试 | 4 | 8 | 24 | 4 |
| 第二章 GPIO 与外设 | 4 | 8 | 24 | 3 + 1 综合项目 |
| 第三章 库/进程/线程 | 4 | 8 | 24 | 4 |
| 第四章 文件系统与网络 | 4 | 8 | 24 | 3 + 1 综合项目 |
| 第五章 RVV 向量加速 | 4 | 8 | 24 | 4 |
| 第六章 机器学习 | 4 | 8 | 24 | 4 |
| **合计** | **24** | **48** | **144** | **24** |

> **教学建议**：每章安排一个综合性大实验作为能力检验节点。第六章大实验可打通前五章能力（GPIO 控灯 + 多线程采集 + MQTT 远程控制 + LLM 对话），形成完整边缘智能终端作品。
