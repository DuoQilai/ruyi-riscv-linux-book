/*
 * bits.h — 位运算与字节拼装（实验一素材）
 * 三个函数，练的都是第四章 DHT22 读到的字节怎么变成温湿度。
 */
#ifndef BITS_H
#define BITS_H
#include <stdint.h>

/* 把两个字节合成 16 位：hi 放高 8 位，lo 放低 8 位。
 * 例：dht_pack_u16(0x01, 0x8C) == 0x018C */
uint16_t dht_pack_u16(uint8_t hi, uint8_t lo);

/* 前 n 个字节求和后取低 8 位（& 0xFF）。
 * DHT22 校验和就是 4 个数据字节之和的低 8 位。 */
uint8_t u8_sum(const uint8_t *d, int n);

/* 16 位原码（最低位 = 0.1°C）换算成摄氏度。
 * 例：tenths_to_c(0x018C) == 39.6 */
float tenths_to_c(uint16_t raw);

#endif
