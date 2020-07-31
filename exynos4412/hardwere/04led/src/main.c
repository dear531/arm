#include <common.h>
#include <hardware.h>
#include <lib.h>
#include <chipid.h>
#include <led.h>

int main(void)
{
	int count = 101;
	printf("Welcom to second part for arm :"
		"arm asm program on bare -metal\n"
		"This is my first program\n");
	led_init();
	while (count--) {
		led_on(count % 2);
		led_off((count + 1) % 2);
		udelay(1000 * 1000);
		printf("count :%d\n", count);
		
#if 0
		delay(100);
#endif
	}

	return 0;
}
