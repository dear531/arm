#include <stdio.h>

int main(void)
{
	int a = 2, b = 3, c = 4, d = 5;
	fprintf(stdout, "a:%d b:%d c:%d, d:%d\n", a, b, c, d);
	asm volatile (
		"mul %[c], %[a], %[b]\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:
	);
	fprintf(stdout, "a:%d b:%d c:%d, d:%d\n", a, b, c, d);
	a = 2, b = 3, c = 4, d = 5;
	asm volatile (
		/* mla rd, rm, rs, rn--rd:=((rm * rs) +rn) */
		/* d = ((a * b) + c)--d=((2 * 3) + 4)=10 */
		"mla %[d], %[a], %[b], %[c]\n"
		:[d]"=&r"(d)
		:[a]"r"(a), [b]"r"(b), [c]"r"(c)
		:
	);
	fprintf(stdout, "a:%d b:%d c:%d, d:%d\n", a, b, c, d);
	return 0;
}
