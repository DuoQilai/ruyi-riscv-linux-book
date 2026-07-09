/*
 * ch03 blink — GPIO 输出闪烁（讲义 3.1）
 * 修改 GPIO_CHIP_PATH / LED_LINE 以匹配课程接线表。
 * 依赖：libgpiod（链接 -lgpiod）
 */
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP_PATH "/dev/gpiochip0"
#define LED_LINE       17 /* TODO: 按接线表修改 */

int main(void)
{
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int value = 0;
	int i;

	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}

	line = gpiod_chip_get_line(chip, LED_LINE);
	if (!line) {
		perror("gpiod_chip_get_line");
		gpiod_chip_close(chip);
		return 1;
	}

	if (gpiod_line_request_output(line, "ch03-blink", 0) < 0) {
		perror("gpiod_line_request_output");
		gpiod_chip_close(chip);
		return 1;
	}

	printf("[INFO] blinking line %d on %s (Ctrl+C to stop)\n",
	       LED_LINE, GPIO_CHIP_PATH);

	for (i = 0; i < 20; i++) {
		value = !value;
		if (gpiod_line_set_value(line, value) < 0) {
			perror("gpiod_line_set_value");
			break;
		}
		usleep(500000);
	}

	gpiod_line_release(line);
	gpiod_chip_close(chip);
	return 0;
}
