# 3.1 静态库与共享库

## 对应大纲

大纲讲次原文：第1讲 静态库与共享库。
大纲知识点原文：库的概念与分类；静态库创建与使用；共享库编译与版本管理；动态链接过程；运行时动态加载；LD_PRELOAD 注入与符号覆盖。

库的概念与分类、静态库创建与使用、共享库编译与版本管理、动态链接过程、运行时动态加载、LD_PRELOAD 注入与符号覆盖。

## 目标

学生能把 DHT22 采集代码拆分为头文件、实现文件和库文件，能分别构建静态库与共享库，并在 LicheePi 4A + RevyOS 上通过链接或 `dlopen` 调用库函数。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 库的概念与分类 | 静态库、共享库、运行时加载库的区别，头文件与二进制库的关系 |
| 2 | 静态库创建与使用 | 使用 `ar`、`ranlib`、`-L`、`-l` 构建和链接 `.a` 文件 |
| 3 | 共享库编译与版本管理 | `-fPIC`、`-shared`、SONAME、软链接和 `ldconfig` 缓存 |
| 4 | 动态链接过程 | `ldd`、动态链接器搜索路径、`LD_LIBRARY_PATH`、`rpath`、`$ORIGIN` |
| 5 | 运行时动态加载 | `dlopen`、`dlsym`、`dlclose`、`dlerror` 与插件式接口 |
| 6 | LD_PRELOAD 注入与符号覆盖 | 通过预加载库覆盖函数，用于测试、诊断和行为追踪 |

## 讲授要点

- 静态库在链接时被合入可执行文件，部署简单但更新成本高；共享库在运行时加载，适合多个程序复用和独立升级。
- 头文件是接口契约，库文件是接口实现。课程中建议把 `include/dht22.h` 作为公开接口，把 GPIO 时序细节隐藏在 `src/dht22.c` 中。
- 编译共享库时必须使用位置无关代码：`gcc -fPIC -c src/dht22.c -o build/dht22.o`，再用 `gcc -shared -Wl,-soname,libdht22.so.1` 生成库。
- `LD_LIBRARY_PATH` 适合实验调试，正式工程优先使用安装路径、`ldconfig` 或 `$ORIGIN` rpath，避免学生把临时路径写死到 shell 配置中。
- `dlopen` 适合可选模块和插件化传感器接口，必须检查每一步返回值，并用 `dlerror()` 输出具体错误。
- `LD_PRELOAD` 功能强大但有安全边界，本课程只用于追踪 `open/read/write` 或模拟传感器函数，不用于绕过权限。

## 操作或演示

1. 准备最小库接口。

```c
// include/dht22.h
#pragma once

typedef struct {
    float temperature;
    float humidity;
} dht22_sample_t;

int dht22_init(int gpio);
int dht22_read(dht22_sample_t *out);
void dht22_close(void);
```

2. 构建静态库并链接。

```bash
mkdir -p build lib
gcc -Iinclude -c src/dht22.c -o build/dht22.o
ar rcs lib/libdht22.a build/dht22.o
gcc -Iinclude app/main.c -Llib -ldht22 -o build/dht22_static_demo
```

3. 构建共享库并观察依赖。

```bash
gcc -Iinclude -fPIC -c src/dht22.c -o build/dht22.pic.o
gcc -shared -Wl,-soname,libdht22.so.1 -o lib/libdht22.so.1.0.0 build/dht22.pic.o
ln -sf libdht22.so.1.0.0 lib/libdht22.so.1
ln -sf libdht22.so.1 lib/libdht22.so
gcc -Iinclude app/main.c -Llib -ldht22 -Wl,-rpath,'$ORIGIN/../lib' -o build/dht22_shared_demo
ldd build/dht22_shared_demo
```

4. 演示运行时动态加载。

```bash
gcc -Iinclude app/dlopen_demo.c -ldl -o build/dlopen_demo
LD_LIBRARY_PATH=$PWD/lib ./build/dlopen_demo ./lib/libdht22.so
```

5. 演示库路径错误和符号错误，让学生观察 `dlerror()` 与 `ldd` 的差异。

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 静态库存在 | `file lib/libdht22.a` | 显示 current ar archive |
| 共享库存在 | `file lib/libdht22.so.1.0.0` | 显示 RISC-V 64-bit shared object |
| 动态依赖 | `ldd build/dht22_shared_demo` | 能找到 `libdht22.so.1` |
| rpath 生效 | `readelf -d build/dht22_shared_demo | grep -E 'RPATH|RUNPATH'` | 输出 `$ORIGIN/../lib` |
| 动态加载 | `./build/dlopen_demo ./lib/libdht22.so` | 输出一次温湿度或模拟读数 |
| 错误诊断 | `./build/dlopen_demo ./lib/not-exist.so` | 输出库打开失败原因 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `cannot open shared object file` | 共享库不在动态链接器搜索路径中 | 使用 `LD_LIBRARY_PATH` 临时验证，或设置 `$ORIGIN` rpath |
| `undefined symbol` | 头文件声明与库实现不一致，或链接了旧库 | `nm -D lib/libdht22.so` 检查导出符号，清理后重编 |
| `relocation ... recompile with -fPIC` | 共享库对象文件未使用位置无关代码 | 用 `-fPIC` 重新编译 `.o` |
| 板端运行提示架构不匹配 | 在主机上用本机 gcc 编译了目标程序 | 使用课程指定 RISC-V 工具链或在板端本地编译 |

## 本讲成果

- 能说明静态库、共享库和运行时加载库的适用场景。
- 能构建 `libdht22.a`、`libdht22.so` 和两个调用示例。
- 能使用 `ldd`、`readelf`、`nm`、`dlerror` 定位库加载问题。
- 为后续守护进程、IPC 和多线程示例提供统一的传感器访问接口。
