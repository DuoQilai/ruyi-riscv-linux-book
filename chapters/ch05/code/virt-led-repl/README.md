# virt-led-repl · 第五章实验二

用 stdin 模拟 MQTT 下行：`on`/`off` 改虚拟灯并打印 `[STATUS]`。无 Broker、无 GPIO。

```bash
make
printf 'on\noff\nbad\nquit\n' | ./build/virt-led-repl
make sol && printf 'on\noff\nquit\n' | ./build/virt-led-repl-sol
```
