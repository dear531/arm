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
#include <linux/delay.h>

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
	printk("This is %s %d data:%p\n",
		__func__, irq, data);
	return IRQ_HANDLED;
}

static __init int irq_test_init(void)
{
	int ret = 0;
	unsigned long flag = 0;
#if 1
	/* HOME key */
	irq = gpio_to_irq(EXYNOS4_GPX1(1));
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
		IRQF_TRIGGER_FALLING
		| IRQF_SHARED, "my_irq", &data1);
	if (ret) {
		goto err;
	}

	ret = request_irq(irq, irq_handler,
		IRQF_TRIGGER_FALLING |
		IRQF_SHARED, "my_irq", &data2);
	if (ret) {
		goto err;
	}
#if 0
#if 1
	local_irq_disable();
#else
	local_irq_save(flag);
#endif
#endif
	mdelay(50000);
#if 0
#if 1
	local_irq_enable();
#else
	local_irq_restore(flag);
#endif
#endif

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
