#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int ret = -1, len = -1;
	char *buff = "arm asm write\n";
	len = strlen("arm asm write\n");
	write(STDOUT_FILENO, "syscall write\n", strlen("syscall write\n"));
	syscall(4, STDOUT_FILENO, "syscall call 4\n", strlen("syscall call 4\n"));
	asm volatile (
		/* incorrect */
		/* 第一二三个参数分别通过第0，1，2个寄存器传入 */
		"mov r0, #1\n"
		"mov r1, %[buff]\n"
		"mov r2, %[len]\n"
		/* 进入内核调用4号系统调用，即write */
		"bl write\n"
		/* "swi #0x900004\n" */
		/* 返回值通过r0返回 */
		"mov %[ret], r0\n"
		:[ret]"=&r"(ret)
		:[len]"r"(len), [buff]"r"(buff)
		:"r0", "r1", "r2"
	);
	fprintf(stdout, "ret :%d\n", ret);
	return 0;
}
