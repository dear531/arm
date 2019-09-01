#include <stdio.h>

int main(void)
{
	int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 =0, sum6 = 0;
	long long lsum1 = 0x8765432100000002, lsum2 = 0x12345678ffffffff;
	long long lsum = 0;
	sum1 = lsum1 & 0x00000000ffffffff;
	sum2 = lsum2 & 0x00000000ffffffff;
	sum3 = lsum1 >> 32;
	sum4 = lsum2 >> 32;
	fprintf(stdout, "sum1:%x, sum2:%x, sum3:%x, sum4:%x, sum5:%x, sum6:%x\n", 
	sum1, sum2, sum3, sum4, sum5, sum6);
	fprintf(stdout, "lsum1:%llx, lsum2:%llx, lsum:%llx\n",
		lsum1, lsum2, lsum);
	__asm__ __volatile__ (
		"ldr r0, =0xffffffff\n" /* 伪指令，编译器会将此指令转成两到三条arm汇编指令,属于编译器的属性 */
		"adds %[sum5], %[sum1], %[sum2]\n" /* 影响进位标志位的加法 */
		"adc %[sum6], %[sum3], %[sum4]"	/* 取进位标志位的加法：sum6=sum3 + sum4 + C */
		:[sum5]"=&r"(sum5), [sum6]"=&r"(sum6)
		:[sum1]"r"(sum1), [sum2]"r"(sum2), [sum3]"r"(sum3), [sum4]"r"(sum4)
		:
	);
	lsum1 = sum6;
	lsum1 = lsum1 << 32;
	lsum = lsum1 + sum5;
	fprintf(stdout, "lsum1:%llx\n", lsum1);
	fprintf(stdout, "sum1:%x, sum2:%x, sum3:%x, sum4:%x, sum5:%x, sum6:%x\n", 
	sum1, sum2, sum3, sum4, sum5, sum6);
	fprintf(stdout, "lsum1:%llx, lsum2:%llx, lsum:%llx\n",
		lsum1, lsum2, lsum);
	return 0;
}
