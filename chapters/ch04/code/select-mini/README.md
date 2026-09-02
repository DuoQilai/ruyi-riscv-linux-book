# select-mini — select 最小多路复用（第四章实验一）

用 `select` 同时等两件事：stdin 输入 + 1 秒 tick。每行输出 `[tick N]` 或 `echo: …`。

## 学生路径

```
make && ./build/select-mini
# 手动敲行（quit 退出）；或自动演示：
(sleep 3; echo hello; sleep 1; echo quit) | ./build/select-mini
```

只改 `src/select.c` 的 `run_repl_loop`。

## 对照

```
make sol && ./build/select-mini-sol
```

本机或板端（riscv64 gcc）均可。
