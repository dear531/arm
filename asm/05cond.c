#include <stdio.h>

int main(void)
{
	int a = 10, b = 20, c = 0;
	asm volatile (
	/* gt lt eq ne le ge */
	/*
	* gt greater than
	* lt lees than
	* eq equal
	* nq not equal
	* le lees than and equal
	* ge greater than and equal
	*/
	/*
	* if (a > b) Z = 0, N = 0, 不等z为零，负标志位0，为正
	* if (a = b) Z = 1 零标志位为0
	* if (a < b) Z = 0, N = 1， 零标志为0，负标志位为1
	*/
		"subs r0, %[a], %[b]\n"
		"movgt %[c], #1\n"
		"moveq %[c], #0\n"
		"movlt %[c], #-1\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:"r0"
	);
	fprintf(stdout, "a:%d, b:%d, c:%d\n", a, b, c);
	fprintf(stdout, "compare %d and %d result:%d\n", a, b, c);
	return 0;
}
