#ifndef __BUTTON_H__
#define __BUTTON_H__
#define GPX1CON (*(volatile unsigned int *)(0x11000000 + 0x0C20))
#define GPX1DAT (*(volatile unsigned int *)(0x11000000 + 0x0C24))
extern void button_init(void);
extern int button_state(void);
#endif
