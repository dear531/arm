#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/io.h>

#define ADC_BASE	0x126C0000
#define ADCCON		0x0000
#define ADCDLY		0x0008
#define ADCDAT		0x000C
#define CLRINTADC	0x0018
#define ADCMUX		0x001C

struct adc_value_st {
	int value;
};

enum {
	NO_DATA,
	HAVE_DATA,
};

struct adc_st {
	char buf[256];
	struct cdev dev;
	dev_t no;
	int count;
	int flag;
	int irq;
	spinlock_t lock;
	wait_queue_head_t rq;
	struct clk *clk;
	void __iomem *v;
	u32 adccon, adcdly, adcdat, clrintadc, adcmux;
};

struct adc_st *adc;
#if 0
typedef irqreturn_t (*irq_handler_t)(int, void *);
#endif

/* 当按键按下相当于有数据传入，整个过程充当了写者，将数据写入
 * 则不需要原来的写队列了，由中断处理函数实现
 */
irqreturn_t read_sensor(int irq, void *data)
{
#if 0
	/* 如果读操作有睡眠，需要用工作队列处理中断下半部分 */

	struct adc_st *m;
	static int a = 0;

	m = data;
	sprintf(m->buf, "%s:%d, %010d",
		__func__, __LINE__, a++);
	printk("%s:%d\n", __func__, __LINE__);
	m->flag = HAVE_DATA;
	wake_up(&m->rq);
#endif

	return IRQ_HANDLED;
}


int adc_open(struct inode *inode, struct file *fp)
{
	struct adc_st *m = NULL;
#if 0
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)
#endif
	m = list_entry(inode->i_cdev, struct adc_st, dev);
	m->count++;

	memset(m->buf, 0x00, sizeof(m->buf));
	fp->private_data = m;
	

	printk("%s\n", __func__);
	printk("%s inode->i_cdev:%p\n", __func__, inode->i_cdev);

	return 0;
}

int adc_release(struct inode *inode, struct file *fp)
{
	struct adc_st *m = NULL;
	m = fp->private_data;

	m->count--;

	printk("%s\n", __func__);
	return 0;
}

void adc_start(struct adc_st *adc)
{
	adc->adccon = ioread32(adc->v + ADCCON);
	adc->adccon |= 1;
	iowrite32(adc->adccon, adc->v + ADCCON);
}

void wait_for_adc(struct adc_st *adc)
{
	while (0 == (ioread32(adc->v + ADCCON) & (1 << 15)));

}

int get_adc(struct adc_st *adc)
{
	return ioread32(adc->v + ADCDAT) & 0xfff;
}

/* /dev/test/adc */
ssize_t adc_read(struct file *fp, char __user *buff, size_t count, loff_t *off)
{
#if  0
	struct adc_st *m = NULL;
	int ret = 0;
	printk("%s:%d\n", __func__, __LINE__);
	m = fp->private_data;
	while (m->flag == NO_DATA) {
		if (fp->f_flags & O_NONBLOCK) {
		/*  nonblock */
			return -EAGAIN;
		} else {
		/* block */
			/* 等待队列，待继续 */
#if 0
			/* signal can't interrupt the queue */
			wait_event(m->rq, m->flag == HAVE_DATA);
#else
			/* signal can interrupt the queue */
			ret = wait_event_interruptible(m->rq, m->flag == HAVE_DATA);
			if (0 > ret) {
				return -EINTR;
			}
#endif
		}
	}
	/* 内核栈大小为2k，不要分配太变量在内核代码里 */
	count = (int)min((int)count, 256);

	ret = copy_to_user(buff, m->buf, count);
	if (0 > ret) {
		ret = -EFAULT;
		goto copy_to_user_error;
	}

	m->flag = NO_DATA;

	printk("%s\n", __func__);
	return count;
copy_to_user_error:
	return ret;
#else
	struct adc_st *m = NULL;
	int ret = 0;
	struct adc_value_st avs;

	m = fp->private_data;
	if (count != sizeof(avs)) {
		return -EINVAL;
	}
	adc_start(m);
	wait_for_adc(m);
	avs.value = get_adc(m);
	printk("%s:%d, avs.value:%d\n",
		__func__, __LINE__, avs.value);
	ret = copy_to_user(buff, &avs, sizeof(avs));
	if (ret) {
		return -EFAULT;
	}
	return count;
#endif
}

