# cmd-parse — 切词与数值解析（第二章实验二）

实现 `split_tokens`：把一行命令按空白切词，并用 `strtod` 标出「干净浮点数」。

样例（`ch04` 的 `set temp <数>` 就是这种格式）：

```
set temp 28.5      → 3 词，第三个是数字 28.5
status             → 1 词
set humidity 60    → 3 词，60 是数字
set temp abc       → 3 词，abc 不是数字
```

## 学生路径

```
make && ./build/cmd-parse
```

只改 `src/parse.c`。

## 对照

```
make sol && ./build/cmd-parse-sol    # 应 ALL PASS
```

本机或板端（`uname -m` = riscv64）的 gcc 均可。
