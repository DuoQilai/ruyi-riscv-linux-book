# 第一章实验一 · probe（板端原生）
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/probe
#$ expect \$
make clean && make
#$ expect \$
file probe
#$ expect RISC-V
#$ expect \$
#$ snapshot file-riscv
./probe
#$ expect probe done
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
