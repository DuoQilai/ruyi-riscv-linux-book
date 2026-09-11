# 第六章实验二 · snapshot-lock（板上自动演示）
# Requires: ~/snapshot-lock 源码（勿同步主机 x86 build/）
# 脚手架输出很长：录像用 head 截前几行 [RACE]，再跑 sol 看 race_hits=0
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
(./build/snapshot-lock 2>&1 | head -15) || true
#$ expect RACE
#$ expect \$
#$ snapshot scaffold-race
exit
#$ expect \$
