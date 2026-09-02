/*
 * shell.h — 温控命令 REPL（实验二素材）
 * 纯 stdin/stdout，没有传感器、没有 GPIO。练的是命令处理器本身：
 * status 与 set temp/set humidity，这正是实验三 cmd_thermo 里要写的处理器的一半。
 */
#ifndef SHELL_H
#define SHELL_H

typedef struct {
	float t_bar;  /* °C：温度 bar */
	float h_bar;  /* %：湿度 bar */
} St;

/* 初始：t_bar=30.0，h_bar=95.0 */
void state_init(St *s);

/* 打印当前 bar。返回 0。 */
int cmd_status(const St *s);

/* 解析 args（形如 "temp 28.5" / "humidity 60"）并更新 bar。
 * 成功返回 0；格式错返回非 0（打一行 [err] 用法）。 */
int cmd_set(St *s, const char *args);

#endif
