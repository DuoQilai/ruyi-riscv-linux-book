/*
 * parse.h — 切词与数值解析（实验二素材）
 * 把一行命令拆成若干词，并标出哪些词是数字。
 * ch04 的 set temp <数> 就是这种格式。
 */
#ifndef PARSE_H
#define PARSE_H

typedef struct {
	char  word[24]; /* 词内容（不含空白），最多 23 字符 */
	int   is_num;   /* 该词是否是一个干净的浮点数 */
	double val;     /* 是数字时：解析出的值 */
} Tok;

/*
 * 把 line 按空白（空格/Tab）切成最多 max 个词填进 toks。
 * 词若是「干净浮点数」（strtod 能吃掉且后面没有尾巴），置 is_num=1 并填 val。
 * 返回词数；空行返回 0；词过长截断但不崩溃。
 */
int split_tokens(const char *line, Tok *toks, int max);

#endif
