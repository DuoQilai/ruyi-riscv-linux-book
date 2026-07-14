# 假温控 · 改代码闯关

半成品在 `src/thermo.c`，故意埋了比较、结构体字段、指针与错误分支问题。  
默认 **本机 gcc**；也可 `make CROSS_COMPILE=riscv64-unknown-linux-gnu-`。

## 构建

```bash
make
./build/fake-thermo
printf '25\n27\n29\n27\n24\n-1\n' | ./build/fake-thermo --stdin
```

## 修好后的期望行为

配置：`t_high=28`，`t_low=26`，初始风扇 OFF。滞回：高于上限开、低于下限关、中间保持。

| 步 | 输入 | 期望 |
|----|------|------|
| 1 | 25.0 | OFF |
| 2 | 27.0 | OFF（保持） |
| 3 | 29.0 | ON |
| 4 | 27.0 | ON（保持） |
| 5 | 24.0 | OFF |
| 6 | 读失败 | 保持 OFF，并打印 `READ_FAIL` |

`thermo_decide`：空指针安全返回。`thermo_read`：失败返回非 0，且不写输出；`out_c == NULL` 时返回非 0。
