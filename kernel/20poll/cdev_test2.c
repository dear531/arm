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
#include <linux/gpio.h>
#include <linux/poll.h>

enum {
	NO_DATA,
	HAVE_DATA,
};

struct mydev_st {
	char buf[256];
	struct cdev dev;
	dev_t no;
	int count;
	int flag;
	int irq;
	spinlock_t lock;
	wait_queue_head_t rq;
};

struct mydev_st *mydev;
#if 0
typedef irqreturn_t (*irq_handler_t)(int, void *);
#endif

/* 当按键按下相当于有数据传入，整个过程充当了写者，将数据写入
 * 则不需要原来的写队列了，由中断处理函数实现
 */
irqreturn_t read_sensor(int irq, void *data)
{
	/* 如果读操作有睡眠，需要用工作队列处理中断下半部分 */

	struct mydev_st *m;
	static int a = 0;

	m = data;
	sprintf(m->buf, "%s:%d, %010d",
		__func__, __LINE__, a++);
	printk("%s:%d\n", __func__, __LINE__);
	m->flag = HAVE_DATA;
	wake_up(&m->rq);

	return IRQ_HANDLED;
}

int my_open(struct inode *inode, struct file *fp)
{
	struct mydev_st *m = NULL;
#if 0
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)
#endif
	m = list_entry(inode->i_cdev, struct mydev_st, dev);
	m->count++;

	memset(m->buf, 0x00, sizeof(m->buf));
	fp->private_data = m;
	

	printk("%s\n", __func__);
	printk("%s inode->i_cdev:%p\n", __func__, inode->i_cdev);

	return 0;
}

int my_release(struct inode *inode, struct file *fp)
{
	struct mydev_st *m = NULL;
	m = fp->private_data;

	m->count--;

	printk("%s\n", __func__);
	return 0;
}

/* /dev/test/mydev */
ssize_t my_read(struct file *fp, char __user *buff, size_t count, loff_t *off)
{
	struct mydev_st *m = NULL;
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
}

unsigned int my_poll(struct file *fp, struct poll_table_struct *table)
{
	unsigned int mask = 0;

	struct mydev_st *m = NULL;
	m = fp->private_data; 
	/* 1.把等待队列放到table */
#if 0
static inline void poll_wait(struct file * filp, wait_queue_head_t * wait_address, poll_table *p)
#endif
	poll_wait(fp, &m->rq, table);
	/* 2.检测硬件的状态（都写）*/
	if (m->flag == HAVE_DATA) {
		mask |= POLLIN;
	}
	/* 3.返回相应的状态 */
	return mask;
}

struct file_operations fops = {
#if 1
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.poll = my_poll,
#endif
};
/* when install then perform */
static __init int module_test1_init(void)
{
	int ret = 0;
#if 0
static inline void *kzalloc(size_t size, gfp_t flags)
#endif
	mydev = kzalloc(sizeof(*mydev), GFP_KERNEL);
	if (NULL == mydev) {
		ret = -ENOMEM;
		goto kzalloc_error;
	}

	mydev->flag = NO_DATA;

	spin_lock_init(&mydev->lock);
	init_waitqueue_head(&mydev->rq);

	mydev->irq = gpio_to_irq(EXYNOS4_GPX1(1));

#if 0
int request_irq(unsigned int irq,
		irq_handler_t handler,
		unsigned long flags, const char *devname, void *dev_id)
#endif

	ret = request_irq(mydev->irq, read_sensor,
		IRQF_TRIGGER_FALLING,
		"sensor", mydev);
	if (0 > ret) {
		goto request_irq_error;
	}
#if 1
#if 0
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name)
#endif
	ret = alloc_chrdev_region(&mydev->no, 2, 3, "my_chardev");
#endif
	if (ret < 0) {
		goto alloc_chrdev_region_error;
	}
#if 0
void cdev_init(struct cdev *cdev, const struct file_operations *fops)
#endif
	cdev_init(&mydev->dev, &fops);
#if 0
int cdev_add(struct cdev *p, dev_t dev, unsigned count)
#endif
	ret = cdev_add(&mydev->dev, mydev->no, 3);
	if (ret < 0) {
		ret = -ENOMEM;
		goto cdev_add_error;
	}
	
	printk("%s dev addr:%p\n", __func__, &mydev->dev);
	printk("major:%d, minor:%d\n", MAJOR(mydev->no), MINOR(mydev->no));

	return 0;
cdev_add_error:
	unregister_chrdev_region(mydev->no, 3);
alloc_chrdev_region_error:
#if 0
void free_irq(unsigned int irq, void *dev_id)
#endif
	free_irq(mydev->irq, mydev);
request_irq_error:
	kfree(mydev);
kzalloc_error:
	return ret;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
#if 0
void cdev_del(struct cdev *p)
#endif
	cdev_del(&mydev->dev);
#if 0
void unregister_chrdev_region(dev_t from, unsigned count)
#endif
	unregister_chrdev_region(mydev->no, 3);
	free_irq(mydev->irq, mydev);
	kfree(mydev);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
