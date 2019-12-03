#ifndef __IRQ_H__
#define __IRQ_H__
extern void request_irq(int no, void (*handler)(void));
extern void do_irq(void);
#endif
