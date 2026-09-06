# 第四章实验二 · cmd-repl（板上自动演示）
# Requires: ~/cmd-repl/build/cmd-repl-sol
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/cmd-repl
#$ expect \$
ls build/cmd-repl-sol
#$ expect cmd-repl-sol
#$ expect \$
#$ snapshot before-demo
printf 'status\nset temp 28.5\nstatus\nset humidity 60\nstatus\nset temp abc\nquit\n' | ./build/cmd-repl-sol
#$ expect bye
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
