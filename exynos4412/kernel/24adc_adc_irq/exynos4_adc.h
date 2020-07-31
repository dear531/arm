#ifndef __EXYNOS4_ADC__
#define __EXYNOS4_ADC__
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/wait.h>
#include <linux/clkdev.h>

#define EXYNOS4_PA_ADC	0x126C0000
#define EXYNOS4_SZ_ADC	SZ_4K
#define ADCCON		0x0000
#define ADCDLY		0x0008
#define ADCDAT		0x000C
#define CLRINTADC	0x0018
#define ADCMUX		0x001C
#define EXYNOS_PA_IESR	0x10440000
#define EXYNOS4_SZ_IESR	SZ_4K
#define IESR2		0x0020


struct adc_value_st {
	int value;
};

enum {
	HAVE_DATA,
	NO_DATA,
};
struct adc_st {
	int flag;
	int count;
	spinlock_t lock;
	int value;
	struct cdev dev;
	dev_t no;
	struct file_operations adc_ops;
	void __iomem *v;
	void __iomem *v_iesr;
	u32 adccon, adcdat, adcdly, clrintadc, adcmux, iesr2;
	struct clk *clk;
	int irq;
	wait_queue_head_t rq;
};
#endif
