#include <stdio.h>

int main(void)
{
	int i = 0;
	fprintf(stdout, "i:%d\n", i);
	__asm__ __volatile__(
		"mov r0, %[i]\n"
		/* bl是跳转之前将下条指令的地址压栈 */
		"bl add\n"
		"mov %[i], r0\n"
		:[i]"+&r"(i)
		:
		:
	);
	fprintf(stdout, "i:%d\n", i);
	return 0;
}

__asm__(
	"add:add r0, r0, #1\n"
	/* bx是以寄存器为地址进行跳转 */
	"bx lr\n"
);
