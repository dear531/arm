#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#define GPFCON	0x56000050
#define GPFDAT	0x56000054
#define GPFUP	0x56000058

struct firstdrv {
	void __iomem *v;
	unsigned int gpfcon;
	unsigned int gpfdat;
};
struct firstdrv *firstdrv_st;

static struct class *firstdrv_class;
#define FIRST_DEV_NUM 4
static struct class_device *firstdrv_class_devs[FIRST_DEV_NUM];

int first_open(struct inode *inode, struct file *fp)
{
	printk("%s:%s:%d\n", __func__, __FILE__,  __LINE__);
	return 0;
}

ssize_t first_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	int n;
	int f_minor = MINOR(fp->f_dentry->d_inode->i_rdev);
	int ret;
	printk("%s:%s:%d\n", __func__, __FILE__,  __LINE__);

	ret = copy_from_user(&n, buff, sizeof(n));
	if (ret)
		return -ENOMEM;

	printk("%s:%s:%d:n:%d,minor:%d\n", __func__, __FILE__,  __LINE__, n, f_minor);

	if (0 == f_minor && 1 == n) {
		firstdrv_st->gpfdat = ioread32(firstdrv_st->v - GPFCON + GPFDAT);

		firstdrv_st->gpfdat = firstdrv_st->gpfdat \
			& ~(1 << 4 | 1 << 5 | 1 << 6);
		iowrite32(firstdrv_st->gpfdat, firstdrv_st->v - GPFCON + GPFDAT);
	} else if (0 == f_minor && 0 == n) {
		firstdrv_st->gpfdat = ioread32(firstdrv_st->v - GPFCON + GPFDAT);
		firstdrv_st->gpfdat = firstdrv_st->gpfdat \
			| (1 << 4 | 1 << 5 | 1 << 6);
		iowrite32(firstdrv_st->gpfdat, firstdrv_st->v - GPFCON + GPFDAT);
	} else if (1 <= f_minor && 3 >= f_minor && 1 == n) {
		firstdrv_st->gpfdat = ioread32(firstdrv_st->v - GPFCON + GPFDAT);
		firstdrv_st->gpfdat = firstdrv_st->gpfdat \
			& ~(1 << (3 + f_minor));
		iowrite32(firstdrv_st->gpfdat, firstdrv_st->v - GPFCON + GPFDAT);
	} else if (1 <= f_minor && 3 >= f_minor && 0 == n) {
		firstdrv_st->gpfdat = ioread32(firstdrv_st->v - GPFCON + GPFDAT);
		firstdrv_st->gpfdat = firstdrv_st->gpfdat \
			| (1 << (3 + f_minor));
		iowrite32(firstdrv_st->gpfdat, firstdrv_st->v - GPFCON + GPFDAT);
	}

	return 0;
}

static struct file_operations first_drv_fops= {
	.owner = THIS_MODULE,
	.open = first_open,
	.write = first_write,
};

static int major;
static int minor;
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
#if 0
struct class_device *class_device_create(struct class *cls,
					 struct class_device *parent,
					 dev_t devt,
					 struct device *device,
					 const char *fmt, ...)
#endif
#if 0
# ls /dev/first_dev_file* -l
crw-rw----  1 0  0  252, 0 Jan  1 10:52 /dev/first_dev_file0
crw-rw----  1 0  0  252, 1 Jan  1 10:52 /dev/first_dev_file1
crw-rw----  1 0  0  252, 2 Jan  1 10:52 /dev/first_dev_file2
crw-rw----  1 0  0  252, 3 Jan  1 10:52 /dev/first_dev_file3
#endif
	for (minor = 0; FIRST_DEV_NUM > minor; minor++) {
		firstdrv_class_devs[minor] = class_device_create(firstdrv_class, NULL,
			MKDEV(major, minor), NULL, "first_dev_file%d", minor);
		if (!firstdrv_class_devs[minor]) {
			ret = -ENODEV;
			goto class_device_create_error;
		}
	}

	firstdrv_st = kzalloc(sizeof(*firstdrv_st), GFP_KERNEL);
	if (!firstdrv_st ) {
		ret = -ENOMEM;
		goto kzalloc_err;
	}

	firstdrv_st->v = ioremap(GPFCON, 0x10);
	if (!firstdrv_st->v) {
		ret = -EIO; 
		goto ioremap_err;
	}

	firstdrv_st->gpfcon = ioread32(firstdrv_st->v);
	firstdrv_st->gpfcon = (firstdrv_st->gpfcon \
		& (~(0x3 << (4*2) | 0x3 << (4*2) | 0x3 << (6*2)))) \
		| (0x1 << (4*2) | 0x1 << (5*2) | 0x1 << (6*2));
	iowrite32(firstdrv_st->gpfcon, firstdrv_st->v);

	return 0;

	iounmap(firstdrv_st->v);
ioremap_err:
	kfree(firstdrv_st);
kzalloc_err:
	while (minor--)
		class_device_destroy(firstdrv_class, MKDEV(major, minor));
class_device_create_error:
	class_destroy(firstdrv_class);
class_create_error:
	unregister_chrdev(major, "first_drv");
register_chrdev_error:
	return ret;
}

static void __exit first_exit(void)
{
	iounmap(firstdrv_st->v);
	kfree(firstdrv_st);
	while (minor--)
		class_device_destroy(firstdrv_class, MKDEV(major, minor));
	class_destroy(firstdrv_class);
	unregister_chrdev(major, "first_drv");
	return;
}

module_init(first_init);
module_exit(first_exit);

MODULE_LICENSE("GPL");
