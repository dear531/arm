#ifndef __WDT_H__
#define __WDT_H__
extern void wdt_init(int ms);
extern void wdt_enable(void);
extern void wdt_disable(void);

extern void wdt_irq_init(int ms);
#endif
