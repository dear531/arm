#include <stdio.h>

int main(void)
{
	int a = 1, b = 2, c = 3;
	fprintf(stdout, "a:%d, b:%d, c:%d\n", a, b, c);
	asm volatile (
		/* 排序 */
		"cmp %[a], %[b]\n"
		"movlt r0, %[a]\n"
		"movlt %[a], %[b]\n"
		"movlt %[b], r0\n"

		"cmp %[a], %[c]\n"
		"movlt r0, %[a]\n"
		"movlt %[a], %[c]\n"
		"movlt %[c], r0\n"

		"cmp %[b], %[c]\n"
		"movlt r0, %[b]\n"
		"movlt %[b], %[c]\n"
		"movlt %[c], r0\n"

		:[a]"+&r"(a) ,[b]"+&r"(b), [c]"+&r"(c)
		:
		:"r0"
	);
	fprintf(stdout, "a:%d, b:%d, c:%d\n", a, b, c);
	return 0;
}
