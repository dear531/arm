#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include "adc.h"

/* when install then perform */
static __init int module_test1_init(void)
{
#if 0
struct clk *clk_get(struct device *dev, const char *id)
#endif
	struct clk *clk = NULL;
	clk = clk_get(NULL, "adc");
	if (!clk) {
		printk("clk_get adc oerror\n");
		return -ENODEV;
	}

	clk_enable(clk);
	printk("%s:%d\n", __func__, __LINE__);

	adc_init();

	while (1) {
		adc_start();
		wait_for_adc();
		printk("%s:%d:value:%d\n",
			__func__, __LINE__ ,get_adc());
		ssleep(1);
	}
	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
