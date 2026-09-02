/*
 * bits-sol.c — bits-check 参考实现（make sol）
 */
#include <stdint.h>
#include "bits.h"

uint16_t dht_pack_u16(uint8_t hi, uint8_t lo)
{
	return (uint16_t)(((uint16_t)hi << 8) | lo);
}

uint8_t u8_sum(const uint8_t *d, int n)
{
	uint8_t s = 0;
	int i;
	for (i = 0; i < n; i++)
		s = (uint8_t)(s + d[i]);
	return s;
}

float tenths_to_c(uint16_t raw)
{
	return (float)raw / 10.0f;
}
