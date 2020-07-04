#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/device.h>
#include "bdd.h"

/* 设备模型原理 */

/* 当总线上注册新设备或者新驱动的时候调用
 * 总线用于对设备和驱动的匹配用，回调函数，给内核用
 */
int led_match(struct device *dev, struct device_driver *drv)
{
	struct led_device *ldev;
	struct led_driver *ldrv;

	ldev = container_of(dev, struct led_device, dev);
	ldrv = container_of(drv, struct led_driver, drv);

	return (ldev->id == ldrv->id);
}

struct led_bus ledbus = {
	.id = 0,
	.bus = {
		/* 注册总线的名字，会在/sys/bus/形成一个目录 */
		.name = "ledbus",
		/* 用于总线将设备和驱动惊醒匹配用 */
		.match = led_match,	
	},
};

EXPORT_SYMBOL(ledbus);

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	/* 注册总线 */
	return bus_register(&ledbus.bus);
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
#if 0
void bus_unregister(struct bus_type *bus)
#endif
	printk("%s:%d\n", __func__, __LINE__);
	/* 删除总线 */
	bus_unregister(&ledbus.bus);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
