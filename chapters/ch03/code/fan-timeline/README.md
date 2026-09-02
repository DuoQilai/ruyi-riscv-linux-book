# fan-timeline — 决策 + 时间线统计（第三章实验二）

模拟温度 24→30→24 扫 240 步，每步做滞回决策，打印 `t=… fan=ON/OFF` 时间线并统计：

- `SUMMARY steps=240 on_steps=… turn_ons=…`

`fan_decide` 签名与实验三 `temperature_fan` 完全一致——写好后可直接平移到实验三（把执行换成真 GPIO）。

## 学生路径

```
make && ./build/fan-timeline
```

只改 `src/decide.c`。决策写错（如没做滞回直接按温度开关），SUMMARY 三数会与参考不同。

## 对照

```
make sol && ./build/fan-timeline-sol
```

本机或板端（riscv64 gcc）均可。
