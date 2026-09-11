# 第五章实验一 · led-cmd-parse（板上自动演示）
# Requires: ~/led-cmd-parse/build/led-cmd-parse-sol
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/led-cmd-parse
#$ expect \$
./build/led-cmd-parse-sol
#$ expect ALL PASS
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
