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
#include <linux/workqueue.h>

struct tasklet_struct ts1 = {}, ts2 = {};
struct work_struct work = {};
struct delayed_work dwork = {};

#if 0
#define S3C2410_GPFxx(_nr) S3C2410_GPF(_nr)
#endif

int irq = 0;
int data1 = 0, data2 = 1;
#if 0
typedef irqreturn_t (*irq_handler_t)(int, void *);
#endif
#if 0
typedef void (*work_func_t)(struct work_struct *work);
#endif

void prink_interrupt_process_content(const char*func)
{
	/* 查看是中断上下文还是进程上下文函数 */
	char *p = NULL;
	if (in_interrupt()) {
	/* 非零中断上下文 */
		p = "interrupt";
	} else {
	/* 0值进程上下文 */
		p = "process";
	}
	printk("%s is in %s content\n",
			func, p);
}

void work_handler(struct work_struct *work)
{
	prink_interrupt_process_content(__func__);
}

void dwork_handler(struct work_struct *work)
{
	prink_interrupt_process_content(__func__);
}

void tasklet_isr1(unsigned long data)
{
	prink_interrupt_process_content(__func__);
	printk("%lu\n", data);
#if 0
static inline void tasklet_schedule(struct tasklet_struct *tasklet)
#endif
#if 0 
static inline void tasklet_schedule(struct tasklet_struct *t)
#endif
}

void tasklet_isr2(unsigned long data)
{
	prink_interrupt_process_content(__func__);
	printk("%lu\n", data);
#if 0
static inline void tasklet_schedule(struct tasklet_struct *tasklet)
#endif
#if 0 
static inline void tasklet_schedule(struct tasklet_struct *t)
#endif
}

void irq_bh(struct softirq_action *action)
{
	/* 软中断的下半部分，仍然处于中断上下文
	 * 所以仍然不能睡眠 */
#if 0
	void *v = NULL;
#endif
	/* 打印会显示中断下半部分为中断上下文 */
	prink_interrupt_process_content(__func__);
#if 0 /* 中断上下文在睡眠情况下会内核崩溃 */
# if 1
	ssleep(2);
# else
	v = kzalloc(10000000, GFP_KERNEL);
	if (!v) {
		printk("%s v:%p\n",
			__func__, v);
	}
# endif
#endif
	return;
}

irqreturn_t button0_irq_handle(int irq, void *data)
{

	/* 需要立即处理的，在中断处理函数上半部分中执行
	 * 可以延迟的放在中断下半部分执行
	 */
#if 0
	/* failure */
	IRQ_NONE		= (0 << 0),
	/* success */
	IRQ_HANDLED		= (1 << 0),
#endif
	printk("irq %d\n", irq);

	open_softirq(MY_SOFTIRQ, irq_bh);
	raise_softirq(MY_SOFTIRQ);

	tasklet_schedule(&ts1);
	tasklet_hi_schedule(&ts2);

	INIT_WORK(&work, work_handler);
	INIT_DELAYED_WORK(&dwork, dwork_handler);

	schedule_work(&work);
	schedule_delayed_work(&dwork, HZ*3);

	prink_interrupt_process_content(__func__);

	return IRQ_HANDLED;
}

static __init int irq_test_init(void)
{
	int ret = 0;
#if 0
static inline void tasklet_init
(struct tasklet_struct *tasklet,
void (*func)(unsigned long),
unsigned long data)
#endif
	tasklet_init(&ts1, tasklet_isr1, 123);
	tasklet_init(&ts2, tasklet_isr2, 123);
#if 0
	irq = gpio_to_irq(2);
#else
	/* HOME */
	irq = gpio_to_irq(EXYNOS4_GPX1(1));
	printk("irq:%d\n", irq);
#endif
	prink_interrupt_process_content(__func__);
#if 0
extern int __must_check
request_threaded_irq(unsigned int irq, irq_handler_t
handler,
                     irq_handler_t thread_fn,
                     unsigned long flags, const char
*name, void *dev);
#endif
	/* request irq */
	ret = request_irq(irq, button0_irq_handle,
		IRQF_TRIGGER_FALLING | IRQF_SHARED, "my_irq",
		&data1);
	if (ret) {
		goto err;
	}
#if 0
	ret = request_irq(irq, button0_irq_handle,
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
