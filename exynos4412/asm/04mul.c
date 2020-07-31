#include <stdio.h>

int main(void)
{
	int a = 2, b = 3, c = 4, d = 5, e = 6;
	fprintf(stdout, "a:%d b:%d c:%d, d:%d, e:%d\n", a, b, c, d, e);
	asm volatile (
		"mul %[c], %[a], %[b]\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:
	);
	fprintf(stdout, "a:%d b:%d c:%d, d:%d, e:%d\n", a, b, c, d, e);
	a = 2, b = 3, c = 4, d = 5;
	asm volatile (
		/* mla rd, rm, rs, rn--rd:=((rm * rs) +rn) */
		/* d = (c + (a * b))--d=(4 + (2 * 3))=10 */
		"mla %[d], %[a], %[b], %[c]\n"
		/* d = ((c - a * b))--d=((4 - (2 * 3))= -2)*/
		"mls %[e], %[a], %[b], %[c]\n"
		:[d]"=&r"(d), [e]"=&r"(e)
		:[a]"r"(a), [b]"r"(b), [c]"r"(c)
		:
	);
	fprintf(stdout, "a:%d b:%d c:%d, d:%d, e:%d\n", a, b, c, d, e);
	fprintf(stdout, "%d + %d * %d = %d\n", c, a, b, d);
	fprintf(stdout, "%d - %d * %d = %d\n", c, a, b, e);
	return 0;
}
