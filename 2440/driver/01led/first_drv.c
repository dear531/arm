#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>

static struct class *firstdrv_class;
static struct class_device *firstdrv_class_devs;

int first_open(struct inode *inode, struct file *fp)
{
	printk("%s:%s:%d\n", __func__, __FILE__,  __LINE__);
	return 0;
}

ssize_t first_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	printk("%s:%s:%d\n", __func__, __FILE__,  __LINE__);
	return 0;
}

static struct file_operations first_drv_fops= {
	.owner = THIS_MODULE,
	.open = first_open,
	.write = first_write,
};

static int major;
static int __init first_init(void)
{
	int ret;
	/* 注册字符设备到系统内核
	 * # cat /proc/devices |grep first
	 * 252 first_drv
	 */
	major = register_chrdev(0, "first_drv", &first_drv_fops);
	if (0 > major) {
		ret = major;
		goto register_chrdev_error;
	}

	/* 为映射到/dev/xxx文件创建接口结构体
	 * # cat /sys/class/first_drv_class/first_dev_file/dev
	 * 252:0
	 */
	firstdrv_class = class_create(THIS_MODULE, "first_drv_class");
	if (IS_ERR(firstdrv_class)) {
		ret = PTR_ERR(firstdrv_class);
		goto class_create_error;
	}

	/* 创建设备文件
	 * # ls /dev/first_dev_file -l
	 * crw-rw----  1 0    0   252,0 Jan  1 03:08 /dev/first_dev_file
	 */
	firstdrv_class_devs = class_device_create(firstdrv_class, NULL,
		MKDEV(major, 0), NULL, "first_dev_file");
	if (IS_ERR(firstdrv_class_devs)) {
		ret = PTR_ERR(firstdrv_class_devs);
		goto class_device_create_error;
	}
	return 0;

	class_device_destroy(firstdrv_class, MKDEV(major, 0));
class_device_create_error:
	class_destroy(firstdrv_class);
class_create_error:
	unregister_chrdev(major, "first_drv");
register_chrdev_error:
	return ret;
}

static void __exit first_exit(void)
{
	class_device_destroy(firstdrv_class, MKDEV(major, 0));
	class_destroy(firstdrv_class);
	unregister_chrdev(major, "first_drv");
	return;
}

module_init(first_init);
module_exit(first_exit);

MODULE_LICENSE("GPL");
