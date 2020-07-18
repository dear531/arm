#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>

#include "button.h"

struct platform_device_id id_table[] = {
	{"my-button", 123},
};

#if 0
struct input_dev {
	const char *name;
	const char *phys;
	const char *uniq;
	struct input_id id;

	unsigned long propbit[BITS_TO_LONGS(INPUT_PROP_CNT)];

	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
	unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
	unsigned long swbit[BITS_TO_LONGS(SW_CNT)];

	unsigned int hint_events_per_packet;

	unsigned int keycodemax;
	unsigned int keycodesize;
	void *keycode;

	int (*setkeycode)(struct input_dev *dev,
			  const struct input_keymap_entry *ke,
			  unsigned int *old_keycode);
	int (*getkeycode)(struct input_dev *dev,
			  struct input_keymap_entry *ke);

	struct ff_device *ff;

	unsigned int repeat_key;
	struct timer_list timer;

	int rep[REP_CNT];

	struct input_mt_slot *mt;
	int mtsize;
	int slot;
	int trkid;

	struct input_absinfo *absinfo;

	unsigned long key[BITS_TO_LONGS(KEY_CNT)];
	unsigned long led[BITS_TO_LONGS(LED_CNT)];
	unsigned long snd[BITS_TO_LONGS(SND_CNT)];
	unsigned long sw[BITS_TO_LONGS(SW_CNT)];

	int (*open)(struct input_dev *dev);
	void (*close)(struct input_dev *dev);
	int (*flush)(struct input_dev *dev, struct file *file);
	int (*event)(struct input_dev *dev, unsigned int type, unsigned int code, int value);

	struct input_handle __rcu *grab;

	spinlock_t event_lock;
	struct mutex mutex;

	unsigned int users;
	bool going_away;

	bool sync;

	struct device dev;

	struct list_head	h_list;
	struct list_head	node;
};

struct input_id {
	__u16 bustype;,
	__u16 vendor;
	__u16 product;
	__u16 version;
};
#endif

void exynos4_button_init(struct button_st *b, int num, struct resource **irq_res)
{
	int i;
	for (i = 0; i < num; i++) {
		b[i].dev = b[0].dev;
		b[i].irq = irq_res[i / 2]->start + i % 2;
		b[i].v = b[0].v;
	}
	b[0].code = KEY_UP;
	b[1].code = KEY_DOWN;
	b[2].code = KEY_ENTER;
	b[3].code = KEY_ESC;
}

int is_down(struct button_st *b)
{
	u32 data;
	unsigned long giodat;
	int i, n;

	/* 根据中断线获得中断所处第一段资源还是第二段资源 */
	i = b->irq / IRQ_EINT(16);

	/* 根据资源所在位置，获取内存相对基地址的偏移位置，即属于GPX1还是GPX2 */
	giodat = (0 == i ? GPX1DAT : GPX2DAT);

	/* 根据资源所处位置计算在寄存器数据所在的位置 */
	n = (0 == i ? b->irq % IRQ_EINT(9) : b->irq % IRQ_EINT(16));
	data = ioread32(b->v + giodat) & 0x3;

	return !!(data & (1 << n));
}

irqreturn_t button_isr(int irq, void *data)
{
	struct button_st *b;
	b = data;
	printk("%s:%d:b.code:%d\n",
		__func__, __LINE__, b->code);

	/* 向事件层报告键盘事件 */
#if 0
static inline void input_report_key(struct input_dev *dev, unsigned int code, int value)
#endif
	/* 报告相对事件 */
	if (is_down(b))
		input_report_key(b->dev, b->code, 1);
	else
		input_report_key(b->dev, b->code, 0);
	/* 报告同步事件 */
	input_sync(b->dev);
	return IRQ_HANDLED;
}

