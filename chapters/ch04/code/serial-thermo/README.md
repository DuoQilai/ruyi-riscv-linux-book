# serial-thermo · 串口命令温控

荔枝派 4A 上：自动阈值控风扇，同时用终端敲 `status` / `set`。

交叉工具链默认 `riscv64-unknown-linux-gnu-`（RuyiSDK）。板端需 libgpiod。

## 你要补全

`main.c` 中 TODO：

1. `cmd_status` / `cmd_set` 实现  
2. 命令表 `g_cmds[]` 注册与查找  
3. `select` 主循环：同时等采样超时与串口（stdin）输入  

## 构建

```bash
make
file serial-thermo
scp serial-thermo user@board-ip:~/
ssh user@board-ip './serial-thermo'
```

默认 `USE_STDIO=1`、`SIMULATE_SENSOR=1`，SSH 终端即可联调。接真 DHT 时把 `SIMULATE_SENSOR` 改为 `0`，并核对 GPIO 宏。
