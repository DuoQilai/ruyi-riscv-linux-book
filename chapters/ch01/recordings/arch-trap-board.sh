# 第一章实验二 · 架构陷阱（板上跑 x86 产物 → Exec format error）
# 前置：主机已 scp probe(x86) → /tmp/probe.x86
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/probe && make
#$ expect \$
file probe
#$ expect RISC-V
#$ expect \$
file /tmp/probe.x86
#$ expect x86-64
#$ expect \$
#$ snapshot before-trap
/tmp/probe.x86 || true
#$ expect Exec format error
#$ expect \$
./probe
#$ expect arch
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
