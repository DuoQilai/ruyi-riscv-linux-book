# cmd-demo — 第四章先分再合示例：读一行 → 切词 → 查表 → 派发
# 纯标准输入输出，不碰 GPIO；本机 gcc 即可编译运行
cmd-demo: cmd-demo.c
	gcc -Wall -Wextra -O2 cmd-demo.c -o $@

clean:
	rm -f cmd-demo

.PHONY: clean
