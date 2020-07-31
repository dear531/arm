#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
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
