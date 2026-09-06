# 第三章实验二 · fan-timeline
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/fan-timeline
#$ expect \$
./build/fan-timeline-sol
#$ expect SUMMARY
#$ expect turn_ons=9
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
