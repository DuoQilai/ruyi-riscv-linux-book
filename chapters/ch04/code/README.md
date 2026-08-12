# 第四章实验代码 · 终端命令温控

工程目录：`chapters/ch04/code/`（扁平，无子目录）。  
板上同步到：`~/ruyi-riscv-embedded/chapter04-lab/`。

## 文件

| 文件 | 作用 |
|------|------|
| `cmd_thermo.c` | 学生版（STUDENT TODO） |
| `cmd_thermo-sol.c` | 参考实现 |
| `Makefile` | `make` / `make sol` |

## 学生要写

打开 `cmd_thermo.c`，实现（契约见 `lab.html`）：

- `cmd_status` / `cmd_set`
- 命令表 `g_cmds[]`
- `dispatch_command`
- `main_loop`（`select`）

先自己写；过关后再对照 `cmd_thermo-sol.c`。

## 编译

```bash
make          # 学生版 → ./cmd_thermo
make sol      # 参考实现 → ./cmd_thermo-sol
```

默认 `SIMULATE_SENSOR=1`。接真 DHT22：

```bash
make sol CFLAGS='-Wall -Wextra -O2 -DSIMULATE_SENSOR=0'
sudo ./cmd_thermo-sol
```
