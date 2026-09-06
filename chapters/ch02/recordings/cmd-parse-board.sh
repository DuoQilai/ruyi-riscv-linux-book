# 第二章实验二 · cmd-parse
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/cmd-parse
#$ expect \$
./build/cmd-parse-sol
#$ expect ALL PASS
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
