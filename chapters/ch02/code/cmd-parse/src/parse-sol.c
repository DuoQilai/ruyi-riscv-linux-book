/*
 * parse-sol.c — cmd-parse 参考实现（make sol）
 */
#include <ctype.h>
#include <stdlib.h>
#include "parse.h"

int split_tokens(const char *line, Tok *toks, int max)
{
	int n = 0;
	const char *p = line;

	if (!line)
		return 0;

	while (*p) {
		while (*p && isspace((unsigned char)*p))
			p++;
		if (!*p)
			break;
		if (n >= max)
			break;

		{
			Tok *t = &toks[n];
			int k = 0;
			while (*p && !isspace((unsigned char)*p) &&
			       k < (int)sizeof(t->word) - 1)
				t->word[k++] = *p++;
			t->word[k] = '\0';

			{
				char *end = NULL;
				double v = strtod(t->word, &end);
				t->is_num = (end != t->word) && (*end == '\0');
				t->val = t->is_num ? v : 0.0;
			}
		}
		n++;
	}
	return n;
}
