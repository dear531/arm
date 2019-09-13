#include <stdio.h>

int main(void)
{
	int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0;
	long long a = 0x8888888855555555, b = 0x8888888765555555, c = 3;
	long long tmp = 0;
	a1 = 0x00000000ffffffff & a;
	a2 = 0x00000000ffffffff & b;
	a3 = a >> 32;
	a4 = b >> 32;
	fprintf(stdout,"a1:%x,a2:%x,a3:%x,a4:%x,a5:%x,a6:%x\n",
		a1, a2, a3, a4, a5, a6);
	fprintf(stdout, "a:%llx, b:%llx, c:%llx\n", a, b, c);
	__asm__ __volatile__(
		"subs %[a6], %[a3], %[a4]\n"
		/* a5 = a1-a2-(!C):减法时标志先置1，有借位清零，没有保持 */
		"sbc %[a5], %[a1], %[a2]\n"
		:[a5]"=&r"(a5), [a6]"=&r"(a6)
		:[a1]"r"(a1), [a2]"r"(a2), [a3]"r"(a3), [a4]"r"(a4)
		:
	);
	tmp = a6;
	tmp = tmp << 32;
	c = tmp + a5;
	fprintf(stdout,"a1:%x,a2:%x,a3:%x,a4:%x,a5:%x,a6:%x\n",
		a1, a2, a3, a4, a5, a6);
	fprintf(stdout, "a:%llx, b:%llx, c:%016llx\n", a, b, c);
	fprintf(stdout, "%llx - %llx = %016llx\n", a, b, c);
	return 0;
}
