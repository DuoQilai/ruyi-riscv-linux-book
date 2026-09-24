# 主机侧驱动实物风扇 + LED（本机 broker → 荔枝派继电器/LED）
#$ expect \$
cd ~/Projects/full-stack/ruyi/ruyi-riscv-linux-book
#$ expect \$
#$ snapshot host-prompt
project/recordings/demo-cycle.sh 1
#$ expect demo] 结束
#$ snapshot host-drive
ssh debian@192.168.31.179
#$ expect \$
grep -a -E "cmd payload=fan|fan ON|fan OFF|mode=" /tmp/tri-thread.log | tail -n 6
#$ expect fan
#$ snapshot board-fan
grep -a -E "msg topic|LED" /tmp/led.log | tail -n 4
#$ expect LED
#$ snapshot board-led
exit
#$ expect \$
