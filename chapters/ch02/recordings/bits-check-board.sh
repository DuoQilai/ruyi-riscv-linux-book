# 第二章实验一 · bits-check
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/bits-check
#$ expect \$
./build/bits-check-sol
#$ expect ALL PASS
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
