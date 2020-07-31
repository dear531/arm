#include <stdio.h>

int main(void)
{
	int a = 0, b = 1, c = 2;
	fprintf(stdout, "a:%d, b:%d, c:%d\n", a, b, c);
	asm volatile (
		"teq %[a], %[b]\n"
		"moveq %[c], #1\n"
		"movne %[c], #0\n"
		:[c]"=&r"(c)
		:[a]"r"(a),[b]"r"(b)
		:
	);
	fprintf(stdout, "a:%d, b:%d, c:%d\n", a, b, c);
	fprintf(stdout, "teq %d and %d result %d\n", a, b, c);
	return 0;
}
