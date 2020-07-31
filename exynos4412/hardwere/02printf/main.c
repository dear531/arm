#include <common.h>
#include <hardware.h>
#include <lib.h>

int main(void)
{
	int count = 5;
	printf("Welcom to second part for arm :"
		"arm asm program on bare -metal\n"
		"This is my first program\n");
	while (count--) {
		hardware_init();
		hardware_ops();
		udelay(1000 * 1000);
#if 0
		delay(100);
#endif
	}

	return 0;
}
