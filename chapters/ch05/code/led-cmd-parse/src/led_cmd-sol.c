#include "led_cmd.h"
#include <string.h>

int parse_led_cmd(const char *payload, int payloadlen)
{
	if (!payload || payloadlen <= 0)
		return -1;
	if (payloadlen == 2 && memcmp(payload, "on", 2) == 0)
		return 1;
	if (payloadlen == 3 && memcmp(payload, "off", 3) == 0)
		return 0;
	return -1;
}

const char *status_payload(int on)
{
	return on ? "on" : "off";
}