int my_probe(struct platform_device *pdev)
{
	struct button_st *button;
	struct resource *io_res, *irq_res[2];
	struct platform_data_st *pdata;
	int ret = 0;
	int i;

	printk("%s:%d\n", __func__, __LINE__);

	/* 获得内存资源 */
	io_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	/* 获得中断第0个资源 */
	irq_res[0] = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	/* 获得中断第1个资源 */
	irq_res[1] = platform_get_resource(pdev, IORESOURCE_IRQ, 1);
	if (!io_res || !irq_res[0] || !irq_res[1]) {
		return -EBUSY;
	}
	/* 分配设备结构体数组 */
	pdata = pdev->dev.platform_data;
	printk("%s:%d:num:%d\n", __func__, __LINE__, pdata->num);
	button = kzalloc(sizeof(*button) * pdata->num, GFP_KERNEL);
	if (!button) {
		printk("%s:%dkzalloc error\n", __func__, __LINE__);
		ret = -ENOMEM;
		goto kzalloc_error;
	}
#if 0
struct input_dev *input_allocate_device(void);
#endif
	/* 创建设备 */
	button->dev = input_allocate_device();
	if (!button->dev) {
		printk("%s:%dkzalloc error\n", __func__, __LINE__);
		ret = -ENOMEM;
		goto kzalloc_error_2;
	}
#if 0
[root@iTOP-4412]# cat /proc/bus/input/devices 
I: Bus=0019 Vendor=0001 Product=0001 Version=0001
N: Name="my-button"
P: Phys=/sys/xxx
S: Sysfs=/devices/virtual/input/input2
U: Uniq=20200711
H: Handlers=event1 
B: PROP=0
B: EV=3
B: KEY=1080 0 0 10000002

#endif
	/* my-button */
	button->dev->name = pdev->name;
	button->dev->phys = "/sys/xxx";
	button->dev->uniq = "20200711";
	/* platform bus */
	button->dev->id.bustype = BUS_HOST;
	/* 厂家 */
	button->dev->id.vendor = 1;
	/* 产品 */
	button->dev->id.product = 1;
	/* 版本 */
	button->dev->id.version = 1;
	/* 键盘及同步事件 */
	set_bit(EV_KEY,  button->dev->evbit);
	set_bit(EV_SYN,  button->dev->evbit);
	/* 上下，enter,ESC */
	set_bit(KEY_UP,  button->dev->keybit);
	set_bit(KEY_DOWN,  button->dev->keybit);
	set_bit(KEY_ENTER,  button->dev->keybit);
	set_bit(KEY_ESC,  button->dev->keybit);

	/* 注册设备 */
	ret = input_register_device(button->dev);
	if (ret) {
		printk("%s:%dinput_register_device\n", __func__, __LINE__);
		ret = -EBUSY;
		goto input_register_device_error;
	}

	/* 对读取高低电平的内存进行内存映射 */
	button->v = ioremap(io_res->start,
		io_res->end - io_res->start + 1);
	if (!button->v) {
		printk("%s:%dioremap\n", __func__, __LINE__);
		ret = -ENOMEM;
		goto ioremap_error;
	}
#if 0
void exynos4_button_init(struct button_st *b, int num, struct resource *irq_res)
#endif
	/* 初始化按键 */
	exynos4_button_init(button, pdata->num, irq_res);

	/* 申请中断 */
	for (i = 0; i < pdata->num; i++) {
		ret = request_irq(button[i].irq, button_isr,
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
			pdev->name, button + i);
		if (ret < 0) {
			goto request_irq_error;
		}
	}
	/* 将申请的button设备数据保存到dev设备结构体数据中 */
	platform_set_drvdata(pdev, button);

	return 0;
request_irq_error:
	/* 取值后再自减，让后续操作取得正确的值 */
	while (i--)
		free_irq(button[i].irq, button + i);
	iounmap(button->v);
ioremap_error:
	input_unregister_device(button->dev);
input_register_device_error:
#if 0
void input_free_device(struct input_dev *dev);
#endif
	input_free_device(button->dev);
kzalloc_error_2:
	kfree(button);
kzalloc_error:
	return ret;
}

int my_remove(struct platform_device *pdev)
{
	struct button_st *button;
	struct platform_data_st *pdata;
	int i;

	button = platform_get_drvdata(pdev);
	pdata = pdev->dev.platform_data;

	for (i = 0; i < pdata->num; i++) {
		free_irq(button[i].irq, button + i);
	}

	iounmap(button->v);
	input_unregister_device(button->dev);
	input_free_device(button->dev);
	kfree(button);
	return 0;
}

void my_shutdown(struct platform_device *dev)
{
	printk("%s:%d\n", __func__, __LINE__);
}

int my_suspend(struct platform_device *dev, pm_message_t state)
{
	printk("%s:%d\n", __func__, __LINE__);
	return 0;
}

int my_resume(struct platform_device *dev)
{
	printk("%s:%d\n", __func__, __LINE__);
	return 0;
}

#if 0
struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *, pm_message_t state);
	int (*resume)(struct platform_device *);
	struct device_driver driver;
	const struct platform_device_id *id_table;
};
#endif

struct platform_driver drv = {
	.probe = my_probe,
	.remove = my_remove,
	.shutdown = my_shutdown,
	.suspend = my_suspend,
	.resume = my_resume,
	.driver = {
		.name = "my-button",
	},
	.id_table = id_table,
};

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	return platform_driver_register(&drv);
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	platform_driver_unregister(&drv);
	return;
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
