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
		delay(100);
	}

	return 0;
}
