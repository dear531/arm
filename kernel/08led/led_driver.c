#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/io.h>

struct led_st {
	int status;
	dev_t no;
	int count;
	int ledcon, leddat;
	void *__iomem v;
	struct cdev dev;
	void (*ledon)(struct led_st *l, int no);
	void (*ledoff)(struct led_st *l, int no);
};

struct led_pro_st {
	int led_no;
	int light;
	/* mapbit,位图表示 */
	int status;
};

struct led_st *led;

/* 硬件相关 */
#if 1
#define GPL2CON 0x0100
#define GPL2DAT 0x0104
#define GPK1CON 0x0060
#define GPK1DAT 0x0064
#endif
void exynos4_led_init(struct led_st *l)
{
	
	*(volatile unsigned int*)(l->v + GPL2CON) &= 0xfffffff0;
	*(volatile unsigned int*)(l->v + GPL2CON) |= 0x00000001;
	*(volatile unsigned int*)(l->v + GPL2DAT) |= (1 << 0);
	*(volatile unsigned int*)(l->v + GPK1CON) &= 0xffffff0f;
	*(volatile unsigned int*)(l->v + GPK1CON) |= 0x00000010;
	*(volatile unsigned int*)(l->v + GPK1DAT) |= (1 << 1);

}

void exynods4_led_exit(struct led_st *l)
{
	*(volatile unsigned int*)(l->v + GPL2CON) &= 0xfffffff0;
	*(volatile unsigned int*)(l->v + GPL2DAT) |= (1 << 0);
	*(volatile unsigned int*)(l->v + GPK1CON) &= 0xffffff0f;
	*(volatile unsigned int*)(l->v + GPK1DAT) |= (1 << 1);
}

void exynos4_led_on(struct led_st *l, int no)
{
	switch (no)
	{
		case 0:
			*(volatile unsigned int*)(l->v + GPL2DAT) |= (1 << 0);
			break;
		case 1:
			*(volatile unsigned int*)(l->v + GPK1DAT) |= (1 << 1);
			break;
		default:
			;
	}
}



void exynos4_led_off(struct led_st *l, int no)
{
	switch (no)
	{
		case 0:
			*(volatile unsigned int*)(l->v + GPL2DAT) &= ~(1 << 0);
			break;
		case 1:
			*(volatile unsigned int*)(l->v + GPK1DAT) &= ~(1 << 1);
			break;
		default:
			;
	}
}




/* 硬件无关 */


int led_open(struct inode *inode, struct file *fp)
{
	struct led_st *m = NULL;
#if 0
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)
#endif
	m = list_entry(inode->i_cdev, struct led_st, dev);

	/* fix:lock */
	if (1 == m->count) {
		/* fix:unlock */
		return -EBUSY;
	}
	m->count = 1;
	/* fix:unlock */

	fp->private_data = m;

	return 0;

	
	printk("%s\n", __func__);
	printk("%s inode->i_cdev:%p\n", __func__, inode->i_cdev);
	return 0;
}

int led_release(struct inode *inode, struct file *fp)
{
	struct led_st *m = NULL;

	m = fp->private_data;

	/* fix:lock */
	if (m->count == 0) {
		/* fix:unlock */
		return -ENODEV;
	}
	m->count = 0;
	/* fix:unlock */

	printk("%s\n", __func__);
	return 0;
}

ssize_t led_read(struct file *fp, char __user *buff, size_t count, loff_t *off)
{
	struct led_st *m = NULL;
	struct led_pro_st lps;
	int ret = 0;
	m = fp->private_data;
	lps.status = m->status;

	if (count != sizeof(lps)) {
		return -EINVAL;
	}

	ret = copy_to_user((void __user *)buff, &lps, sizeof(lps));
	if (0 > ret) {
		return -EFAULT;
	}
	return count;
}

ssize_t led_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	struct led_st *m = NULL;
	struct led_pro_st lps = {};
	int ret = 0;

	if (count != sizeof(lps)) {
		return -EINVAL;
	}

	ret = copy_from_user(&lps, buff, sizeof(lps));
	if (ret < 0) {
		return -EFAULT;
	}

	m = fp->private_data;

	if (lps.light) {
		m->ledon(m, lps.led_no); m->status |= (1 << lps.led_no);
	} else {
		m->ledoff(m, lps.led_no);
		m->status &= ~(1 << lps.led_no);
	}
	return count;

	printk("%s\n", __func__);
	return 0;
}

long led_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
#if 1
	struct led_st *l;

	l = fp->private_data;
	printk("l:%p\n", l);

	if (0 == cmd) {
		l->ledon(l, 0);
		l->ledon(l, 1);
	} else {
		l->ledoff(l, 0);
		l->ledoff(l, 1);
	}
#endif

	printk("%s\n", __func__);

	return 0;
}

struct file_operations fops = {
#if 1
	.open = led_open,
	.release = led_release,
	.read = led_read,
	.write = led_write,
	.unlocked_ioctl = led_ioctl,
#endif
};
/* when install then perform */
static __init int module_led_init(void)
{
	int ret = 0;
#if 0
static inline void *kzalloc(size_t size, gfp_t flags)
#endif
	led = kzalloc(sizeof(*led), GFP_KERNEL);
	if (NULL == led) {
		ret = -ENOMEM;
		goto kzalloc_error;
	}

#if 1
#if 0
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name)
#endif
	ret = alloc_chrdev_region(&led->no, 2, 3, "led_chardev");
#endif
	if (ret < 0) {
		goto alloc_chrdev_region_error;
	}
#if 0
void cdev_init(struct cdev *cdev, const struct file_operations *fops)
#endif
	cdev_init(&led->dev, &fops);
#if 0
int cdev_add(struct cdev *p, dev_t dev, unsigned count)
#endif
	ret = cdev_add(&led->dev, led->no, 3);
	if (ret < 0) {
		ret = -ENOMEM;
		goto cdev_add_error;
	}
	/**/
	led->v = ioremap(0x11000000, SZ_4K);
	if (led->v == NULL) {
		ret = -ENOMEM;
		goto ioremap_error;
	}

	led->ledon = exynos4_led_on;
	led->ledoff = exynos4_led_off;
	exynos4_led_init(led);
	
	printk("%s dev addr:%p\n", __func__, &led->dev);
	printk("major:%d, minor:%d\n", MAJOR(led->no), MINOR(led->no));

	return 0;
ioremap_error:
	cdev_del(&led->dev);
cdev_add_error:
	unregister_chrdev_region(led->no, 3);
alloc_chrdev_region_error:
	kfree(led);
kzalloc_error:
	return ret;
}

/* when uninstall then perfrom */
static __exit void module_led_exit(void)
{
	iounmap(led->v);
#if 0
void cdev_del(struct cdev *p)
#endif
	cdev_del(&led->dev);
#if 0
void unregister_chrdev_region(dev_t from, unsigned count)
#endif
	unregister_chrdev_region(led->no, 3);
	kfree(led);
	exynods4_led_exit(led);
}

module_init(module_led_init);
module_exit(module_led_exit);

MODULE_LICENSE("GPL");
