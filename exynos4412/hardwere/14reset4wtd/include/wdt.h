#ifndef __WDT_H__
#define __WDT_H__

#define WDT_BASE 0x10060000
#define WTCON		(*(volatile unsigned int *)(WDT_BASE + 0x0000))
#define WTDAT		(*(volatile unsigned int *)(WDT_BASE + 0x0004))
#define WTCNT		(*(volatile unsigned int *)(WDT_BASE + 0x0008))
#define WTCLRINT	(*(volatile unsigned int *)(WDT_BASE + 0x000C))

#define AUTOMATIC_WDT_RESET_DISABLE	(*(volatile unsigned int *)(0x10020000 + 0x0408))
#define MASK_WDT_RESET_REQUEST		(*(volatile unsigned int *)(0x10020000 + 0x040C))

extern void wdt_init(int ms);
extern void wdt_enable(void);
extern void wdt_disable(void);

extern void wdt_irq_init(int ms);
#endif
