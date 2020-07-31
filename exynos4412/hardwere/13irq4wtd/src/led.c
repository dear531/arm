#include <common.h>
#include <led.h>



void led_init(void)
{
	/* gpm0[7] 初始化为输出功能 */
	GPL2CON &= 0xfffffff0;
	GPL2CON |= 0x00000001;
	GPL2DAT |= (1 << 0);
	GPK1CON &= 0xffffff0f;
	GPK1CON |= 0x00000010;
	GPK1DAT |= (1 << 1);
}

void led_on(int no)
{
	switch (no)
	{
		case 0:
			GPL2DAT |= (1 << 0);
			break;
		case 1:
			GPK1DAT |= (1 << 1);
			break;
		default:
			;
	}
}

void led_off(int no)
{
	switch (no)
	{
		case 0:
			GPL2DAT &= ~(1 << 0);
			break;
		case 1:
			GPK1DAT &= ~(1 << 1);
			break;
		default:
			;
	}
}
