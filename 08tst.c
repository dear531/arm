#include <stdio.h>

int main(void)
{
	int a = 0xa0, b = 1;
	fprintf(stdout, "a:%x, b:%x\n", a, b);
	asm volatile (
		"tst %[a], #(0x03 << 4)\n"
		"moveq %[b], #1\n"
		"movne %[b], #0\n"
		:[b]"=&r"(b)
		:[a]"r"(a)
		:
	);
	fprintf(stdout, "a:%x, b:%x\n", a, b);
	return 0;
}
