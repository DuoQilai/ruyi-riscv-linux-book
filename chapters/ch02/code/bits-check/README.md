# bits-check — 位运算与字节拼装（第二章实验一）

练习把 DHT22 式字节拼成数值：

- `dht_pack_u16(hi, lo)`：两个字节合成 16 位
- `u8_sum(d, n)`：求和取低 8 位（校验和）
- `tenths_to_c(raw)`：0.1°C 单位换算

## 学生路径

```
make && ./build/bits-check     # 未写 TODO 前会 FAIL
```

只改 `src/bits.c`。

## 对照

```
make sol && ./build/bits-check-sol    # 应 ALL PASS
```

本机或板端（`uname -m` = riscv64）的 gcc 均可；Makefile 会自动用当前机器编译器。
