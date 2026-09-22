/* led_cmd.h — 第五章实验一：解析 on/off，生成 status 载荷 */
#ifndef LED_CMD_H
#define LED_CMD_H

/* 精确匹配「整段载荷」，长度由 payloadlen 给出（MQTT 载荷不一定以 '\0' 结尾）：
 *   payloadlen == 2 且 memcmp(payload, "on",  2) == 0 → 返回 1
 *   payloadlen == 3 且 memcmp(payload, "off", 3) == 0 → 返回 0
 *   其他 → 返回 -1
 * 不接受空格变体（"on "）、大小写变体（"ON"/"On"）；
 * 也拒绝内嵌空字节的伪造载荷（如 "on\0x"，长度 4）——只看长度和字节，不看 '\0'。 */
int parse_led_cmd(const char *payload, int payloadlen);

/* on!=0 → 返回 "on"；否则返回 "off"。返回的是静态字符串，勿 free。 */
const char *status_payload(int on);

#endif
