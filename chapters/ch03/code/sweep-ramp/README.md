# sweep-ramp — 温度扫描边界（第三章实验一）

把「24 → 30 → 24」来回扫的边界逻辑抽出来单练：到上限前反向、到下限前反向，不许越界。

- `ramp_next(t, dir, lo, hi, *next_dir)`：走一步，返回新温度并回写方向。

## 学生路径

```
make && ./build/sweep-ramp     # 未实现前会 FAIL（温度卡在 24，到不了 30）
```

只改 `src/ramp.c`。

## 对照

```
make sol && ./build/sweep-ramp-sol    # 应 ALL PASS
```

这段逻辑曾因越界（30.5/23.5）被审阅指出过；本实验用断言直接卡住它。
本机或板端（riscv64 gcc）均可。
