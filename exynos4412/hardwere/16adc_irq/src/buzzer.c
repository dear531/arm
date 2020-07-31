#include <buzzer.h>

void buzzer_init(void)
{
	GPD0CON &= 0xfffffff0;
	GPD0CON |= 0x00000001;
	GPD0DAT &= 0xfffffffe;
}

void buzzer_ops(int mode)
{
	if (mode)
		GPD0DAT |= 0x00000001;
	else
		GPD0DAT &= 0xfffffffe;
}
