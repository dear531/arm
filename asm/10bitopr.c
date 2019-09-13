#include <stdio.h>

int main(void)
{
	int a = 0xff, b = 0x00, c = 0xff, d = 0xff, e = 0xff;
	fprintf(stdout, "a:%x, b:%x, c:%x, d:%x, e:%x\n", a, b, c, d, e);
	asm volatile (
		/* a = a &(~(1 << 3)) */
		"and %[a], %[a], #(1 << 3)\n"
		/* b = b | (1 << 3) */
		"orr %[b], %[b], #(1 << 3)\n"
		/* c = c ^ (1 << 3 | 1) */
		"eor %[c], %[c], #(1 << 3 | 1)\n"
		/* d = d & (~(1 << 3)) : 清除位 */
		"bic %[d], %[d], #(1 << 3)\n"
		/* beflow two equal above one */
		"mvn r0, #(1 << 3)\n"
		"and %[e], %[e], r0\n"
		:[a]"+&r"(a), [b]"+&r"(b), [c]"+&r"(c), [d]"+&r"(d), [e]"+&r"(e)
		:
		:"r0"
	);
	fprintf(stdout, "a:%x, b:%x, c:%x, d:%x, e:%x\n", a, b, c, d, e);
	return 0;
}
