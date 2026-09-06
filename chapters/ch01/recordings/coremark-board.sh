# 第一章实验三 · CoreMark（板端原生跑分证据；交叉路径讲义另述）
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/coremark
#$ expect \$
file ./coremark.exe
#$ expect RISC-V
#$ expect \$
#$ snapshot before-run
./coremark.exe
#$ expect Correct operation validated
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
