# 第三章实验三 · temperature_fan-sol（模拟温度；sudo；Ctrl+C 干净停）
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/ch03-lab
#$ expect \$
sudo ./temperature_fan-sol
#$ expect fan ON
#$ wait 1500
#$ sendcontrol c
#$ expect cleaned
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
