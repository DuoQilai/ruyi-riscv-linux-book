# 第六章实验三 · tri-thread（板上：无锁现 race → 加锁清零）
# 录像用：不改实验源码。预编译 lock0/lock1；Ctrl+C 触发程序自带干净退出路径。
# MQTT 强制开关/状态查询的实机转录另见 lab.html「MQTT 与并发」一节（录像工具无法在程序运行时键入）。
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/tri-thread
#$ expect \$
ls tri-thread-lock0 tri-thread-lock1
#$ expect tri-thread-lock1
#$ expect \$
#$ snapshot before-lock0
BROKER_HOST=192.168.31.206 ./tri-thread-lock0
#$ expect RACE
#$ wait 2500
#$ sendcontrol c
#$ expect race_hits
#$ expect \$
#$ snapshot lock0-race
BROKER_HOST=192.168.31.206 ./tri-thread-lock1
#$ expect USE_LOCK=1
#$ wait 2500
#$ sendcontrol c
#$ expect race_hits=0
#$ expect cleanly
#$ expect \$
#$ snapshot lock1-clean
exit
#$ expect \$
