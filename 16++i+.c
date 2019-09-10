#include <stdio.h>
#if 0
zhangly@u12:~/Arm/myroot/root/arm$ make 16++i+
arm-none-linux-gnueabi-gcc -march=armv7-a    16++i+.c   -o 16++i+
zhangly@u12:~/Arm/myroot/root/arm$ arm-none-linux-gnueabi-objdump -D 16++i+ > 16++i+.s
#endif
int main(void)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	k++;
	m = 2;
	++k;
	m = 3;
	j = i++ + ++i + ++i + i++;
	fprintf(stdout, "i:%d, j:%d\n", i, j);
	return 0;
}
