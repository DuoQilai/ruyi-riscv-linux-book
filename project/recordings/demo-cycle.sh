#!/bin/bash
# 实物录像驱动（同时版）：风扇与 LED 同开同关
# 用法: ./demo-cycle.sh [轮数]  0/不填=无限循环，Ctrl+C 停
B=course-mosquitto
pub() { docker exec "$B" mosquitto_pub -h 127.0.0.1 -t "$1" -m "$2"; }
FAN=course/thermo/cmd; LED=course/led/cmd
N=${1:-0}; i=0
echo "[demo] 同时模式：风扇 + LED 同开(8s) → 同关(8s) → 回 auto(4s)"
while :; do
  i=$((i+1))
  echo "===== 第 $i 轮：风扇 ON + LED ON ====="
  pub $FAN "fan on";  pub $LED "on";  sleep 8
  echo "===== 第 $i 轮：风扇 OFF + LED OFF ====="
  pub $FAN "fan off"; pub $LED "off"; sleep 8
  echo "===== 第 $i 轮：恢复 fan auto ====="
  pub $FAN "fan auto"; sleep 4
  [ "$N" != "0" ] && [ "$i" -ge "$N" ] && break
done
echo "[demo] 结束"
