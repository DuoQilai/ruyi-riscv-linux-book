# 实验 3.1：DHT22 驱动封装为共享库

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第三章 程序库、进程与线程 |
| 讲次 | 第 1 讲 |
| 课程主题 | 静态库与共享库 |
| 实验类型 | 必做实验 |

大纲讲次原文：第1讲 静态库与共享库。
大纲实验原文：将 DHT22 驱动封装为共享库（libdht22.so），主程序通过 dlopen 动态加载并调用
大纲知识点原文：库的概念与分类；静态库创建与使用；共享库编译与版本管理；动态链接过程；运行时动态加载；LD_PRELOAD 注入与符号覆盖。

## 实验目标

- 建立 `include/`、`src/`、`app/`、`lib/`、`build/` 工程结构。
- 生成 `libdht22.a` 和 `libdht22.so.1.0.0`。
- 编译普通链接程序和 `dlopen` 加载程序。
- 使用 `ldd`、`readelf`、`nm` 和运行日志验证库路径、符号和版本。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 库的概念与分类 | 对比静态链接程序和动态链接程序的体积与部署方式 |
| 2 | 静态库创建与使用 | 使用 `ar rcs` 生成 `libdht22.a` 并链接运行 |
| 3 | 共享库编译与版本管理 | 使用 `-fPIC -shared` 和 SONAME 生成版本化 `.so` |
| 4 | 动态链接过程 | 使用 `ldd`、`LD_LIBRARY_PATH`、`rpath` 验证搜索路径 |
| 5 | 运行时动态加载 | 使用 `dlopen/dlsym/dlclose/dlerror` 调用库函数 |
| 6 | LD_PRELOAD 注入与符号覆盖 | 可选：预加载 mock 库替换 `dht22_read` 以返回固定数据 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 已安装课程指定 RISC-V Linux 工具链，能通过 SSH/SCP 访问板端 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 数据脚连接到课程指定 3.3V GPIO，电源和地线连接正确 |
| 软件依赖 | `gcc`、`make`、`binutils`、`ldd`、`libdl`；板端可本地编译或运行交叉编译产物 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| DHT22 VCC | 3.3V | 接 LicheePi 4A 3.3V | 不使用 5V 信号直接输入 GPIO |
| DHT22 GND | GND | 与开发板共地 | 接触不良会导致读数超时 |
| DHT22 DATA | 课程指定 GPIO | DATA 到 GPIO，中间按模块要求接上拉 | GPIO 编号以第二章实测表为准 |
| 程序部署 | `/home/debian/ch03_lab1` | `bin/` 放程序，`lib/` 放共享库 | 运行前确认库路径 |

## 实验任务

### 任务 1：整理库接口

创建 `include/dht22.h`，声明初始化、读取和关闭函数；把硬件相关实现放入 `src/dht22.c`。

### 任务 2：构建静态库和共享库

编写 Makefile，生成 `lib/libdht22.a`、`lib/libdht22.so.1.0.0` 以及软链接。

### 任务 3：编写两个测试程序

`app/main_link.c` 使用普通动态链接调用；`app/main_dlopen.c` 通过命令行传入库路径并使用 `dlsym` 查找函数。

### 任务 4：部署到板端验证

将 `bin/` 和 `lib/` 上传到 LicheePi 4A，在 RevyOS 上运行并记录输出。

## 实验步骤

```bash
mkdir -p ch03_lab1/{include,src,app,build,lib,bin}
cd ch03_lab1
```

```bash
# 编译对象文件
gcc -Iinclude -c src/dht22.c -o build/dht22.o
gcc -Iinclude -fPIC -c src/dht22.c -o build/dht22.pic.o

# 静态库
ar rcs lib/libdht22.a build/dht22.o
ranlib lib/libdht22.a

# 共享库
gcc -shared -Wl,-soname,libdht22.so.1 -o lib/libdht22.so.1.0.0 build/dht22.pic.o
ln -sf libdht22.so.1.0.0 lib/libdht22.so.1
ln -sf libdht22.so.1 lib/libdht22.so
```

```bash
# 普通链接程序
gcc -Iinclude app/main_link.c -Llib -ldht22 -Wl,-rpath,'$ORIGIN/../lib' -o bin/dht22_link_demo

# dlopen 程序
gcc -Iinclude app/main_dlopen.c -ldl -o bin/dht22_dlopen_demo
```

```bash
# 本地或板端检查
file bin/dht22_link_demo lib/libdht22.so.1.0.0
readelf -d bin/dht22_link_demo | grep -E 'NEEDED|RPATH|RUNPATH'
nm -D lib/libdht22.so | grep dht22
ldd bin/dht22_link_demo
```

```bash
# 上传到板端，IP 按实际修改
scp -r bin lib debian@<board-ip>:/home/debian/ch03_lab1/
ssh debian@<board-ip>
cd /home/debian/ch03_lab1
./bin/dht22_link_demo --gpio <gpio-number>
./bin/dht22_dlopen_demo ./lib/libdht22.so --gpio <gpio-number>
```

```bash
# 故障验证
mv lib/libdht22.so lib/libdht22.so.bak
./bin/dht22_dlopen_demo ./lib/libdht22.so --gpio <gpio-number>
mv lib/libdht22.so.bak lib/libdht22.so
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 静态库生成 | `lib/libdht22.a` 存在，`file` 显示 ar archive |  |
| 共享库生成 | `lib/libdht22.so.1.0.0` 存在，软链接完整 |  |
| 动态依赖 | `ldd` 能找到 `libdht22.so.1` |  |
| 普通调用 | 程序输出温度、湿度、时间戳或模拟读数 |  |
| dlopen 调用 | 使用库路径运行成功，错误路径输出 `dlerror` |  |
| 板端运行 | LicheePi 4A 上输出稳定，无架构错误 |  |

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
| 板端提示 `No such file or directory` | 动态链接器或架构不匹配 | 用 `file` 检查是否为 RISC-V 64 位 Linux 可执行文件 |
| 找不到 `libdht22.so.1` | 未上传库或 rpath 不正确 | 保持 `bin/../lib` 目录关系，或临时设置 `LD_LIBRARY_PATH` |
| `dlsym` 返回空 | 函数未导出或函数名拼写错误 | 用 `nm -D` 检查符号 |
| 读数一直超时 | GPIO 编号、上拉或供电错误 | 回到第二章 DHT22 示例确认硬件读数 |

## 提交要求

- 实验记录：工程目录树、构建命令、板端运行命令。
- 运行截图：`ldd`、`readelf`、两种调用程序输出。
- 源码或配置文件：`dht22.h`、`dht22.c`、两个主程序、Makefile。
- 简短说明：说明静态库、共享库和 `dlopen` 方式的差异。
