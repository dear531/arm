#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct mydev_st {
	char buf[256];
	struct cdev dev;
	dev_t no;
	int count;
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
	memset(m->buf, 0x00, sizeof(m->buf));
	m->count++;
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

ssize_t my_read(struct file *fp, char __user *buff, size_t count, loff_t *off)
{
	struct mydev_st *m = NULL;
	int ret = 0;
	m = fp->private_data;
	/* 内核栈大小为2k，不要分配太变量在内核代码里 */
	count = (int)min((int)count, 256);
	ret = copy_to_user(buff, m->buf, count);
	if (0 > ret) {
		ret = -EFAULT;
		goto copy_to_user_error;
	}

	printk("%s\n", __func__);
	return count;
copy_to_user_error:
	return ret;
}

ssize_t my_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	struct mydev_st *m = NULL;
	int ret = 0;
	m = fp->private_data;
	count = min((int)count, 256);
	printk("%s\n", __func__);
	ret = copy_from_user(m->buf, buff, count);
	if (0 > ret) {
		ret = -EFAULT;
		goto copy_from_user_error;
	}
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
