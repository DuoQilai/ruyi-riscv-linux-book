# 三处同看：主机侧发命令 / Broker 中转 / 荔枝派 tcpdump 抓包 / K3 侧驱动
#$ expect \$
cd ~/Projects/full-stack/ruyi/ruyi-riscv-linux-book
#$ expect \$
#$ snapshot host-prompt
docker exec course-mosquitto timeout 150 mosquitto_sub -h 127.0.0.1 -t 'course/#' -v > /tmp/broker-sub.log 2>&1 &
#$ expect \$
ssh debian@192.168.31.179
#$ expect \$
sudo pkill -x tcpdump; sudo setsid nohup timeout 90 tcpdump -i wlan0 -n -s0 'tcp port 1883' -w /tmp/mqtt.pcap > /tmp/tcpdump.log 2>&1 < /dev/null &
#$ expect \$
exit
#$ expect \$
#$ snapshot board-capture-start
project/recordings/demo-cycle.sh 1
#$ expect demo] 结束
#$ snapshot host-drive
echo "=== Broker 侧看到的中转报文 ==="
head -12 /tmp/broker-sub.log
#$ snapshot broker-view
ssh fengde@192.168.31.195
#$ expect \$
cd ~/course-board && BROKER_HOST=192.168.31.206 node mqtt.mjs fan auto
#$ expect fan auto
#$ snapshot k3-drive
exit
#$ expect \$
ssh debian@192.168.31.179
#$ expect \$
echo "=== 板端程序日志 ==="; grep -a -E "cmd payload=fan|fan ON|fan OFF|LED ON|LED OFF" /tmp/tri-thread.log /tmp/led.log | tail -6
#$ expect LED
sudo pkill -x tcpdump; sleep 1
#$ expect \$
echo "=== 板端 tcpdump 抓到的报文（前 8 条）==="; sudo tcpdump -r /tmp/mqtt.pcap -n 2>&1 | head -8
#$ expect 1883
#$ snapshot board-tcpdump
exit
#$ expect \$
