#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <mach/irqs.h>
#include <linux/platform_device.h>

#include "button.h"

struct platform_data_st pds = {
	.num = EXYNOS_NUM_BUTTON,
	.info = "From Button Dev",
};

#if 0
struct platform_device {
	const char	* name;
	int		id;
	struct device	dev;
	u32		num_resources;
	struct resource	* resource;

	const struct platform_device_id	*id_entry;

	/* MFD cell pointer */
	struct mfd_cell *mfd_cell;

	/* arch specific additions */
	struct pdev_archdata	archdata;
};

struct device {
	struct device		*parent;

	struct device_private	*p;

	struct kobject kobj;
	const char		*init_name; /* initial name of the device */
	const struct device_type *type;

	struct mutex		mutex;	/* mutex to synchronize calls to
					 * its driver.
					 */

	struct bus_type	*bus;		/* type of bus device is on */
	struct device_driver *driver;	/* which driver has allocated this
					   device */
	void		*platform_data;	/* Platform specific data, device
					   core doesn't touch it */
	struct dev_pm_info	power;
	struct dev_power_domain	*pwr_domain;

#ifdef CONFIG_NUMA
	int		numa_node;	/* NUMA node this device is close to */
#endif
	u64		*dma_mask;	/* dma mask (if dma'able device) */
	u64		coherent_dma_mask;/* Like dma_mask, but for
					     alloc_coherent mappings as
					     not all hardware supports
					     64 bit addresses for consistent
					     allocations such descriptors. */

	struct device_dma_parameters *dma_parms;

	struct list_head	dma_pools;	/* dma pools (if dma'ble) */

	struct dma_coherent_mem	*dma_mem; /* internal for coherent mem
					     override */
	/* arch specific additions */
	struct dev_archdata	archdata;

	struct device_node	*of_node; /* associated device tree node */

	dev_t			devt;	/* dev_t, creates the sysfs "dev" */

	spinlock_t		devres_lock;
	struct list_head	devres_head;

	struct klist_node	knode_class;
	struct class		*class;
	const struct attribute_group **groups;	/* optional groups */

	void	(*release)(struct device *dev);
};

struct resource {
	resource_size_t start;
	resource_size_t end;
	const char *name;
	unsigned long flags;
	struct resource *parent, *sibling, *child;
};
#endif

struct resource my_res[] = {
	[0] = {
		.start = EXYNOS_PA_BUTTON,
		.end = EXYNOS_PA_BUTTON + EXYNOS_SZ_BUTTON - 1,
		.name = "io",
		.flags = IORESOURCE_MEM,
	},
	[1] = {
#if 0
		.start = gpio_to_irq(EXYNOS4_GPX1(1)),
		.end = gpio_to_irq(EXYNOS4_GPX1(4)),
#endif
		/* IRQ_EINT--只能用于外部中断，如看门狗等的内部中断不能用此方法 */
		.start = IRQ_EINT(9),
		.end = IRQ_EINT(10),
		.name = "irq",
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		/* 宏的原理同上 */
		.start = IRQ_EINT(16),
		.end = IRQ_EINT(17),
		.name = "irq",
		.flags = IORESOURCE_IRQ,
	},
};

void my_release(struct device *dev)
{
	printk("%s:%d\n", __func__, __LINE__);
}

struct platform_device dev = {
	.name = "my-button",
	.id = -1,
	.dev = {
		.platform_data = &pds,
		.release = my_release,
	},
	.resource = my_res,
	.num_resources = ARRAY_SIZE(my_res),
};

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	return platform_device_register(&dev);
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	platform_device_unregister(&dev);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
