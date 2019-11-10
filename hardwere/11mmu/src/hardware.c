#include <common.h>
#include <hardware.h>
#if 0
#define printf ((int(*)(const char *, ...))0x43e40008)
#endif


void hardware_init(void)
{
	printf("%s %s\n", __FILE__, __func__);
}

void hardware_ops(void)
{
	printf("%s %s\n", __FILE__, __func__);
}
