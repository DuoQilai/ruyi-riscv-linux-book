#include "led_cmd.h"
#include <string.h>

int parse_led_cmd(const char *payload)
{
	if (!payload)
		return -1;
	if (strcmp(payload, "on") == 0)
		return 1;
	if (strcmp(payload, "off") == 0)
		return 0;
	return -1;
}

const char *status_payload(int on)
{
	return on ? "on" : "off";
}
