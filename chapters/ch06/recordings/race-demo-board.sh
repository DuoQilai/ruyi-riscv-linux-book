# 第六章实验一 · race-demo（板上：丢更新 + TSan 报告）
# Requires: ~/race-demo/{main.c,Makefile}（Makefile 带 make tsan）
# race-demo 检出丢更新时 exit 1（实验设计）；|| true 只保录像 shell
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/race-demo
#$ expect \$
make clean && make
#$ expect \$
./race-demo || true
#$ expect RACE
#$ expect \$
#$ snapshot after-demo
make clean && make tsan
#$ expect \$
./race-demo 2>&1 | head -24 || true
#$ expect ThreadSanitizer: data race
#$ expect \$
#$ snapshot tsan-report
exit
#$ expect \$
