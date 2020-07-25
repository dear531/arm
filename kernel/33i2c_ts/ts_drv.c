#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/gpio.h>

#include "ts.h"

#define TSC2007_MEASURE_TEMP0           (0x0 << 4)
#define TSC2007_MEASURE_AUX             (0x2 << 4)
#define TSC2007_MEASURE_TEMP1           (0x4 << 4)
#define TSC2007_ACTIVATE_XN             (0x8 << 4)
#define TSC2007_ACTIVATE_YN             (0x9 << 4)
#define TSC2007_ACTIVATE_YP_XN          (0xa << 4)
#define TSC2007_SETUP                   (0xb << 4)
#define TSC2007_MEASURE_X               (0xc << 4)
#define TSC2007_MEASURE_Y               (0xd << 4)
#define TSC2007_MEASURE_Z1              (0xe << 4)
#define TSC2007_MEASURE_Z2              (0xf << 4)

#define TSC2007_POWER_OFF_IRQ_EN        (0x0 << 2)
#define TSC2007_ADC_ON_IRQ_DIS0         (0x1 << 2)
#define TSC2007_ADC_OFF_IRQ_EN          (0x2 << 2)
#define TSC2007_ADC_ON_IRQ_DIS1         (0x3 << 2)

#define TSC2007_12BIT                   (0x0 << 1)
#define TSC2007_8BIT                    (0x1 << 1)

#define MAX_12BIT                       ((1 << 12) - 1)

#define ADC_ON_12BIT    (TSC2007_12BIT | TSC2007_ADC_ON_IRQ_DIS0)

#define READ_Y          (ADC_ON_12BIT | TSC2007_MEASURE_Y)
#define READ_Z1         (ADC_ON_12BIT | TSC2007_MEASURE_Z1)
#define READ_Z2         (ADC_ON_12BIT | TSC2007_MEASURE_Z2)
#define READ_X          (ADC_ON_12BIT | TSC2007_MEASURE_X)
#define PWRDOWN         (TSC2007_12BIT | TSC2007_POWER_OFF_IRQ_EN)


void work_handler(struct work_struct *work)
{
	struct ts_st *ts;
	u16 x;
	u16 y;

	printk("%s:%d\n", __func__, __LINE__);
	ts = container_of(work, struct ts_st, work);
	/* client内容来自i2c_dev
	 * i2c_dev里的信息是来源于i2c_board里的信息
	 * 也就是板级文件
	 */
	x = i2c_smbus_read_word_data(ts->client, READ_X);
	y = i2c_smbus_read_word_data(ts->client, READ_X);
	i2c_smbus_read_word_data(ts->client, PWRDOWN);

	printk("value x:%d\n",  x);
	printk("value y:%d\n",  y);

	input_report_abs(ts->dev, ABS_X, x);
	input_report_abs(ts->dev, ABS_Y, y);
	input_sync(ts->dev);

	enable_irq(ts->irq);
}

irqreturn_t ts_handler(int irq, void * data)
{
	struct ts_st *ts;
#if 0
	printk("%s:%d\n", __func__, __LINE__);
#endif
	ts = data;
	if (!gpio_get_value(EXYNOS4_GPX0(0))) {
		disable_irq_nosync(ts->irq);
		schedule_work(&ts->work);
	}

	return IRQ_HANDLED;
}

int ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ts_st *ts;
	struct ts_info_st *pdata;
	int ret;

	printk("%s:%d\n", __func__, __LINE__);

	pdata = client->dev.platform_data;
	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (!ts) {
		ret = -ENOMEM;
		goto kzalloc_error;
	}

	ts->client = client;
	ts->irq = pdata->irq;
	ts->xres = pdata->xres;
	ts->yres = pdata->xres;
	ts->pressure = pdata->pressure;

	INIT_WORK(&ts->work, work_handler);
#if 0
input.h
#endif

	ts->dev = input_allocate_device();
	if (!ts->dev) {
		ret = -ENOMEM;
		goto input_allocate_device_error;
	}
        ts->dev->name = client->name;
        ts->dev->phys = "tsdrvxxxx";
        ts->dev->uniq = "20200724";
	ts->dev->id.bustype = BUS_I2C; 	/* I2C总线 */
	ts->dev->id.vendor = 1;		/* 厂家  */
	ts->dev->id.product = 1;	/* 产品 */
	ts->dev->id.version = 1;	/* 版本 */

	/* 支持同步事件 */
	set_bit(EV_SYN, ts->dev->evbit);	
	/* 支持屏幕点阵值是绝对值的绝对事件 */
	set_bit(EV_ABS, ts->dev->evbit);
	/* 触摸屏是个巨大的按键，支持按键事件 */
	set_bit(EV_KEY, ts->dev->evbit);

	/* 触摸屏的绝对事件坐标x和y */
	set_bit(ABS_X, ts->dev->absbit);
	set_bit(ABS_Y, ts->dev->absbit);

	/* 绝对时间挤压事件 */
	set_bit(ABS_PRESSURE, ts->dev->absbit);

	/* 触摸屏的键盘事件BTN_TOUCH，固定的 */
	set_bit(BTN_TOUCH, ts->dev->keybit);
#if 0
void input_set_abs_params(struct input_dev *dev, unsigned int axis,
                          int min, int max, int fuzz, int flat);
#endif
	/* 设置x和y的最大坐标值 */
	input_set_abs_params(ts->dev, ABS_X, /* x轴 */
                          0, ts->xres, 0, 0);
	input_set_abs_params(ts->dev, ABS_X, /* y轴 */
                          0, ts->yres, 0, 0);

	/* 挤压事件取值范围设定 */
	input_set_abs_params(ts->dev, ABS_PRESSURE, /* 挤压事件 */
                          0, ts->pressure, 0, 0);
	ret = input_register_device(ts->dev);
	if (ret) {
		goto input_register_device_error;
	}

	ret = request_irq(ts->irq, ts_handler, IRQF_TRIGGER_FALLING,
		client->name, ts);
	if (ret) {
		goto request_irq_error;
	}

#if 0
static inline void i2c_set_clientdata(struct i2c_client *dev, void *data)
#endif
	/* 设置驱动私有数据 */
	i2c_set_clientdata(client, ts);
	
	return 0;

	free_irq(ts->irq, ts);
request_irq_error:
	input_unregister_device(ts->dev);
input_register_device_error:
	input_free_device(ts->dev);
input_allocate_device_error:
	kfree(ts);
kzalloc_error:
	return ret;
}

int ts_remove(struct i2c_client *client)
{
	struct ts_st *ts;
	printk("%s:%d\n", __func__, __LINE__);
#if 0
static inline void *i2c_get_clientdata(const struct i2c_client *dev)
#endif
	ts = i2c_get_clientdata(client);
	free_irq(ts->irq, ts);
	input_unregister_device(ts->dev);
	input_free_device(ts->dev);
	kfree(ts);
	return 0;
}

struct i2c_device_id id_table[] = {
	{"my_tsc2007", 123},
	{"herts", 456},
	{},
};

struct i2c_driver ts_drv = {
	.probe = ts_probe,
	.remove = ts_remove,
	.id_table = id_table,
	.driver.name = "myts",
};

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	return i2c_add_driver(&ts_drv);
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	i2c_del_driver(&ts_drv);
}

module_init(module_test1_init);
module_exit(module_test1_exit);

MODULE_LICENSE("GPL");
