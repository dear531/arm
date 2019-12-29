#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>



int my_print(int no)
{
	int i = 0;
	for (i = 0; no > i; i++) {
		printk("no : %d\n", i);
	}

	return i - 1;
}
EXPORT_SYMBOL(my_print);

/* when install then perform */
static __init int module_test2_init(void)
{
	printk("%s:%s:%d\n",__FILE__, __func__, __LINE__);
	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test2_exit(void)
{
	printk("%s:%s:%d\n",__FILE__, __func__, __LINE__);
}

module_init(module_test2_init);
module_exit(module_test2_exit);
MODULE_LICENSE("GPL");
