#include <pwm.h>
#include <common.h>

void pwm_init(void)
{
	GPD0CON &= ~(0xf << 4);
	GPD0CON |= (0x1 << 4);
#define GPD0DAT	(*(volatile unsigned int *)(0x11400000 + 0xa4))
	GPD0DAT |= (0x1 << 1);	
#define GPL1CON (*(volatile unsigned int *)(0x11400000 + 0x00E0))
	GPL1CON &= ~(0xf << 4);
	GPL1CON |= (0x1 << 4);
#define GPD1DAT (*(volatile unsigned int *)(0x11400000 + 0x00E4))
	GPD1DAT |= (0x1 < 1);	
#define GPA0CON (*(volatile unsigned int *)(0x11400000 + 0x0000))
#define GPA0DAT	(*(volatile unsigned int *)(0x11400000 + 0x0004))
	GPA0CON &= ~(0xf << 24);
	GPA0CON |= (0x1 << 24);
	GPA0DAT	|= (1 << 6);
#if 0
	/* 100MHz pclock / 9 */
	TCFG0 &= ~0xff;
	TCFG0 |= 89;
	/* 1.1MHz / 1 = 1.1MHz */
	TCFG1 &= ~(0xf << 4);

	TCNTB1 = 1111111;
	TCMPB1 = 1111111;

	TCON &= ~0xf00;
	TCON |= (1 << (3 + 8)) | (1 << (1 + 8));
	TCON |= (1 << (0 + 8));
	/* 停止刷新操作，否则定时器无法递减到零，
	过一个时钟就会被初始化 */
	TCON &= ~(1 << (1 + 8));

	/* 设置不产生中断信号 */
	TINT_CSTAT &= (1 < 1);
#define LCD0_SYS_PWR_REG (*(volatile unsigned int*)(0x10020000 + 0x1390))
	printf("LCD0_SYS_PWR_REG:%#010x\n", LCD0_SYS_PWR_REG);
#endif
}

void pwm_set(int cmp, int count)
{
	TCNTB0 = count - 1;
	TCMPB0 = cmp - 1;
}
