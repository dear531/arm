#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>

struct timer_list timer;

void timer_handler(unsigned long data)
{
	if (in_interrupt()) {
		printk("interrupt\n");
	} else {
		printk("process\n");
	}
	/* reset timer */
#if 0
	timer.expires = jiffies + HZ * 2;
	add_timer(&timer);
#else
# if 0
int mod_timer(struct timer_list *timer, unsigned long expires)
# endif
	mod_timer(&timer, jiffies + HZ * 2);
#endif

	return;
}

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
#if 0
	init_timer(&timer);
	timer.function = timer_handler;
	timer.data = 123;
#else
#if 0
#define setup_timer(timer, fn, data)
#endif
	setup_timer(&timer, timer_handler, 456);
#endif
	timer.expires = jiffies + HZ * 2;
	add_timer(&timer);
	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	del_timer_sync(&timer);
	printk("%s:%d\n", __func__, __LINE__);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
