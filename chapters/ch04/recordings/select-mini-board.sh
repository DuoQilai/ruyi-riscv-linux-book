# Board recording scenarios for asciinema expect (DuoQilai fork)
#
# Host: cargo install --locked --git https://github.com/DuoQilai/asciinema
# Board must be reachable via: ssh debian@192.168.31.179 (key auth)
#
# Run from this directory:
#   asciinema expect select-mini-board.sh
# Outputs: select-mini-board/{.cast,.gif,.mp4,snapshots/*.png}

# 第四章实验一 · select-mini（板上自动演示）
# Requires: ~/select-mini/build/select-mini-sol on the board
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/select-mini
#$ expect \$
ls build/select-mini-sol
#$ expect select-mini-sol
#$ expect \$
#$ snapshot before-demo
(sleep 2; echo hello; sleep 1; echo quit) | ./build/select-mini-sol
#$ expect bye
#$ expect \$
#$ snapshot after-demo
exit
#$ expect \$
