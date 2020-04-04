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

#if 1
#define WDT_BASE	0x10060000
#define WTCON		0x0000
#define WTDAT		0x0004
#define WTCNT		0x0008
#define WTCLRINT	0x000C
#endif

struct wdt_st {
	int wtcon, wtdat, wdtcnt;
	void *__iomem v;
	struct clk *clk;
	dev_t no;
	struct cdev dev;
	spinlock_t lock;
	int count;
} *wdt = NULL;

struct wdt_feed_st {
	int time;
};

void exynos4_wdt_init(struct wdt_st *w)
{
	w->wtcon = (99 << 8) | (2 << 3) | 1;
	iowrite32(w->wtcon, w->v + WTCON);

	w->wdtcnt = 15625 * 3;
	iowrite32(w->wdtcnt, w->v + WTCNT);

	return;
}

void exynos4_wdt_exit(struct wdt_st *w)
{
	return;
}

void exynos4_wdt_enable(struct wdt_st *w)
{
	w->wtcon = ioread32(w->v + WTCON);
	w->wtcon |= 1 << 5;
	iowrite32(w->wtcon, w->v + WTCON);

	return;
}

void exynos4_wdt_disable(struct wdt_st *w)
{
	w->wtcon = ioread32(w->v + WTCON);
	w->wtcon &= ~(1 << 5);
	iowrite32(w->wtcon, w->v + WTCON);

	return;
}

void exynos4_feed_wdt(struct wdt_st *w, int ms)
{
	w->wdtcnt = 15625 * ms / 1000;
	iowrite32(w->wdtcnt, w->v + WTCNT);

	return;
}

ssize_t wdt_write(struct file *fp, const char __user *buff, size_t count, loff_t *off)
{
	struct wdt_feed_st wfs = {};
	struct wdt_st *w = NULL;
	int ret = 0;

	if (count != sizeof(wfs)) {
		return -EINVAL;
	}
#if 0
static inline int copy_from_user(void *to, const void __user volatile *from, unsigned long n)
#endif
	ret = copy_from_user(&wfs, buff, count);
	if (0 > ret) {
		goto copy_from_user_error;
	}

	w = fp->private_data;
	exynos4_feed_wdt(w, wfs.time);

	return count;

copy_from_user_error:
	return ret;
}

int wdt_open(struct inode *inode, struct file *fp)
{
#if 0
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)
#endif
	struct wdt_st *w = NULL;
	spin_lock_init(&wdt->lock);
	w = list_entry(inode->i_cdev, struct wdt_st, dev);
	fp->private_data = w;

	spin_lock(&wdt->lock);
	if (w->count) {	
		spin_unlock(&wdt->lock);
		return -EBUSY;
	}

	w->count = 1;
	spin_unlock(&wdt->lock);

	return 0;
}

int wdt_release(struct inode *inode, struct file *fp)
{
	struct wdt_st *w = NULL;
	w = fp->private_data;

	spin_lock(&wdt->lock);
	if (!w->count) {	
		spin_unlock(&wdt->lock);
		return -ENODEV;
	}

	w->count = 0;
	spin_unlock(&wdt->lock);

	fp->private_data = NULL;
	return 0;
}

long wdt_unlocked_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	struct wdt_st *w = NULL;
	w = fp->private_data;
	switch (cmd) {
		case 0:
			exynos4_wdt_enable(w);
			break;
		case 1:
			exynos4_wdt_disable(w);
			break;
		default:
			break;
	}

	return 0;
}


struct file_operations fps = {
#if 0
	int (*open) (struct inode *, struct file *);
	int (*release) (struct inode *, struct file *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
#endif
	.open = wdt_open,
	.release = wdt_release,
	.unlocked_ioctl = wdt_unlocked_ioctl,
	.write = wdt_write,
};

static __init int wdt_init(void)
{
	int ret = 0;
	wdt = kzalloc(sizeof(*wdt), GFP_KERNEL);
	if (!wdt) {
		ret = -ENOMEM;
		goto kzalloc_error;
	}
#if 0
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name)
#endif

	ret = alloc_chrdev_region(&wdt->no, 1, 1, "mywdt");
	if (0 > ret) {
		goto alloc_chrdev_region_error;
	}

	cdev_init(&wdt->dev, &fps);

	ret = cdev_add(&wdt->dev, wdt->no, 1);
	if (0 > ret) {
		goto cdev_add_error;
	}
	printk("finished cdev_add\n");
#if 1
	wdt->v = ioremap(WDT_BASE, SZ_4K);
	if (NULL == wdt->v) {
		ret = -ENOMEM;
		printk("ioremap error\n");
		goto ioremap_error;
	}
	printk("wdt->v:%p\n", wdt->v);
#endif
#if 1
	wdt->clk = clk_get(NULL, "watchdog");

	if (IS_ERR(wdt->clk)) {
#if 0
                dev_err(NULL, "failed to find watchdog clock source\n");
#endif
                ret = PTR_ERR(wdt->clk);
		goto clk_get_error; 
	}
#else
        wdt->clk = devm_clk_get(NULL, "watchdog");
        if (IS_ERR(wdt->clk)) {
                dev_err(NULL, "failed to find watchdog clock source\n");
                ret = PTR_ERR(wdt->clk);
		goto clk_get_error; 
        }  
#endif

	ret = clk_enable(wdt->clk);
	if (0 > ret) {
		goto clk_enable_error;
	}

	exynos4_wdt_init(wdt);

	return 0;

clk_enable_error:
	clk_put(wdt->clk);
clk_get_error:
	iounmap(wdt->v);
ioremap_error:
	cdev_del(&wdt->dev);
cdev_add_error:
	unregister_chrdev_region(wdt->no, 1);
alloc_chrdev_region_error:
	kfree(wdt);
kzalloc_error:
	return ret;
}

static __exit void wdt_exit(void)
{
	exynos4_wdt_exit(wdt);
	clk_disable(wdt->clk);
	clk_put(wdt->clk);
	iounmap(wdt->v);
	cdev_del(&wdt->dev);
	unregister_chrdev_region(wdt->no, 1);
	kfree(wdt);
	return;
}

module_init(wdt_init);
module_exit(wdt_exit);
MODULE_LICENSE("GPL");
