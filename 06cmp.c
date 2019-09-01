#include <stdio.h>

int main(void)
{
	int a = 1, b = 2, c = 3;
	asm volatile (
		"cmp %[a], %[c]\n"
		"movgt %[c], #1\n"
		"moveq %[c], #0\n"
		"movlt %[c], #-1\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:
	);
	fprintf(stdout, "instruction cmp %d and %d result :%d\n", a, b, c);
	return 0;
}
