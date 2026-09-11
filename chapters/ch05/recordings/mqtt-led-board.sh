# 第五章实验三 · mqtt-led（板上 SSH + 本机 Broker）
# 录像用：不改实验源码。板端需能开 gpiochip（gpio 组或既有权限）。
# Requires: ~/mqtt-led/mqtt-led-sol ; Broker at 192.168.31.206:1883
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/mqtt-led
#$ expect \$
ls mqtt-led-sol
#$ expect mqtt-led-sol
#$ expect \$
#$ snapshot before-demo
BROKER_HOST=192.168.31.206 timeout 22 ./mqtt-led-sol &
#$ expect subscribed
#$ wait 1000
mosquitto_pub -h 192.168.31.206 -t course/led/cmd -m on
#$ expect published course/led/status = on
#$ snapshot led-on
mosquitto_pub -h 192.168.31.206 -t course/led/cmd -m off
#$ expect published course/led/status = off
#$ snapshot led-off
mosquitto_pub -h 192.168.31.206 -t course/led/cmd -m ON
#$ expect unknown payload
#$ wait 1200
#$ snapshot after-demo
exit
#$ expect \$
