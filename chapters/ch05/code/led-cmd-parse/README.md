# led-cmd-parse · 第五章实验一

按 `payloadlen` 精确匹配 MQTT 载荷 `on` / `off`，并生成 status 字符串。无 Broker、无 GPIO。

MQTT 载荷不保证以 `'\0'` 结尾，所以签名带长度：

```c
/* payloadlen==2 且 memcmp(payload,"on",2)==0  → 1
 * payloadlen==3 且 memcmp(payload,"off",3)==0 → 0
 * 其他（含 "on\0x"、"ON"、"on "）             → -1 */
int parse_led_cmd(const char *payload, int payloadlen);
```

```bash
make && ./build/led-cmd-parse          # 未实现 → SOME FAIL
make sol && ./build/led-cmd-parse-sol  # 参考 → ALL PASS
```

测试驱动里带 `on\0x`（长度 4）这类伪造载荷，`strcmp` 写法会漏过，必须用 `memcmp` 比长度。
