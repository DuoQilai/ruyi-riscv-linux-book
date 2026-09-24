# 第五章实验三 · mqtt-led（主机侧发命令 + 板端响应 + 报文抓取）
# 录像用：不改实验源码。板端需能开 gpiochip（sudo）。
# Requires: ~/mqtt-led/mqtt-led-sol ；Broker 192.168.31.206:1883
# 说明：板端未装 tcpdump（apt 不可达），用 mosquitto_sub 抓取经 Broker 的实际报文代替。
#$ expect \$
ssh -tt -o StrictHostKeyChecking=no -o PreferredAuthentications=publickey debian@192.168.31.179
#$ expect \$
#$ snapshot board-prompt
cd ~/mqtt-led
#$ expect \$
ls mqtt-led-sol
#$ expect mqtt-led-sol
#$ expect \$
timeout 30 mosquitto_sub -h 192.168.31.206 -t 'course/led/#' -v > /tmp/cap.log 2>&1 &
#$ expect \$
sudo setsid nohup env BROKER_HOST=192.168.31.206 ./mqtt-led-sol > /tmp/led.log 2>&1 < /dev/null &
#$ expect \$
#$ wait 2000
exit
#$ expect \$
cd ~/Projects/full-stack/ruyi/ruyi-riscv-linux-book
#$ expect \$
#$ snapshot host-publish
node project/course-board/mqtt.mjs led on
#$ expect \$
node project/course-board/mqtt.mjs led off
#$ expect \$
node -e "import('./project/course-board/mqtt.mjs').then(m=>m.setLed('ON')).catch(e=>console.log('err =', e.message))"
#$ expect err = invalid led state
ssh debian@192.168.31.179
#$ expect \$
#$ snapshot board-log
tail -n 8 /tmp/led.log
#$ expect published course/led/status = off
echo "--- 经过 Broker 的实际报文 ---"
cat /tmp/cap.log
#$ expect course/led/cmd
#$ wait 1000
#$ snapshot captured-packets
exit
#$ expect \$
