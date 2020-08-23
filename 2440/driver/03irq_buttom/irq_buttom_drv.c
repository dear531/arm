#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm-arm/gpio.h>

#define GPFCON 0x56000050
#define GPFDAT 0x56000054
#define GPGCON 0x56000060
#define GPGDAT 0x56000064


struct buttom_st {
void __iomem *cpf_v;
void __iomem *cpg_v;
};

struct buttom_st thrid_buttom_st;
static unsigned char key_val;
wait_queue_head_t wq;
static int ev_press = 0;
#if 0
GPF0
GPF2
GPG3
GPG11
#endif
ssize_t buttom_read(struct file *fp, char __user * buff, size_t count, loff_t * off)
{
	/* 暂时不考虑非阻塞情况 */
	int ret;
	ret = wait_event_interruptible(wq, ev_press);
	if (ret) {
		printk("wait_event_interruptible interrupt\n");
		return -EINTR;
	}
	if (copy_to_user(buff, &key_val, sizeof(key_val))) {
		printk("copy_to_user error\n");
		return -EFAULT;
	}
	ev_press = 0;
	return sizeof(key_val);
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

int buttom_close(struct inode *inode, struct file *fp)
{
	return 0;
}

struct file_operations irq_buttom_drv_file_st = {
	.owner = THIS_MODULE,
	.read = buttom_read,
	.write = buttom_write,
	.open = buttom_open,
	.release = buttom_close,
};

static unsigned int major;
struct class *buttom_class_st;
struct class_device *irq_buttom_class_device;

struct pin_desc {
	unsigned int pin;
	unsigned int key_val;
}
pins_desc[4] = {
	{S3C2410_GPF0, 0x01},
	{S3C2410_GPF2, 0x02},
	{S3C2410_GPG3, 0x03},
	{S3C2410_GPG11, 0x04},
};
irqreturn_t buttom_irq(int irq, void *data)
{
	struct pin_desc *desc;
	unsigned int pinval;
	desc = data;
	printk("%s:%d:%s:irq:%d\n", __FILE__, __LINE__, __func__, irq);
	pinval = s3c2410_gpio_getpin(desc->pin);
	if (pinval) {
	/* 按键按下为低电平 */
		key_val = 0x80 | desc->key_val;
	} else {
	/* 按键抬起为高电平 */
		key_val = desc->key_val;
	}
	ev_press = 1;
	wake_up(&wq);
	return IRQ_HANDLED;
}

static int __init irq_buttom_drv_init(void)
{
	int ret;
	ret = 0;
	printk("%s:%s\n", __func__, __FILE__);
	major = register_chrdev(0, "irq_buttom_drv", &irq_buttom_drv_file_st);
	if (0 > ret) {
		ret = major;
		goto register_chrdev_error;
	}
	
	buttom_class_st = class_create(THIS_MODULE, "irq_buttom_drv_class");
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
	irq_buttom_class_device = class_device_create(buttom_class_st,
			NULL, MKDEV(major, 0), NULL, "irq_buttom_class_dev");

	if (IS_ERR(irq_buttom_class_device)) {
		ret = PTR_ERR(irq_buttom_class_device);
		goto class_device_create_error;
	}
	thrid_buttom_st.cpf_v = ioremap(GPFCON, 0x10);
	if (!thrid_buttom_st.cpf_v) {
		ret = -ENOMEM;
		goto ioremap_cpf_v_error;
	}

	thrid_buttom_st.cpg_v = ioremap(GPGCON, 0x10);
	if (!thrid_buttom_st.cpg_v) {
		ret = -ENOMEM;
		goto ioremap_cpg_v_error;
	}
#if 0
int request_irq(unsigned int irq, irq_handler_t handler,
		unsigned long irqflags, const char *devname, void *dev_id)
#endif
	ret = request_irq(IRQ_EINT0, buttom_irq, IRQT_BOTHEDGE,"IRQ_EINT0", pins_desc);
	if (0 > ret) {
		printk("request_irq IRQ_EINT0 error\n");
		goto request_irq_IRQ_EINT0_error; 
	}
	ret = request_irq(IRQ_EINT2, buttom_irq, IRQT_BOTHEDGE,"IRQ_EINT2", pins_desc + 1);
	if (0 > ret) {
		printk("request_irq IRQ_EINT2 error\n");
		goto request_irq_IRQ_EINT2_error; 
	}
	ret = request_irq(IRQ_EINT11, buttom_irq, IRQT_BOTHEDGE,"IRQ_EINT11", pins_desc + 2);
	if (0 > ret) {
		printk("request_irq IRQ_EINT11 error\n");
		goto request_irq_IRQ_EINT11_error; 
	}
	ret = request_irq(IRQ_EINT19, buttom_irq, IRQT_BOTHEDGE,"IRQ_EINT19", pins_desc + 3);
	if (0 > ret) {
		printk("request_irq IRQ_EINT19 error\n");
		goto request_irq_IRQ_EINT19_error; 
	}
	init_waitqueue_head(&wq);

	return 0;

	free_irq(IRQ_EINT19, pins_desc + 3);
request_irq_IRQ_EINT19_error:
	free_irq(IRQ_EINT11, pins_desc + 2);
request_irq_IRQ_EINT11_error:
	free_irq(IRQ_EINT2, pins_desc + 1);
request_irq_IRQ_EINT2_error:
	free_irq(IRQ_EINT0, pins_desc);
request_irq_IRQ_EINT0_error:
	iounmap(thrid_buttom_st.cpg_v);
ioremap_cpg_v_error:
	iounmap(thrid_buttom_st.cpf_v);
ioremap_cpf_v_error:
	class_device_destroy(buttom_class_st, MKDEV(major, 0));
class_device_create_error:
	class_destroy(buttom_class_st);
class_create_error:
	unregister_chrdev(major, "thrid_drv");
register_chrdev_error:
	return ret;
}

static void __exit irq_buttom_drv_exit(void)
{
	free_irq(IRQ_EINT0, pins_desc + 0);
	free_irq(IRQ_EINT2, pins_desc + 1);
	free_irq(IRQ_EINT11, pins_desc + 2);
	free_irq(IRQ_EINT19, pins_desc + 3);
	iounmap(thrid_buttom_st.cpg_v);
	iounmap(thrid_buttom_st.cpf_v);
	class_device_destroy(buttom_class_st, MKDEV(major, 0));
	class_destroy(buttom_class_st);
	unregister_chrdev(major, "thrid_drv");
	return;
}

module_init(irq_buttom_drv_init);
module_exit(irq_buttom_drv_exit);

MODULE_LICENSE("GPL");
