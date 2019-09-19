#include <common.h>
#include <hardware.h>
#include <lib.h>
#include <chipid.h>
#include <led.h>
#include <buzzer.h>

int main(void)
{
	int count = 101;
	int flag = 0;
	buzzer_init();
	while (1) {
		flag = button_state();
		buzzer_ops(flag);
	}

	return 0;
}
