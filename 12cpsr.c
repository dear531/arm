#include <stdio.h>

int main(void)
{
	int cpsr = 0;
	fprintf(stdout, "cpsr :%x\n", cpsr);
	asm volatile (
		"mov r0, #0\n"
		"adds r0, r0, #0\n"
		/*C=0,N=0,Z=1,V=0,Q=0*/
		/*
		*M[4:0] 10000 user mode
		*NZCVQ EAIFT
		*01000 00000
		*/
		/* move CPSR to register */
		"mrs %[cpsr], cpsr\n"
		:[cpsr]"+&r"(cpsr)
		:
		:"r0"
	);
	fprintf(stdout, "cpsr :%x\n", cpsr);
	/* 尝试写cpsr */
	asm volatile(
		"mrs %[cpsr], cpsr\n"
		"orr %[cpsr], %[cpsr], #(1 << 7)\n"
		"msr cpsr, %[cpsr]\n"
		:[cpsr]"+&r"(cpsr)
		:
		:
	);
	/* 再次读cpsr看其中的内容 */
	asm volatile (
		"mov r0, #0\n"
		"adds r0, r0, #0\n"
		"mrs %[cpsr], cpsr\n"
		:[cpsr]"+&r"(cpsr)
		:
		:"r0"
	);
	fprintf(stdout, "cpsr :%x\n", cpsr);
	return 0;
}
