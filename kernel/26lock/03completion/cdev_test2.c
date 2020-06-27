#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/gpio.h>

struct mydev_st {
	char buf[256];
	struct cdev dev;
	dev_t no;
	atomic_t count;
	spinlock_t lock;
	struct semaphore sem;
	struct completion com;
	int irq;
};

struct mydev_st *mydev;

int my_open(struct inode *inode, struct file *fp)
{
	struct mydev_st *m = NULL;
#if 0
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)
#endif
	m = list_entry(inode->i_cdev, struct mydev_st, dev);
#if 0
	memset(m->buf, 0x00, sizeof(m->buf));
#endif
	atomic_add(1, &m->count);
	fp->private_data = m;
	
	printk("%s\n", __func__);
	printk("%s inode->i_cdev:%p\n", __func__, inode->i_cdev);
	return 0;
}

int my_release(struct inode *inode, struct file *fp)
{
	struct mydev_st *m = NULL;
	m = fp->private_data;
	atomic_sub(1, &m->count);
	printk("%s\n", __func__);
	return 0;
}

ssize_t my_read(struct file *fp, char __user *buff, size_t count, loff_t *off)
{
	struct mydev_st *m = NULL;
	int ret = 0;
#if 0
	unsigned long flags = 0;
#endif
	m = fp->private_data;
	/* 内核栈大小为2k，不要分配太变量在内核代码里 */
	count = (int)min((int)count, 256);
#if 0
	spin_lock_irqsave(&mydev->lock, flags);
#elif 0
	spin_lock(&mydev->lock);
#elif 0
	down(&m->sem);
#else
	ret = wait_for_completion_interruptible(&m->com);
	if (0 > ret) {
		return -EINTR;
	}
#endif
	printk("%s:m->buf:%p:%s\n", __func__, m->buf, m->buf);
	ret = copy_to_user(buff, m->buf, count);
	if (0 > ret) {
		ret = -EFAULT;
#if 0
		spin_unlock_irqrestore(&mydev->lock, flags);
#elif 0
		spin_unlock(&mydev->lock);
#elif 0
		up(&m->sem);
#endif
		goto copy_to_user_error;
	}
	printk("%s:m->buf:%p:%s\n", __func__, m->buf, m->buf);
#if 0
	spin_unlock_irqrestore(&mydev->lock, flags);
#elif 0
	spin_unlock(&mydev->lock);
#elif 0
	up(&m->sem);
#endif

	printk("%s\n", __func__);
	return count;
copy_to_user_error:
	return ret;
}

ssize_t my_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	struct mydev_st *m = NULL;
	int ret = 0;
#if 0
	unsigned long flags = 0;
#endif
	m = fp->private_data;
	count = min((int)count, 256);
	printk("%s\n", __func__);
#if 0
	spin_lock_irqsave(&mydev->lock, flags);
#elif 0
	down(&m->sem);
#endif
	ret = copy_from_user(m->buf, buff, count);
	if (0 > ret) {
		ret = -EFAULT;
#if 0
		spin_unlock_irqrestore(&mydev->lock, flags);
#else
		up(&m->sem);
#endif
		goto copy_from_user_error;
	}
	printk("%s:m->buf:%p:%s\n", __func__, m->buf, m->buf);
#if 0
	spin_unlock_irqrestore(&mydev->lock, flags);
#elif 0
	up(&m->sem);
#else
	complete(&m->com);
#endif
	return count;
copy_from_user_error:
	return ret;
}

long my_ioctl(struct file *fp, unsigned int i, unsigned long l)
{
	printk("%s\n", __func__);
	return 0;
}

struct file_operations fops = {
#if 1
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl,
#endif
};

#if 0
irq_handler_t handler,
typedef irqreturn_t (*irq_handler_t)(int, void *);
#endif

irqreturn_t gpio_irq_handler(int irq, void *data)
{
	struct mydev_st *m = NULL;
#if 0
	unsigned long flags = 0;
#endif
#if 0
	struct file *fp = NULL;
#endif
	m = data;
	/* 内核栈大小为2k，不要分配太变量在内核代码里 */
#if 0
	spin_lock_irqsave(&mydev->lock, flags);
#elif 0
	spin_lock(&mydev->lock);
#elif 0
	down(&m->sem);
#endif
	printk("%s:m->buf:%p:%s\n", __func__, m->buf, m->buf);
	memcpy(m->buf, "This is gpio_irq_handler write",
			sizeof("This is gpio_irq_handler write") - 1);
	printk("%s:m->buf:%p:%s\n", __func__, m->buf, m->buf);
#if 0
	fp = list_entry(data, struct file, private_data);
	atomic_long_add(sizeof("This is gpio_irq_handler write") - 1,
		&fp->f_count);
#endif
#if 0
	spin_unlock_irqrestore(&mydev->lock, flags);
#elif 0
	spin_unlock(&mydev->lock);
#elif 0
	up(&m->sem);
#else
	complete(&m->com);
#endif

	printk("%s\n", __func__);
	return IRQ_HANDLED;
}
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
	mydev->buf[0] = 'A';
	printk("%s:mydev->buf:%p:%s\n", __func__, mydev->buf, mydev->buf);

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

	atomic_set(&mydev->count, 0);
	spin_lock_init(&mydev->lock);
#if 0
static inline void sema_init(struct semaphore *sem, int val)
#endif
	sema_init(&mydev->sem, 1);
#if 0
static inline void init_completion(struct completion *x)
#endif
	init_completion(&mydev->com);
#if 0
static inline int gpio_to_irq(unsigned int gpio)
#endif
	mydev->irq = gpio_to_irq(EXYNOS4_GPX1(1));
#if 0
int request_irq(unsigned int irq,
		irq_handler_t handler,
		unsigned long flags, const char *devname, void *dev_id)
#endif

#if 0
	ret = request_irq(mydev->irq, gpio_irq_handler,
		IRQF_TRIGGER_FALLING, "my_button", (void *)mydev);
#else
# if 0
request_threaded_irq(unsigned int irq, irq_handler_t handler,
		     irq_handler_t thread_fn,
		     unsigned long flags, const char *name, void *dev);
# endif
	ret = request_threaded_irq(mydev->irq, NULL, gpio_irq_handler,
		IRQF_TRIGGER_FALLING, "my_button", (void *)mydev);
#endif
	if (ret) {
		ret = -ENODEV;
		goto request_irq_error;
	}
	return 0;
#if 0
void cdev_del(struct cdev *p)
#endif
request_irq_error:
	cdev_del(&mydev->dev);
cdev_add_error:
	unregister_chrdev_region(mydev->no, 3);
alloc_chrdev_region_error:
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
	kfree(mydev);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
