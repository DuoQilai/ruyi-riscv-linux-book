/*
 * parse.c — 切词与数值解析（学生版）
 * 只实现 split_tokens。对照答案：make sol && ./build/cmd-parse-sol
 */
#include <ctype.h>
#include <stdlib.h>
#include "parse.h"

/* STUDENT TODO [P1]：把 line 按空白切到 toks（最多 max 个），返回词数。
 *
 * 逐字符做法：
 *   1) 跳过空白；
 *   2) 复制到下一个空白为止，填 toks[i].word；
 *   3) 用 strtod 判断该词是否「干净浮点数」：endptr 越过至少 1 个字符、
 *      且 *endptr == '\0' → is_num=1, val=数值；否则 is_num=0。
 * 空行返回 0。词超过 23 字符截断，不越界、不崩溃。
 */
int split_tokens(const char *line, Tok *toks, int max)
{
	return 0;   /* ← 这里实现 */
}
