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
void
prink_interrupt_process_content(const char * func)
{
	/* 查看是中断上下文还是进程上下文函数 */
	if (in_interrupt()) {
	/* 非零中断上下文 */
		printk("%s is in interrupt content\n",
		func);
	} else {
	/* 0值进程上下文 */
		printk("%s is not in process content\n",
			func);
	}
}

irqreturn_t irq_handler(int irq, void *data)
{
	void *v = NULL;
#if 0
	/* failure */
	IRQ_NONE		= (0 << 0),
	/* success */
	IRQ_HANDLED		= (1 << 0),
#endif
	printk("irq %d\n", irq);
	prink_interrupt_process_content(__func__);

	/* 进程在中断当中不允许睡眠的调试
	 * 睡眠时会出现内核崩溃 */
#if 0
# if 0
	ssleep(2);
# else
	v = kzalloc(10000000, GFP_KERNEL);
	if (!v) {
		printk("%s v:%p\n",
			__func__, v);
	}
# endif
#endif
	return IRQ_HANDLED;
}

static __init int irq_test_init(void)
{
	int ret = 0;
#if 0
	irq = gpio_to_irq(2);
#else
	/* HOME */
	irq = gpio_to_irq(EXYNOS4_GPX1(1));
	printk("irq:%d\n", irq);
#endif
	prink_interrupt_process_content(__func__);
	ssleep(2);
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
#if 0
	ret = request_irq(irq, irq_handler,
		IRQF_TRIGGER_FALLING |
		IRQF_SHARED, "my_irq", &data2);
	if (ret) {
		goto err;
	}
#endif

	return 0;
err:
	return ret;
}

static __exit void irq_test_exit(void)
{
	/* free irq */
#if 0
	free_irq(irq, &data2);
#endif
	free_irq(irq, &data1);
}

module_init(irq_test_init);
module_exit(irq_test_exit);
MODULE_LICENSE("GPL");
