#include <stdio.h>
/*
 * zhangly@u12:~/Arm/myroot/root/arm$ make 19section
 * arm-none-linux-gnueabi-gcc -march=armv7-a    19section.c   -o 19section
 * zhangly@u12:~/Arm/myroot/root/arm$ arm-none-linux-gnueabi-objdump 19section -D > section.s 
*/
int a1 = 0;	/* bss */
int a2;		/* bss */
int a3 = 1;	/* data */

int main(void)
{
	static int a4 = 0;	/* bss */
	static int a5;		/* bss */
	static int a6 = 1;	/* data */
	static char *p2 = "nihao";	/* p2:data,nihao->rodata */
	int a7 = 0;		/* stack */
	int a8;			/* stack */
	char *p1 = "hello";	/* p1:stack, hello->rodata */
	return 0;
}
