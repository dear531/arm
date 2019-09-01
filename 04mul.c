#include <stdio.h>

int main(void)
{
	int a = 2, b = 3, c = 4;
	fprintf(stdout, "a:%d b:%d c:%d\n", a, b, c);
	asm volatile (
		"mul %[c], %[a], %[b]\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:
	);
	fprintf(stdout, "a:%d b:%d c:%d\n", a, b, c);
	return 0;
}
