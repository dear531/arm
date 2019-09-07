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
		:
	);
	fprintf(stdout, "cpsr :%x\n", cpsr);
	return 0;
}
