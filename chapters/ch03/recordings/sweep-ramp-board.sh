# 第三章实验一 · sweep-ramp
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/sweep-ramp
#$ expect \$
./build/sweep-ramp-sol
#$ expect ALL PASS
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
