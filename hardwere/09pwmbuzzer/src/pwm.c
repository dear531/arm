#include <pwm.h>
#include <common.h>

void pwm_init(void)
{
	GPD0CON &= ~0xf;
	GPD0CON |= 0x2;
	/* 100MHz pclock / 100 */
	TCFG0 &= ~0xff;
	TCFG0 |= 99;
	/* 1MHz / 4 = 250kHz */
	TCFG1 &= 0xf;
	TCFG1 |= 2;

	TCNTB0 = 250000 - 1;
	TCMPB0 = 125000 - 1;

	TCON &= ~0x1f;
	TCON |= (1 << 3) | (1 << 1) | (1 << 0);
	/* 停止刷新操作，否则定时器无法递减到零，
	过一个时钟就会被初始化 */
	TCON &= ~(1 << 1);

	/* 设置不产生中断信号 */
	TINT_CSTAT |= 0;
}

void pwm_set(int cmp, int count)
{
	TCNTB0 = count - 1;
	TCMPB0 = cmp - 1;
}
