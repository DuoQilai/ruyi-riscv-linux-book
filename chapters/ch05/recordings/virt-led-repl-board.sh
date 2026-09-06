# 第五章实验二 · virt-led-repl（板上自动演示）
# Requires: ~/virt-led-repl/build/virt-led-repl-sol
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/virt-led-repl
#$ expect \$
printf 'on\noff\nbad\nquit\n' | ./build/virt-led-repl-sol
#$ expect bye
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
