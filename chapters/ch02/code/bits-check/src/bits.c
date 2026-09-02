/*
 * bits.c — 位运算与字节拼装（学生版）
 * 只改本文件里标了 STUDENT TODO 的三个函数。
 * 对照答案：make sol && ./build/bits-check-sol（应同样 ALL PASS）
 */
#include <stdint.h>
#include "bits.h"

/* STUDENT TODO [B1]：hi 放高 8 位、lo 放低 8 位合成 uint16。 */
uint16_t dht_pack_u16(uint8_t hi, uint8_t lo)
{
	return 0;   /* ← 这里实现 */
}

/* STUDENT TODO [B2]：前 n 字节求和后 & 0xFF。 */
uint8_t u8_sum(const uint8_t *d, int n)
{
	return 0;   /* ← 这里实现 */
}

/* STUDENT TODO [B3]：raw 每 1 个单位 = 0.1°C，换算成 float 摄氏度。 */
float tenths_to_c(uint16_t raw)
{
	return 0.0f;   /* ← 这里实现 */
}
