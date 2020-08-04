#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>

ssize_t buttom_read(struct file *fp, char __user * buff, size_t count, loff_t * off)
{
	return 0;
}

ssize_t buttom_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	return 0;
}

int buttom_open(struct inode *inode, struct file *fp)
{
	return 0;
}

struct file_operations buttom_drv_file_st = {
	.read = buttom_read,
	.write = buttom_write,
	.open = buttom_open,
};

static unsigned int major;
struct class *buttom_class_st;
struct class_device *buttom_class_device;
static int __init buttom_drv_init(void)
{
	int ret;
	ret = register_chrdev(0, "buttom_drv", &buttom_drv_file_st);
	if (0 > ret) {
		goto register_chrdev_error;
	}
	major = ret;
	buttom_class_st = class_create(THIS_MODULE, "buttom_drv_class");
	if (IS_ERR(buttom_class_st)) {
		ret = PTR_ERR(buttom_class_st);
		goto class_create_error;
	}
#if 0
struct class_device *class_device_create(struct class *cls,
					 struct class_device *parent,
					 dev_t devt,
					 struct device *device,
					 const char *fmt, ...)
#endif
	buttom_class_device = class_device_create(buttom_class_st,
			NULL, MKDEV(major, 0), NULL, "buttom_drv_class_dev");
	if (IS_ERR(buttom_class_device)) {
		ret = PTR_ERR(buttom_class_device);
		goto class_device_create_error;
	}

	return 0;
	class_device_destroy(buttom_class_st, MKDEV(major, 0));
class_device_create_error:
	class_destroy(buttom_class_st);
class_create_error:
	unregister_chrdev(major, "sencond_drv");
register_chrdev_error:
	return ret;
}

static void __exit buttom_drv_exit(void)
{
	return;
}

module_init(buttom_drv_init);
module_exit(buttom_drv_exit);

MODULE_LICENSE("GPL");
