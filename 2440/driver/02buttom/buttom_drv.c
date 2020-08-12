#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#define GPFCON 0x56000050
#define GPFDAT 0x56000054
#define GPGCON 0x56000060
#define GPGDAT 0x56000064


struct buttom_st {
void __iomem *cpf_v;
void __iomem *cpg_v;
};

struct buttom_st second_buttom_st;
#if 0
GPF0
GPF2
GPG3
GPG11
#endif
ssize_t buttom_read(struct file *fp, char __user * buff, size_t count, loff_t * off)
{
	unsigned int data;
	unsigned char key_values[4];
	int ret;
	int i;

	data = ioread32(second_buttom_st.cpf_v + GPFDAT - GPFCON);
	key_values[0] = !!(data & (1 << 0));
	key_values[1] = !!(data & (1 << 2));

	data = ioread32(second_buttom_st.cpg_v + GPGDAT - GPGCON);
	key_values[2] = !!(data & (1 << 3));
	key_values[3] = !!(data & (1 << 11));

	ret = copy_to_user(buff, key_values, sizeof(key_values));

	if (ret) {
		printk("ret:%d\n", ret);
		return -ENOMEM;
	}
	return sizeof(key_values);
}

ssize_t buttom_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	return 0;
}

int buttom_open(struct inode *inode, struct file *fp)
{
	printk("%s:%s\n", __func__, __FILE__);
	return 0;
}

struct file_operations buttom_drv_file_st = {
	.owner = THIS_MODULE,
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
	ret = 0;
	printk("%s:%s\n", __func__, __FILE__);
	major = register_chrdev(0, "buttom_drv", &buttom_drv_file_st);
	if (0 > ret) {
		ret = major;
		goto register_chrdev_error;
	}
	
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
			NULL, MKDEV(major, 0), NULL, "buttom_class_dev");

	if (IS_ERR(buttom_class_device)) {
		ret = PTR_ERR(buttom_class_device);
		goto class_device_create_error;
	}
	second_buttom_st.cpf_v = ioremap(GPFCON, 0x10);
	if (!second_buttom_st.cpf_v) {
		ret = -ENOMEM;
		goto ioremap_cpf_v_error;
	}

	second_buttom_st.cpg_v = ioremap(GPGCON, 0x10);
	if (!second_buttom_st.cpg_v) {
		ret = -ENOMEM;
		goto ioremap_cpg_v_error;
	}

	return 0;

	iounmap(second_buttom_st.cpg_v);
ioremap_cpg_v_error:
	iounmap(second_buttom_st.cpf_v);
ioremap_cpf_v_error:
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
	iounmap(second_buttom_st.cpg_v);
	iounmap(second_buttom_st.cpf_v);
	class_device_destroy(buttom_class_st, MKDEV(major, 0));
	class_destroy(buttom_class_st);
	unregister_chrdev(major, "sencond_drv");
	return;
}

module_init(buttom_drv_init);
module_exit(buttom_drv_exit);

MODULE_LICENSE("GPL");
