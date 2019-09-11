#include <stdio.h>

int main(void)
{
	int i = 0;
	asm volatile (
		/* 比较i值和5的大小 */
		"LOOP:cmp %[i], #5\n"
		/* 如果大于5，则跳转到END */
		"bgt END\n"
		/* i++ */
		"add %[i], %[i], #1\n"
		/* 跳转到LOOP */
		"b LOOP\n"
		"END:\n"
		:[i]"+&r"(i)
		:
		:
	);
	fprintf(stdout, "i:%d\n", i);
	return 0;
}
