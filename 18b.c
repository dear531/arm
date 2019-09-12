#include <stdio.h>

int main(void)
{
	/*
	 * APCS
	 * 1.参数传递，前四个r0-r3，从第五个开始按顺序入栈
	 * 越靠后的越先入栈
	 * 2.满递减栈 fd
	 * 3.返回值，32位r0，64位放r0r1，r0低32位，r1高32位
	 */
	int a = 1, b = 2, c = 0;
	fprintf(stdout, "asm add1 before a:%d, b:%d, c:%d\n",
		a, b, c);
	__asm__ __volatile__(
		"mov r0, %[a]\n"
		"mov r1, %[b]\n"
		/* bl是跳转之前将下条指令的地址压栈 */
		"bl add1\n"
		"mov %[c], r0\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:
	);
	fprintf(stdout, "asm add1 after a:%d, b:%d, c:%d\n",
		a, b, c);
	c = 0;
	fprintf(stdout, "asm add2 before a:%d, b:%d, c:%d\n",
		a, b, c);
	__asm__ __volatile__(
		"mov r0, %[a]\n"
		"mov r1, %[b]\n"
		/* bl是跳转之前将下条指令的地址压栈 */
		"bl add2\n"
		"mov %[c], r0\n"
		:[c]"=&r"(c)
		:[a]"r"(a), [b]"r"(b)
		:
	);
	fprintf(stdout, "asm add2 after a:%d, b:%d, c:%d\n",
		a, b, c);
	return 0;
}
int add2(int a, int b)
{
	return a + b;
}

__asm__(
	"add1:add r0, r0, r1\n"
	/* bx是以寄存器为地址进行跳转 */
	"bx lr\n"
);
