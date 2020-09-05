#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm-arm/gpio.h>
#include <linux/poll.h>

#define GPFCON 0x56000050
#define GPFDAT 0x56000054
#define GPGCON 0x56000060
#define GPGDAT 0x56000064


struct pin_desc {
	unsigned int pin;
	unsigned int key_val;
};

struct button_st {
	void __iomem *cpf_v;
	void __iomem *cpg_v;
	unsigned char key_val;
	wait_queue_head_t wq;
	int ev_press;
	unsigned int major;
	struct class *btn_cls;
	struct class_device *btn_cls_dev;
	struct pin_desc pins_desc[4];
	struct fasync_struct *fasync_queue;
	atomic_t t;
};

static struct button_st *btn;

#if 0
GPF0
GPF2
GPG3
GPG11
#endif

ssize_t button_read(struct file *fp, char __user * buff, size_t count, loff_t * off)
{
	/* 暂时不考虑非阻塞情况 */
	int ret;
	ret = wait_event_interruptible(btn->wq, btn->ev_press);
	if (ret) {
		printk("wait_event_interruptible interrupt\n");
		return -EINTR;
	}
	if (copy_to_user(buff, &btn->key_val, sizeof(btn->key_val))) {
		printk("copy_to_user error\n");
		return -EFAULT;
	}
	btn->ev_press = 0;
	return sizeof(btn->key_val);
}

ssize_t button_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	return 0;
}

int button_open(struct inode *inode, struct file *fp)
{
	printk("%s:%s\n", __func__, __FILE__);
	fp->private_data = btn;
	if (!atomic_dec_and_test(&btn->t)) {
		atomic_inc(&btn->t);
		return -EBUSY;
	}
	return 0;
}

int button_close(struct inode *inode, struct file *fp)
{

	atomic_inc(&btn->t);
	return 0;
}

unsigned int button_poll(struct file *fp, struct poll_table_struct *table)
{
	static struct button_st *b;
	unsigned int mask = 0;

	b = fp->private_data;
	poll_wait(fp, &b->wq, table);
	if (1 == btn->ev_press)
		mask |= POLLIN;
	return mask;
}

/* 当应用程序调用 */
static int button_fasync(int fd, struct file *file, int on)
{       
        return fasync_helper(fd, file, on, &btn->fasync_queue); 
}

struct file_operations atomic_button_file_st = {
	.owner = THIS_MODULE,
	.read = button_read,
	.write = button_write,
	.open = button_open,
	.release = button_close,
	.poll = button_poll,
	.fasync = button_fasync,
};

irqreturn_t button_irq(int irq, void *data)
{
	struct pin_desc *desc;
	unsigned int pinval;
	desc = data;
	printk("%s:%d:%s:irq:%d\n", __FILE__, __LINE__, __func__, irq);
	pinval = s3c2410_gpio_getpin(desc->pin);
	if (pinval) {
	/* 按键按下为低电平 */
		btn->key_val = 0x80 | desc->key_val;
	} else {
	/* 按键抬起为高电平 */
		btn->key_val = desc->key_val;
	}
	btn->ev_press = 1;
	wake_up(&btn->wq);
	/* SIGIO信号表示io上有数据 */
	/* 设置当有数据时发送SIGIO信号 */
	kill_fasync(&btn->fasync_queue, SIGIO, POLL_IN);
	return IRQ_HANDLED;
}

