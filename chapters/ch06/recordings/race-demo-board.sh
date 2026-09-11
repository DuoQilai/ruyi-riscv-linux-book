# 第六章实验一 · race-demo（板上自动演示）
# Requires: ~/race-demo/race-demo
# 录像：race-demo 检出丢更新时 exit 1（实验设计）；|| true 只保录像 shell
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/race-demo
#$ expect \$
./race-demo || true
#$ expect RACE
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
