# 荔枝派：关掉终端之后滞回仍在拉风扇
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
grep -E "fan mode auto|fan ON|fan OFF|status temp" /tmp/tri-thread.log | tail -n 12
#$ expect fan
#$ snapshot hysteresis
exit
#$ expect \$
