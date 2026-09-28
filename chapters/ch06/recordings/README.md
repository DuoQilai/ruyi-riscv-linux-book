# asciinema expect 录像（DuoQilai fork）

复用老师样例：<https://github.com/DuoQilai/asciinema/blob/develop/examples/01-ebc7700.sh>

```bash
cargo install --locked --git https://github.com/DuoQilai/asciinema
# 荔枝派远程：脚本第一命令用 ssh（不要只用串口 tio）
asciinema expect -o <name> <name>.sh
```

约定：
- 脚本放在 `chapters/chXX/recordings/*.sh`
- 产物目录 `recordings/<name>/{.cast,.gif,.mp4,snapshots/*.png}`
- 嵌进讲义的可复用资产拷到 `chapters/chXX/assets/recordings/`（GIF + 关键 PNG）
- **只改录像脚本与临时板端二进制，不改实验契约 / 学生脚手架源码**
- 非零退出（如 race-demo、timeout）在录像命令后加 `|| true`；需要干净退出时用 `#$ sendcontrol c`
