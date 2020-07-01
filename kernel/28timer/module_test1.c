#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>

struct timer_list timer;

void timer_func(unsigned long data)
{
	if (in_interrupt()) {
		printk("interrupt\n");
	} else {
		printk("process\n");
	}
	return;
}

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	init_timer(&timer);
	timer.expires = jiffies + HZ * 2;
	timer.function = timer_func;
	timer.data = 123;
	add_timer(&timer);
	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
