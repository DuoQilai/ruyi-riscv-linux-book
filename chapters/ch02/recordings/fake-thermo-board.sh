# 第二章实验三 · fake-thermostat
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/fake-thermostat
#$ expect \$
./build/fake-thermo-sol
#$ expect READ_FAIL
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
