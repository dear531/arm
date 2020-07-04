#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/device.h>

#include "bdd.h"

/* 设备模型原理 */
 
/* 当设备注销的时被调用 */
void green_led_release(struct device *dev)
{
	printk("green led unrigister\n");
}


struct led_device green = {
	.id = GREEN,
	.dev = {
		/* 挂载总线的总线变量 */
		.bus = &ledbus.bus, 
		/* 注册设备的名字,
		 * 以此名称greendev挂在其所挂载的变量ledbus所在的目录/sys/bus/ledbus/
		 * 下的设备目录devices/下
		 * /sys/bus/ledbus/devices/greendev/ */
		.init_name = "greendev",
		/* 当设备注销的时被调用 */
		.release = green_led_release,
	},
	
};

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	return device_register(&green.dev);
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	return device_unregister(&green.dev);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
