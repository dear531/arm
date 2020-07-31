#ifndef __LED_H__
#define __LED_H__
#define GPL2CON (*(volatile unsigned int *)(0x11000000 + 0x0100))
#define GPL2DAT (*(volatile unsigned int *)(0x11000000 + 0x0104))
#define GPK1CON (*(volatile unsigned int *)(0x11000000 + 0x0060))
#define GPK1DAT (*(volatile unsigned int *)(0x11000000 + 0x0064))
extern void led_init(void);
/* no [0,1] */
extern void led_on(int no);
extern void led_off(int no);
#endif
