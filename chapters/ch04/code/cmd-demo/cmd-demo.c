/* cmd-demo.c — 先分再合：读一行 → 切词 → 查表 → 派发
 * 用 help / echo / quit 三个无关命令演示命令表的骨架
 * 不是实验里的 status / set——那部分由你在实验里自己写 */
#include <stdio.h>
#include <string.h>

/* 分①：处理函数——每个命令做一件事 */
static int cmd_help(char *args) { (void)args; printf("commands: help echo quit\n"); return 0; }
static int cmd_echo(char *args) { printf("%s\n", args); return 0; }
static int cmd_quit(char *args) { (void)args; return 1; }

/* 分②：命令表——名字 → 函数指针 */
struct cmd_entry { const char *name; int (*handler)(char *); };
static const struct cmd_entry cmds[] = {
    { "help", cmd_help },
    { "echo", cmd_echo },
    { "quit", cmd_quit },
    { NULL, NULL },
};

/* 分③：查表派发——名字对上就调用，对不上报 unknown */
static int dispatch(char *line)
{
    char *args = strchr(line, ' ');   /* 切词：第一个空格后是参数 */
    if (args) { *args = '\0'; args++; }
    for (const struct cmd_entry *e = cmds; e->name; e++)
        if (strcmp(line, e->name) == 0)
            return e->handler(args);
    printf("unknown command: %s\n", line);
    return 0;
}

/* 合：读一行 → 派发 → 直到 quit */
int main(void)
{
    char line[128];
    printf("cmd-demo 开始，输入 help 试试\n");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';   /* 去掉末尾换行 */
        if (dispatch(line)) break;
    }
    printf("bye\n");
    return 0;
}
