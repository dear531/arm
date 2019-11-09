#include <common.h>
#include <hardware.h>
#include <lib.h>
#include <chipid.h>
#include <led.h>
#include <buzzer.h>
#include <pwm.h>
#include <clock.h>
#include <lcd.h>

int main(void)
{
#define LCDBLK_CFG2 (*(volatile unsigned int *)(0x10010000 + 0x0214))
	LCDBLK_CFG2 |= 1;
#if 0
#define GPC0CON (*(volatile unsigned int *)(0x11400000 + 0x0060))
#define GPC0DAT (*(volatile unsigned int *)(0x11400000 + 0x0064))
	GPC0CON &= ~(0xf << 8);
	GPC0CON |= (0x1 << 8);
	GPC0DAT |= (1 << 2);
#define GPL1CON (*(volatile unsigned int *)(0x11400000 + 0x00E0))
	GPL1CON &= ~(0xf << 4);
	GPL1CON |= (0x1 << 4);
#define GPD1DAT (*(volatile unsigned int *)(0x11400000 + 0x00E4))
	GPD1DAT |= (0x1 < 1);	
#define GPX0CON (*(volatile unsigned int *)(0x11000000 + 0x0C00))
#define GPX0DAT (*(volatile unsigned int *)(0x11000000 + 0x0C04))
	GPX0CON &= ~0xf;
	GPX0CON |= 1;
	GPX0DAT |= 1;

#define GPD0CON	(*(volatile unsigned int *)(0x11400000 + 0xa0))
	GPD0CON &= ~(0xf << 4);
	GPD0CON |= (0x1 << 4);
#define GPD0DAT	(*(volatile unsigned int *)(0x11400000 + 0xa4))
	GPD0DAT |= (0x1 << 1);	
#endif
#if 1
	int x = 0, y = 0;

	pwm_init();
	clock_for_lcd();
	lcd_init();
	lcd_for_time();
	lcd_wind0_init();

	for (x = 0; 480 > x; x++) {
		for (y = 0; 272 > y; y++) {
			draw_point(0, x, y, 32-1, 64-1, 32-1);
		}
	}
	printf("%x,%x\n", x, y);
#endif
	return 0;
}
