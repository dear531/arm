#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>

dev_t dev_no = 0;
/* when install then perform */
static __init int module_test1_init(void)
{
	int ret = 0;
	dev_no = MKDEV(222, 2);
#if 0
#if 0
int register_chrdev_region(dev_t from, unsigned count, const char *name)
#endif
	ret = register_chrdev_region(dev_no, 3, "my_chardev");
#else
#if 0
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name)
#endif
	ret = alloc_chrdev_region(&dev_no, 2, 3, "my_chardev");
#endif
	if (ret < 0) {
		goto register_error;
	}
	return 0;
register_error:
	printk("ret:%d\n", ret);
	return ret;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
#if 0
void unregister_chrdev_region(dev_t from, unsigned count)
#endif
	unregister_chrdev_region(dev_no, 3);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
