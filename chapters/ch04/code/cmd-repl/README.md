# cmd-repl — 温控命令 REPL（第四章实验二）

纯 stdin/stdout，无传感器、无 GPIO。练 status / set 处理器——这是实验三 `cmd_thermo` 里要写的处理器的一半。

## 学生路径

```
make && ./build/cmd-repl
printf 'status\nset temp 28.5\nstatus\nquit\n' | ./build/cmd-repl
```

只改 `src/handlers.c` 的 `state_init` / `cmd_status` / `cmd_set`。

## 对照

```
make sol && ./build/cmd-repl-sol
```

本机或板端（riscv64 gcc）均可。
