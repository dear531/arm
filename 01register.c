#include <stdio.h>

int main(void)
{
	int a = 0, b = 1, c = 2, d = 3;
	fprintf(stdout, "a:%x, b:%x, c:%x, d:%x\n",
		a, b, c, d);
	asm volatile (
		"mov %[a], #0xff\n"
		"mov %1, %[c]\n" /* %1--变量当中的第二个，从零开始 */
		"mov r1, #0xfe << 16\n"	/* 立即数的范围为小于等于255的偶数倍，偶数倍不大于32，由立即数的高4位决定，只能存储下不小于16的数 */
		"mov %[c], r1\n" 	/* [c]--标记变量中的符号c变量 */
		"mov %[a], %[d]\n"
		/* 声明输入输出变量 */
		/* =--表示输出变量，&--表示独占一个寄存器，r--register，寄存器 */
		/* +--输出输出变量 */
		:[a]"=&r"(a),"=&r"(b),[c]"+&r"(c)
		/* 声明输出变量 */
		:[d]"r"(d)
		/* 声明寄存器保护 */
		:"r1" /* 寄存器用到哪个就写上哪个，用堆栈保护寄存器 */
	);
	fprintf(stdout, "a:%x, b:%x, c:%x, d:%x\n",
		a, b, c, d);
	return 0;
}
