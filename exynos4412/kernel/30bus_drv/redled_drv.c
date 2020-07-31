#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

#include <linux/device.h>

#include "bdd.h"

/* 当某个设备和当前驱动匹配成功时调用 */
int led_probe(struct device *dev)
{
	struct led_device *ldev;
	ldev = container_of(dev, struct led_device, dev);
	printk("%s, ldev->id:%d\n", __func__, ldev->id);
	/* 成功返回0 */
	return 0;
}

/* 当前驱动匹配成功的设备注销或者当前驱动注销时调用 */
int led_remove(struct device *dev)
{
	struct led_device *ldev;
	ldev = container_of(dev, struct led_device, dev);
	printk("%s, ldev->id:%d\n", __func__, ldev->id);
	/* 成功返回0 */
	return 0;
}

struct led_driver red = {
	.id = RED,
	.drv = {
		/* 该名字会出现在/sys/bus/ledbus/drivers目录下 */
		.name = "reddrv",
		/* 挂载总线的变量 */
		.bus = &ledbus.bus,
		.probe = led_probe,
		.remove = led_remove,
	},
};
/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	return driver_register(&red.drv);
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	driver_unregister(&red.drv);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
