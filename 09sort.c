#include <stdio.h>

int main(void)
{
	int a = 1, b = 2;
	fprintf(stdout, "a:%d, b:%d\n", a, b);
	asm volatile (
		/* 排序 */
		"mov r0, %[a]\n"
		"mov r1, %[b]\n"
		"cmp %[a], %[b]\n"
		"movlt %[a], r1\n"
		"movlt %[b], r0\n"
		:[a]"+&r"(a) ,[b]"+&r"(b)
		:
		:"r0", "r1"
	);
	fprintf(stdout, "a:%d, b:%d\n", a, b);
	return 0;
}
