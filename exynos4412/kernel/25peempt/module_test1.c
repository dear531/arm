#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
#if 0
	/*
	 * 验证内核运行于抢占模式时，可以由用户态抢占，在kernel的preempt配置
	 * app运行挂在后台之后，本模块插入，用户程序仍能继续打印
	 */
	while (1) {}
#elif 0
	/* 
	 * 验证内核运行于抢占模式时，关闭允许抢占期间，用户态程序
	 * 无法进行抢占，先运行app挂在后台，插入模块，模块delay期间
	 * 用户态程序不打印
	 */
	preempt_disable();
	mdelay(5000);
	preempt_enable();
#else
	/*
	 *  验证内核运行于非抢占模式时，用户态无法抢占
	 */
	mdelay(5000);
#endif
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
