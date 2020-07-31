#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

struct hrtimer timer;

enum hrtimer_restart timer_handler(struct hrtimer *hrt)
{
#if 0
	printk("%s:%d:%s\n", __FILE__, __LINE__, __func__);
	return HRTIMER_NORESTART;
#else
# if 0
static inline u64 hrtimer_forward_now(struct hrtimer *timer,
				      ktime_t interval)
# endif
	ktime_t kt = ktime_set(2, 20000000);
	hrtimer_forward_now(&timer, kt);
	printk("%s:%d:%s\n", __FILE__, __LINE__, __func__);
	return HRTIMER_RESTART;
#endif
}

/* when install then perform */
static __init int module_test1_init(void)
{
	ktime_t kt = ktime_set(2, 20000000);
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	timer.function = timer_handler;
#if 0
int
hrtimer_start(struct hrtimer *timer, ktime_t tim, const enum hrtimer_mode mode)
#endif
	hrtimer_start(&timer, kt, HRTIMER_MODE_REL);
#if 0
	hrtimer_cancel(struct hrtimer *timer);	
#endif
	printk("%s:%d\n", __func__, __LINE__);
	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	hrtimer_cancel(&timer);	
	printk("%s:%d\n", __func__, __LINE__);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
