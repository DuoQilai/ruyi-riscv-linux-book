# cmd-thermo · 终端命令温控（第四章）

荔枝派 4A：自动按温度/湿度控风扇，同时在 SSH 终端敲 `status` / `set`。

工程目录：`chapters/ch04/code/cmd-thermo/`（与第三章 `temperature-fan/` 一样，一章一个工程子目录）。

## 学生要写（标了 STUDENT TODO）

打开 `main.c`，在 `STUDENT TODO` 区块实现（契约见 `lab.html`）：

1. `cmd_status` / `cmd_set`
2. 命令表 `g_cmds[]` 与 `dispatch_command`
3. `main_loop`（`select`）

脚手架已提供：DHT 读取、风扇、`sample_and_control`、`read_command_line`、`main`。

对照答案：`main-sol.c`（`make sol`）。先自己写，过关后再看。

## 构建

```bash
make          # 学生版 → ./cmd-thermo
make sol      # 参考实现 → ./cmd-thermo-sol
```

默认 `SIMULATE_SENSOR=1`。接真 DHT22：

```bash
make CFLAGS='-Wall -Wextra -O2 -DSIMULATE_SENSOR=0'
```

命令从 SSH 标准输入读入。
