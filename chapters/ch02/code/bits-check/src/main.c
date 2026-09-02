/*
 * main.c — bits-check 测试驱动（实验一）
 * make && ./build/bits-check；全绿打印 ALL PASS。
 * 别改本文件；只实现 src/bits.c 里标了 STUDENT TODO 的函数。
 */
#include <stdio.h>
#include <stdint.h>
#include "bits.h"

static int fails;

static void ok(int cond, const char *msg)
{
	if (cond)
		printf("  ok   %s\n", msg);
	else {
		printf("  FAIL %s\n", msg);
		fails++;
	}
}

int main(void)
{
	uint8_t d1[4] = { 0x37, 0x12, 0x01, 0x8C };
	uint8_t d2[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

	printf("== dht_pack_u16 ==\n");
	ok(dht_pack_u16(0x01, 0x8C) == 0x018C, "pack(0x01,0x8C) == 0x018C");
	ok(dht_pack_u16(0x37, 0x12) == 0x3712, "pack(0x37,0x12) == 0x3712");
	ok(dht_pack_u16(0x80, 0x00) == 0x8000, "高位置 1 保持：pack(0x80,0x00) == 0x8000");

	printf("== u8_sum（DHT 数据字节求和取低 8 位）==\n");
	ok(u8_sum(d1, 4) == 0x37 + 0x12 + 0x01 + 0x8C, "sum(d1,4) 与手算和一致");
	ok(u8_sum(d2, 4) == 0xFC, "0xFF*4=0x3FC 溢出保留低 8 位 0xFC");

	printf("== tenths_to_c ==\n");
	ok(tenths_to_c(0x018C) == 39.6f, "0x018C == 396 个 0.1°C == 39.6");
	ok(tenths_to_c(250) == 25.0f, "250 个 0.1°C == 25.0");
	ok(tenths_to_c(0) == 0.0f, "0 == 0.0");

	if (fails) {
		printf("\nSOME FAIL (%d)\n", fails);
		return 1;
	}
	printf("\nALL PASS\n");
	return 0;
}
