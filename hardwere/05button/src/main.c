#include <common.h>
#include <hardware.h>
#include <lib.h>
#include <chipid.h>
#include <led.h>

int main(void)
{
	int count = 101;
	int flag = 0;
	led_init();
	while (1) {
		flag = button_state();
		if (2 == flag) {
			led_off(0);
			led_off(1);
		} else {
			led_on(flag);
		}
	}

	return 0;
}
