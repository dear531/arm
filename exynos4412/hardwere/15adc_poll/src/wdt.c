#include <wdt.h>
#include <common.h>
#include <irq.h>

#define WDT_BASE 0x10060000
#define WTCON		(*(volatile unsigned int *)(WDT_BASE + 0x0000))
#define WTDAT		(*(volatile unsigned int *)(WDT_BASE + 0x0004))
#define WTCNT		(*(volatile unsigned int *)(WDT_BASE + 0x0008))
#define WTCLRINT	(*(volatile unsigned int *)(WDT_BASE + 0x000C))

#define AUTOMATIC_WDT_RESET_DISABLE	(*(volatile unsigned int *)(0x10020000 + 0x0408))
#define MASK_WDT_RESET_REQUEST		(*(volatile unsigned int *)(0x10020000 + 0x040C))

void wdt_init(int ms)
{
	/* 100MHz / 100 prescaler / 64 mux = 15625 */
	WTCON = (99 << 8) | (2 << 3);
	WTCNT = 15625 * ms / 1000;
	WTDAT = 15625 * ms / 1000;
}

void wdt_enable(void)
{
	WTCON |= 1 << 5;
}

void wdt_disable(void)
{
	WTCON &= ~(1 << 5);
}

void do_wdt(void)
{
	printf("irq func %s:%d and clean irq\n",
		__func__, __LINE__);
	/* 清除中断 Write any value to clear the interrupt */
	WTCLRINT = 1;
}

void wdt_irq_init(int ms)
{
	wdt_init(ms);
	WTCON &= ~0x7;
	/* enable interrupt */
	WTCON |= 1 << 2;
	request_irq(75, do_wdt);
	wdt_enable();
}

void wdt_reset_init(int ms)
{
	wdt_init(ms);
	WTCON &= ~0x7;
	/* assert reset */
	WTCON |= 1 << 0;
	/* disable for WDT reset disable register */
	AUTOMATIC_WDT_RESET_DISABLE = 0;
	/* disable for WDT reset request mask register */
	MASK_WDT_RESET_REQUEST = 0;
	request_irq(75, do_wdt);
	wdt_enable();
}


