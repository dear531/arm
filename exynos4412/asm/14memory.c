#include <stdio.h>

int main(void)
{
	int arr[] = {1, 2, 3, 4, 5};
	int i = 0;
	int m = 0;
	asm volatile (
		/* a[0] */
#if 0
		/* 取arr变量的值，放入r0寄存器中 */
		"mov r0, %[arr]\n"
		/* []代表按地址取值，r1=*r0 */
		"ldr r1, [r0]\n"
#else
		"ldr r1, [%[arr]]\n"
		"add r1, r1, #1\n"
		/* 将寄存器的值赋值给地址对应的变量, *arr=r1 */
		"str r1, [%[arr]]\n"
#endif
		/* a[1] */
#if 1
		"mov r0, %[arr]\n"
		"add r0, r0, #4\n"
		"ldr r1, [r0]\n"
		"add r1, #1\n"
		"str r1, [r0]\n"
#endif
		/* a[2] */
		/* r1 = *(arr + 4) */
		"ldr r1, [%[arr], #8]\n"
		"add r1, r1, #1\n"
		/* *(arr + 4) = r1 */
		"str r1, [%[arr], #8]\n"

		/* a[3] */
		"mov r0, %[arr]\n"
		/* asm :[r0, #0xc]! <==> c: r0 += 0xc */
		"ldr r1, [r0, #0xc]!\n"
		"add r1, r1, #1\n"
		"str r1, [r0]\n"
#if 1
		/* r1=*(r0); r0+=4 */
		"str r1, [r0], #4\n"
		"ldr r1, [r0]\n"
		"add r1, r1, #1\n"
		"str r1, [r0]\n"
#endif
		/* 注意m当作内存值传入时，
		 * 与当作寄存器值传入时的区别，
		 * 当作内存传入时，可直接当作内存地址操作，
		 * 不用再加[]进行转换 */
		/* 以下是变量直接将内存地址传入时的操作 */
		"ldr r0, %[m]\n"
		"add r0, r0, #1\n"
		"str r0, %[m]\n"
		:
		:[arr]"r"(arr), [m]"m"(m)
		/* 只要操作内存，就要加上memory */
		:"r0", "r1", "memory"
	);
	for (i = 0; i < sizeof(arr) / sizeof(*arr); i++) {
		fprintf(stdout, "arr[%d]:%d\n", i, arr[i]);
	}
	fprintf(stdout, "m:%d\n", m);
	return 0;
}
