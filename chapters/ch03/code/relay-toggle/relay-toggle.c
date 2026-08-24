/* relay-toggle.c — 完整演示 gpiod 生命周期（libgpiod v2）
 * 打开 chip → 申请 line 5 为输出(初值 0) → 拉高 → 拉低 → 释放
 */
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    /* 1. 打开 chip：/dev/gpiochip5 对应 IO1_5 所在控制器 */
    struct gpiod_chip *chip = gpiod_chip_open("/dev/gpiochip5");

    /* 2. 配置 line 5：方向=输出，初值=低（不驱动 = 悬空 = 风扇乱转） */
    struct gpiod_line_settings *st = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(st, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(st, GPIOD_LINE_VALUE_INACTIVE);

    struct gpiod_line_config *cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg, (unsigned int[]){5}, 1, st);

    /* 3. 申请（consumer 名字 "relay-toggle"，gpioinfo 里能认出谁占着） */
    struct gpiod_request_config *rcfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(rcfg, "relay-toggle");
    struct gpiod_line_request *req = gpiod_chip_request_lines(chip, rcfg, cfg);

    printf("已申请 line 5，初值低 → 风扇确定关\n");
    sleep(1);

    /* 4. 拉高：继电器吸合，风扇开 */
    gpiod_line_request_set_value(req, 5, GPIOD_LINE_VALUE_ACTIVE);
    printf("line 5 = 高 → 继电器吸合，风扇开\n");
    sleep(2);

    /* 5. 拉低：继电器释放，风扇关 */
    gpiod_line_request_set_value(req, 5, GPIOD_LINE_VALUE_INACTIVE);
    printf("line 5 = 低 → 继电器释放，风扇关\n");

    /* 6. 释放：把 line 还给内核（再没人驱动就悬空） */
    gpiod_line_request_release(req);
    printf("已释放 line 5\n");
    return 0;
}
