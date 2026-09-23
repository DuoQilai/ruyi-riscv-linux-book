#include <stdio.h>
#include <string.h>
#include "virt_led.h"

int apply_cmd(const char *line, int *led_on)
{
	if (!line || !led_on)
		return -1;
	if (strcmp(line, "on") == 0) {
		*led_on = 1;
		printf("[LED] ON\n");
		printf("[STATUS] on\n");
		fflush(stdout);
		return 0;
	}
	if (strcmp(line, "off") == 0) {
		*led_on = 0;
		printf("[LED] OFF\n");
		printf("[STATUS] off\n");
		fflush(stdout);
		return 0;
	}
	printf("[ERR] bad cmd\n");
	fflush(stdout);
	return -1;
}
