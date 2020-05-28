#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

#include <linux/slab.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/delay.h>
#include <linux/poll.h>

#include "exynos4_adc.h"

struct adc_st *adc = NULL;

void adc_start(struct adc_st *a)
{
	a->adccon = ioread32(a->v + ADCCON);
	a->adccon |= 1;
	iowrite32(a->adccon, a->v + ADCCON);
}

void wait_for_adc(struct adc_st *a)
{
	while (0 == (ioread32(a->v + ADCCON) & (1 << 15)));

}

int get_adc(struct adc_st *a)
{
	return ioread32(a->v + ADCDAT) & 0xfff;
}

ssize_t adc_read(struct file *fp, char __user *buff, size_t count, loff_t *off)
{
	struct adc_st *a = NULL;
	int ret = 0;
	struct adc_value_st avs;

	a = fp->private_data;
	if (count != sizeof(avs)) {
		return -EINVAL;
	}

	while (a->flag == NO_DATA) {
		/* 如果是非阻塞访问时没有数据，则返回再读一次 */
		if (fp->f_flags & O_NONBLOCK) {
			return -EAGAIN;
		}
		/* 如果是阻塞访问则将进程投入睡眠 */
		ret = wait_event_interruptible(adc->rq, a->flag == HAVE_DATA);
		if (ret < 0) {
			/* 如果睡眠返回时，不是以非零返回，则说明是被用户发出信号中断 */
			return -EINTR;
		}
	}
#if 0
	wait_for_adc(a);
#endif
	/* 当循环结束时，则说明转换结束有数据，并被中断唤醒了进程 */
	avs.value = a->value;
	printk("%s:%d, avs.value:%d\n",
		__func__, __LINE__, avs.value);
	ret = copy_to_user(buff, &avs, sizeof(avs));
	if (ret) {
		return -EFAULT;
	}
	a->flag = NO_DATA;
	return count;
}

long adc_ioctl(struct file *fp , unsigned int cmd, unsigned long flag)
{
	struct adc_st *a = NULL;
	a = fp->private_data;
	/* 用ioctl函数控制数据生成，且该操作步骤直接操作硬件生成数据，所以可以和都写在一起，也可以分离 */
	adc_start(a);
	return 0;
}
unsigned int adc_poll(struct file *fp, struct poll_table_struct *tb)
{
	int mask = 0;
	struct adc_st *a = NULL;
	a = fp->private_data;

	poll_wait(fp, &a->rq, tb);

	if (a->flag == HAVE_DATA) {
		mask |= POLLIN;
	}

	return mask;
}

int adc_open(struct inode *inode, struct file *fp)
{
	struct adc_st *a = NULL;
	a = list_entry(inode->i_cdev, struct adc_st, dev);
	a->count++;

	fp->private_data = a;

	printk("%s\n", __func__);
	printk("%s inode->i_cdev:%p\n", __func__, inode->i_cdev);

	return 0;
}

int adc_release(struct inode *inode, struct file *fp)
{
	struct adc_st *a = NULL;
	a = fp->private_data;

	a->count--;

	printk("%s\n", __func__);
	return 0;
}

void exynos4_clrintadc(struct adc_st *a)
{
	iowrite32(1, a->v + CLRINTADC);
	return;
}

void exynos4_adc_init(struct adc_st *a)
{
	a->adccon = (1 << 16) | (1 << 14) | (25 << 6);
	iowrite32(a->adccon, a->v + ADCCON);

	a->adcdly = 0xffff;
	iowrite32(a->adcdly, a->v + ADCDLY);

	a->adcmux = 0;
	iowrite32(a->adcmux, a->v + ADCMUX);

	a->iesr2 = ioread32(a->v_iesr + IESR2);
	a->iesr2 |= 1 << 19;
	iowrite32(a->iesr2, a->v_iesr + IESR2);
}

irqreturn_t irq_adc_handle(int irq, void *data)
{
	struct adc_st *a = data;

	/* 取得转换后的值 */
	a->value = get_adc(a);
	/* 设置有数据标志位 */
	a->flag = HAVE_DATA;
	/* 唤醒等待队列来读数据 */
	wake_up(&a->rq);
	/* 清中断 */
	exynos4_clrintadc(a);

	printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	return IRQ_HANDLED;
}

void exynos_adc_exit(struct adc_st *a)
{
	return;
}

static __init int module_adc_init(void)
{
	int ret = 0;
	adc = kzalloc(sizeof(*adc), GFP_KERNEL);
	if (!adc) {
		ret = -ENOMEM;
		goto kzalloc_error;
	}
	adc->flag = NO_DATA;

	ret = alloc_chrdev_region(&adc->no, 1, 1, "myadc");
	if (ret) {
		ret = -ENOMEM;
		goto alloc_chrdev_region_error;
	}

	adc->adc_ops.open = adc_open;
	adc->adc_ops.release = adc_release;
	adc->adc_ops.read = adc_read;
	adc->adc_ops.poll = adc_poll;
	adc->adc_ops.unlocked_ioctl= adc_ioctl;

	cdev_init(&adc->dev, &adc->adc_ops);

	ret = cdev_add(&adc->dev, adc->no, 1);
	if (ret) {
		ret = -ENOMEM;
		goto cdev_add_error;
	}

	/* arch/arm/mach-exynos/clock-exynos4.c */
	adc->clk = clk_get(NULL, "adc");
	if (!adc->clk) {
		ret =-ENOMEM;
		goto clk_get_error;
	}
	ret = clk_enable(adc->clk);
	if (ret) {
		ret = -ENODEV;
		goto clk_enable_error;
	}

	init_waitqueue_head(&adc->rq);

	adc->v = ioremap(EXYNOS4_PA_ADC, EXYNOS4_SZ_ADC);
	if (!adc->v) {
		ret = -ENOMEM;
		goto ioremap_error;
	}

	adc->v_iesr = ioremap(EXYNOS_PA_IESR, EXYNOS4_SZ_IESR);
	if (!adc->v_iesr) {
		ret = -ENOMEM;
		goto ioremap_iesr_error;
	}


	/* rch/arm/mach-exynos/include/mach/irqs-exynos4.h */
	adc->irq = IRQ_ADC;

#if 0
int request_irq(unsigned int irq,
		irq_handler_t handler,
		unsigned long flags, const char *devname, void *dev_id)
#endif
	/* myadc:/proc/interrupts */
	ret = request_irq(adc->irq, irq_adc_handle, 0, "myadc", adc);
	if (ret) {
		ret = -ENODEV;
		goto request_irq_error;
	}

	exynos4_adc_init(adc);

	return 0;
	free_irq(adc->irq, adc);
request_irq_error:
	iounmap(adc->v_iesr);
ioremap_iesr_error:
	iounmap(adc->v);
ioremap_error:
	clk_disable(adc->clk);
clk_enable_error:
	clk_put(adc->clk);
clk_get_error:
	cdev_del(&adc->dev);
cdev_add_error:
	unregister_chrdev_region(adc->no, 1);
alloc_chrdev_region_error:
	kfree(adc);
kzalloc_error:
	return ret;
}

static __exit void module_adc_exit(void)
{
	exynos_adc_exit(adc);
	free_irq(adc->irq, adc);
	iounmap(adc->v_iesr);
	iounmap(adc->v);
	clk_disable(adc->clk);
	clk_put(adc->clk);
	cdev_del(&adc->dev);
	unregister_chrdev_region(adc->no, 1);
	kfree(adc);
	return;
}

module_init(module_adc_init);
module_exit(module_adc_exit);

MODULE_LICENSE("GPL");
