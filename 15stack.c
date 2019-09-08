#include <stdio.h>

int main(void)
{
	int a = 1, b = 2, c = 3, d = 4;
	fprintf(stdout, "a:%d, b:%d, c:%d, d:%d\n",
		a, b, c, d);
	asm volatile (
		"mov r0, %[a]\n"
		"mov r1, %[b]\n"
		"mov r2, %[c]\n"
		"mov r5, %[d]\n"
		"stmfd sp!, {r0-r2, r5}\n"
		"mov r0, #0\n"
		"mov r1, #0\n"
		"mov r2, #0\n"
		"mov r5, #0\n"
		"ldmfd sp!, {r0-r2, r5}\n"
		"mov %[a], r0\n"
		"mov %[b], r1\n"
		"mov %[c], r2\n"
		"mov %[d], r5\n"
		:[a]"+r"(a) ,[b]"+r"(b) ,[c]"+r"(c) ,[d]"+r"(d)
		:
		:
	);
	fprintf(stdout, "stmfd ldmfd -- a:%d, b:%d, c:%d, d:%d\n",
		a, b, c, d);
	asm volatile (
		"mov r0, %[a]\n"
		"mov r1, %[b]\n"
		"mov r2, %[c]\n"
		"mov r5, %[d]\n"
		"push {r0-r2, r5}\n"
		"mov r0, #0\n"
		"mov r1, #0\n"
		"mov r2, #0\n"
		"mov r5, #0\n"
		"pop {r0-r2, r5}\n"
		"mov %[a], r0\n"
		"mov %[b], r1\n"
		"mov %[c], r2\n"
		"mov %[d], r5\n"
		:[a]"+r"(a) ,[b]"+r"(b) ,[c]"+r"(c) ,[d]"+r"(d)
		:
		:
	);
	fprintf(stdout, "push pop -- a:%d, b:%d, c:%d, d:%d\n",
		a, b, c, d);
	return 0;
}
