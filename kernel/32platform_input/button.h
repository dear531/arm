#ifndef __BUTTON_H__
#define __BUTTON_H__

#define GPX1DAT 0x0C24
#define GPX2DAT 0x0C44
#define EXYNOS_PA_BUTTON 0x11000000
#define EXYNOS_SZ_BUTTON SZ_4K
#define EXYNOS_NUM_BUTTON 4

struct button_st {
	void * __iomem v;
	int irq;
	/* 上下，enter,ESC */
	int code;
	struct input_dev *dev;
};

struct platform_data_st {
	int num;
	char *info;
};
#endif
