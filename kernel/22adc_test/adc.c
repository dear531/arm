#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include "adc.h"

volatile unsigned char *v = 0x00;

void adc_init(void)
{
#if 0
static inline void __iomem *ioremap(unsigned long port, unsigned long size)
#endif
	v = ioremap(ADC_BASE, SZ_4K);
	if (!v) {
		printk("ioremap adc error\n");
		return;
	}
	ADCCON = (1 << 16) | (1 << 14) | (25 << 6);
	ADCDLY = 0xffff;

	ADCMUX = 0;
}

void adc_start(void)
{
	ADCCON |= 1;
}

void wait_for_adc(void)
{
	while (0 == (ADCCON & (1 << 15)));

}

int get_adc(void)
{
	return ADCDAT & 0xfff;
}

