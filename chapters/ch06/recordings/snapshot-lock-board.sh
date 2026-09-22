# 第六章实验二 · snapshot-lock（板上自动演示）
# Requires: ~/snapshot-lock 源码（勿同步主机 x86 build/）
# 无锁脚手架会自行跑完并打印最终 race_hits，本录像完整保留输出、不做 head 截断。
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/snapshot-lock
#$ expect \$
make clean && make && make sol
#$ expect snapshot-lock-sol
#$ expect \$
./build/snapshot-lock-sol
#$ expect race_hits=0
#$ expect \$
#$ snapshot sol-clean
./build/snapshot-lock || true
#$ expect race_hits=
#$ expect \$
#$ snapshot scaffold-race
exit
#$ expect \$
