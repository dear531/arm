#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/gpio.h>

#if 0
#define S3C2410_GPFxx(_nr) S3C2410_GPF(_nr)
#endif

int irq = 0;
int data1 = 0, data2 = 1;
#if 0
typedef irqreturn_t (*irq_handler_t)(int, void *);
#endif
irqreturn_t irq_handler(int irq, void *data)
{
#if 0
	/* failure */
	IRQ_NONE		= (0 << 0),
	/* success */
	IRQ_HANDLED		= (1 << 0),
#endif
	return IRQ_HANDLED;
}

static __init int irq_test_init(void)
{
	int ret = 0;
	irq = gpio_to_irq(2);
#if 0
	irq = gpio_to_irq(S3C2410_GPF(2));
#endif
#if 0
extern int __must_check
request_threaded_irq(unsigned int irq, irq_handler_t
handler,
                     irq_handler_t thread_fn,
                     unsigned long flags, const char
*name, void *dev);
#endif
	/* request irq */
	ret = request_irq(irq, irq_handler,
		IRQF_TRIGGER_FALLING | IRQF_SHARED, "my_irq",
		&data1);
	if (ret) {
		goto err;
	}

	ret = request_irq(irq, irq_handler,
		IRQF_TRIGGER_FALLING |
		IRQF_SHARED, "my_irq", &data2);
	if (ret) {
		goto err;
	}

	return 0;
err:
	return ret;
}

static __exit void irq_test_exit(void)
{
	/* free irq */
	free_irq(irq, &data2);
	free_irq(irq, &data1);
}

module_init(irq_test_init);
module_exit(irq_test_exit);
MODULE_LICENSE("GPL");