struct file_operations fops = {
#if 1
	.open = adc_open,
	.release = adc_release,
	.read = adc_read,
#endif
};

void adc_init(struct adc_st *adc)
{
	adc->adccon = (1 << 16) | (1 << 14) | (25 << 6);
	iowrite32(adc->adccon, adc->v + ADCCON);

	adc->adcdly = 0xffff;
	iowrite32(adc->adcdly, adc->v + ADCDLY);

	adc->adcmux = 0;
	iowrite32(adc->adcmux, adc->v + ADCMUX);
}
/* when install then perform */
static __init int module_test1_init(void)
{
	int ret = 0;
#if 0
static inline void *kzalloc(size_t size, gfp_t flags)
#endif
	adc = kzalloc(sizeof(*adc), GFP_KERNEL);
	if (NULL == adc) {
		ret = -ENOMEM;
		goto kzalloc_error;
	}

	adc->flag = NO_DATA;

	spin_lock_init(&adc->lock);
	init_waitqueue_head(&adc->rq);

	adc->clk = clk_get(NULL, "adc");
	if (!adc->clk) {
		printk("clk_get adc oerror\n");
		goto clk_error;
		ret = -ENODEV;
	}

	clk_enable(adc->clk);

	printk("%s:%d\n", __func__, __LINE__);

#if 0
static inline void __iomem *ioremap(unsigned long port, unsigned long size)
#endif
	adc->v = ioremap(ADC_BASE, SZ_4K);
	if (!adc->v) {
		printk("ioremap adc error\n");
		goto ioremap_error;
		ret = -ENOMEM;
	}
	adc->adccon = ioread32(adc->v);
	printk("%s, %d, adc->adccon:%d\n",
		__func__, __LINE__, adc->adccon);
	adc_init(adc);
#if 0

	adc->irq = IRQ_ADC;

#if 0
int request_irq(unsigned int irq,
		irq_handler_t handler,
		unsigned long flags, const char *devname, void *dev_id)
#endif
	ret = request_irq(adc->irq, read_sensor,
		0, "myadc", adc);
	if (0 > ret) {
		goto request_irq_error;
	}
#endif
#if 1
#if 0
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name)
#endif
	ret = alloc_chrdev_region(&adc->no, 2, 3, "myadc");
#endif
	if (ret < 0) {
		goto alloc_chrdev_region_error;
	}
#if 0
void cdev_init(struct cdev *cdev, const struct file_operations *fops)
#endif
	cdev_init(&adc->dev, &fops);
#if 0
int cdev_add(struct cdev *p, dev_t dev, unsigned count)
#endif
	ret = cdev_add(&adc->dev, adc->no, 3);
	if (ret < 0) {
		ret = -ENOMEM;
		goto cdev_add_error;
	}
	
	printk("%s dev addr:%p\n", __func__, &adc->dev);
	printk("major:%d, minor:%d\n", MAJOR(adc->no), MINOR(adc->no));

	return 0;
cdev_add_error:
	unregister_chrdev_region(adc->no, 3);
alloc_chrdev_region_error:
#if 0
	free_irq(adc->irq, adc);
request_irq_error:
#endif
	iounmap(adc->v);
ioremap_error:
	clk_disable(adc->clk);
	clk_put(adc->clk);
clk_error:
	kfree(adc);
kzalloc_error:
	return ret;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
#if 0
void cdev_del(struct cdev *p)
#endif
	cdev_del(&adc->dev);
#if 0
void unregister_chrdev_region(dev_t from, unsigned count)
#endif
	unregister_chrdev_region(adc->no, 3);
#if 0
	free_irq(adc->irq, adc);
#endif
	kfree(adc);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
