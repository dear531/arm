#ifndef __PWM_H__
#define __PWM_H__
#define GPD0CON (*(volatile unsigned int*)(0x11400000 + 0x00A0))
#define PWM_Base 0x139D0000
#define TCFG0	(*(volatile unsigned int *)(PWM_Base + 0x0000))
#define TCFG1	(*(volatile unsigned int *)(PWM_Base + 0x0004))
#define TCON	(*(volatile unsigned int *)(PWM_Base + 0x0008))
#define TCNTB0	(*(volatile unsigned int *)(PWM_Base + 0x000C))
#define TCMPB0	(*(volatile unsigned int *)(PWM_Base + 0x0010))
#define TCNTO0	(*(volatile unsigned int *)(PWM_Base + 0x0014))
#define TCNTB1	(*(volatile unsigned int *)(PWM_Base + 0x0018))
#define TCMPB1	(*(volatile unsigned int *)(PWM_Base + 0x001C))
#define TCNTO1	(*(volatile unsigned int *)(PWM_Base + 0x0020))
#define TCNTB2	(*(volatile unsigned int *)(PWM_Base + 0x0024))
#define TCMPB2	(*(volatile unsigned int *)(PWM_Base + 0x0028))
#define TCNTO2	(*(volatile unsigned int *)(PWM_Base + 0x002C))
#define TCNTB3	(*(volatile unsigned int *)(PWM_Base + 0x0030))
#define TCMPB3	(*(volatile unsigned int *)(PWM_Base + 0x0034))
#define TCNTO3	(*(volatile unsigned int *)(PWM_Base + 0x0038))
#define TCNTB4	(*(volatile unsigned int *)(PWM_Base + 0x003C))
#define TCNTO4	(*(volatile unsigned int *)(PWM_Base + 0x0040))
#define TINT_CSTAT	(*(volatile unsigned int *)(PWM_Base + 0x0044))
void pwm_init(void);
#endif
