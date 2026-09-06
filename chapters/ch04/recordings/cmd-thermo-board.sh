# 第四章实验三 · cmd_thermo-sol（SIMULATE；sudo；status/set；Ctrl+C）
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/ch04-lab
#$ expect \$
sudo ./cmd_thermo-sol
#$ expect SIMULATE
#$ wait 2500
status
#$ expect STATUS
#$ snapshot status
set temp 20
#$ expect 温度 bar
#$ wait 2500
#$ snapshot after-set
#$ sendcontrol c
#$ expect 已清理
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