static int __init atomic_button_init(void)
{
	int ret;
	ret = 0;
	printk("%s:%s\n", __func__, __FILE__);
	btn = kzalloc(sizeof(*btn), GFP_KERNEL);
	if (!btn) {
		printk("kzalloc error\n");
		ret = -ENOMEM;
		goto kzalloc_error;
	}
	btn->ev_press = 0;
	btn->pins_desc[0].pin = S3C2410_GPF0;
	btn->pins_desc[0].key_val = 0x01;
	btn->pins_desc[1].pin = S3C2410_GPF2;
	btn->pins_desc[1].key_val = 0x02;
	btn->pins_desc[2].pin = S3C2410_GPG3;
	btn->pins_desc[2].key_val = 0x03;
	btn->pins_desc[3].pin = S3C2410_GPG11;
	btn->pins_desc[3].key_val = 0x04;

	btn->major = register_chrdev(0, "atomic_button", &atomic_button_file_st);
	if (0 > ret) {
		ret = btn->major;
		goto register_chrdev_error;
	}
	
	btn->btn_cls = class_create(THIS_MODULE, "atomic_button_class");
	if (IS_ERR(btn->btn_cls)) {
		ret = PTR_ERR(btn->btn_cls);
		goto class_create_error;
	}
#if 0
struct class_device *class_device_create(struct class *cls,
					 struct class_device *parent,
					 dev_t devt,
					 struct device *device,
					 const char *fmt, ...)
#endif
	btn->btn_cls_dev = class_device_create(btn->btn_cls,
			NULL, MKDEV(btn->major, 0), NULL, "async_class_dev");

	if (IS_ERR(btn->btn_cls_dev)) {
		ret = PTR_ERR(btn->btn_cls_dev);
		goto class_device_create_error;
	}
	btn->cpf_v = ioremap(GPFCON, 0x10);
	if (!btn->cpf_v) {
		ret = -ENOMEM;
		goto ioremap_cpf_v_error;
	}

	btn->cpg_v = ioremap(GPGCON, 0x10);
	if (!btn->cpg_v) {
		ret = -ENOMEM;
		goto ioremap_cpg_v_error;
	}
	ret = request_irq(IRQ_EINT0, button_irq, IRQT_BOTHEDGE,"IRQ_EINT0", btn->pins_desc);
	if (0 > ret) {
		printk("request_irq IRQ_EINT0 error\n");
		goto request_irq_IRQ_EINT0_error; 
	}
	ret = request_irq(IRQ_EINT2, button_irq, IRQT_BOTHEDGE,"IRQ_EINT2", btn->pins_desc + 1);
	if (0 > ret) {
		printk("request_irq IRQ_EINT2 error\n");
		goto request_irq_IRQ_EINT2_error; 
	}
	ret = request_irq(IRQ_EINT11, button_irq, IRQT_BOTHEDGE,"IRQ_EINT11", btn->pins_desc + 2);
	if (0 > ret) {
		printk("request_irq IRQ_EINT11 error\n");
		goto request_irq_IRQ_EINT11_error; 
	}
	ret = request_irq(IRQ_EINT19, button_irq, IRQT_BOTHEDGE,"IRQ_EINT19", btn->pins_desc + 3);
	if (0 > ret) {
		printk("request_irq IRQ_EINT19 error\n");
		goto request_irq_IRQ_EINT19_error; 
	}
	init_waitqueue_head(&btn->wq);
	atomic_set(&btn->t, 1);

	return 0;

	free_irq(IRQ_EINT19, btn->pins_desc + 3);
request_irq_IRQ_EINT19_error:
	free_irq(IRQ_EINT11, btn->pins_desc + 2);
request_irq_IRQ_EINT11_error:
	free_irq(IRQ_EINT2, btn->pins_desc + 1);
request_irq_IRQ_EINT2_error:
	free_irq(IRQ_EINT0, btn->pins_desc);
request_irq_IRQ_EINT0_error:
	iounmap(btn->cpg_v);
ioremap_cpg_v_error:
	iounmap(btn->cpf_v);
ioremap_cpf_v_error:
	class_device_destroy(btn->btn_cls, MKDEV(btn->major, 0));
class_device_create_error:
	class_destroy(btn->btn_cls);
class_create_error:
	unregister_chrdev(btn->major, "atomic_button");
register_chrdev_error:
	kfree(btn);
	btn = NULL;
kzalloc_error:
	return ret;
}

static void __exit atomic_button_exit(void)
{
	free_irq(IRQ_EINT0, btn->pins_desc + 0);
	free_irq(IRQ_EINT2, btn->pins_desc + 1);
	free_irq(IRQ_EINT11, btn->pins_desc + 2);
	free_irq(IRQ_EINT19, btn->pins_desc + 3);
	iounmap(btn->cpg_v);
	iounmap(btn->cpf_v);
	class_device_destroy(btn->btn_cls, MKDEV(btn->major, 0));
	class_destroy(btn->btn_cls);
	unregister_chrdev(btn->major, "atomic_button");
	return;
}

module_init(atomic_button_init);
module_exit(atomic_button_exit);

MODULE_LICENSE("GPL");
