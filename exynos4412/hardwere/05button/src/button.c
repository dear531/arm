#include <button.h>

void button_init(void)
{
	GPX1CON &= (0xffffff0f);
	GPX1CON |= (0x00000010);
}

int button_state(void)
{
	int flag = 0;
	if (0 == (GPX1DAT & (1 << 1)))
		flag = 1;
	else
		flag = 2;

	return flag;
}
