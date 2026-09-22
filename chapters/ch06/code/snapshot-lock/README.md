# snapshot-lock · 第六章实验二

纯软件：两线程写/读 `temp_a`/`temp_b` 成对快照。先跑脚手架看 `[RACE]`，再 `make sol` 对照加锁版。

```bash
make && ./build/snapshot-lock          # 无锁 → race_hits 很多
make sol && ./build/snapshot-lock-sol  # 加锁 → race_hits=0
```

实验三 `tri-thread` 把同一套成对写入接到真采集/控制/MQTT。不要把无锁空窗抄进产品验收路径。
