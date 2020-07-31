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
	int a = 1, b = 2, c = 0, d = 0, e = 0, f = 0, g = 0;
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
	a = 1;
	b = 2;
	c = 3;
	d = 4;
	e = 5;
	f = 6;
	g = 0;
	fprintf(stdout,
		"add3 before a:%d, b:%d, c:%d, d:%d, e:%d, f:%d, g:%d\n",
		a, b, c, d, e, f, g);
	__asm__ __volatile__(
		"ldr r0, %[e]\n"
		"ldr r1, %[f]\n"
		"push {r0, r1}\n"
		"ldr r0, %[a]\n"
		"ldr r1, %[b]\n"
		"ldr r2, %[c]\n"
		"ldr r3, %[d]\n"
		/* bl是跳转之前将下条指令的地址压栈 */
		"bl add3\n"
		"mov %[g], r0\n"
		"add sp, sp, #8\n"
		:[g]"=&r"(g)
		:[a]"m"(a), [b]"m"(b), [c]"m"(c), [d]"m"(d), [e]"m"(e), [f]"m"(f) 
		:
	);
	fprintf(stdout,
		"add3 after a:%d, b:%d, c:%d, d:%d, e:%d, f:%d, g:%d\n",
		a, b, c, d, e, f, g);
	return 0;
}

int add3(int a, int b, int c ,int d, int e, int f)
{
	fprintf(stdout,
		"in the add3 a:%d, b:%d, c:%d, d:%d, e:%d, f:%d\n",
		a, b, c, d, e, f);
	return a + b + c + d + e + f;
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
