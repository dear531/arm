#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <asm/arch/gpio.h>
#include <linux/interrupt.h>
#include <linux/input.h>

struct input_button {
	struct input_dev *button_dev;
	struct pin_desc {
		unsigned int pin;
		unsigned int key_val;
		int irq;
		char *name;
	} pins_desc[4];
	struct timer_list timer;
};

struct input_button *input;

irqreturn_t irq_button_handler(int irq, void *data)
{
	input->timer.data = (unsigned int)data;
	mod_timer(&input->timer, jiffies + HZ / 100);
	return IRQ_RETVAL(IRQ_HANDLED);
}

void button_timer_func(unsigned long data)
{
	/* 在定时器处理函数当中，上报按键事件 */
	struct pin_desc *desc;
	unsigned int pinval;

	desc = (struct pin_desc *)data;

	/* 如果是第一次，刚注册时，data为空值，不执行
	 * 起到定时器注册之初不执行的作用 */
	if (!desc)
		return;
	/* 通过gpio物理地址获得引脚状态 */
	pinval = s3c2410_gpio_getpin(desc->pin);
	if (pinval) {
	/* 按键按下为低电平 */
		input_event(input->button_dev, EV_KEY, desc->key_val, 0);
	} else {
	/* 按键抬起为高电平 */
		input_event(input->button_dev, EV_KEY, desc->key_val, 1);
	}
	input_sync(input->button_dev);
}


static int __init input_button_init(void)
{
	int ret;
	int i;
	input = kzalloc(sizeof(*input), GFP_KERNEL);
	if (!input) {
		ret = -ENOMEM;
		goto kzalloc_err;
	}
	/* 1.分配一个input_dewv结构体 */
	input->button_dev = input_allocate_device();
	if (!input->button_dev) {
		ret = -ENOMEM;
		goto input_allocate_device_err;
	}
	/* 2.设置 */
#if 0
#define EV_KEY	0x01	表示哪些按键
#define EV_REL	0x02	表示能产生哪些绝对位移事件，x,y触摸屏
#define EV_ABS	0x03	表示能产生哪些相对位移事件，x,y,滚轮
#endif
	/* 2.1 能产生哪类事件 */
	set_bit(EV_KEY, input->button_dev->evbit);
	/* 产生按键不动不断产生输出的重复按键事件 */
	set_bit(EV_REP, input->button_dev->evbit);
	/* 2.2 能产生这类操作里的哪些事件 */
	set_bit(KEY_L, input->button_dev->keybit);
	set_bit(KEY_S, input->button_dev->keybit);
	set_bit(KEY_LEFTSHIFT, input->button_dev->keybit);
	set_bit(KEY_ENTER, input->button_dev->keybit);
	/* 3.注册 */
	ret = input_register_device(input->button_dev);
	if (ret) {
		goto input_register_device_error;
	}
	/* 4.硬件相关的操作 */

	/* 4.1 按键初始化 */
	/* 通过引脚获得GPIO定义获得引脚状态按下还是抬起 */
	/* 这种对应关系是从哪来的，FIXME */
	input->pins_desc[0].pin = S3C2410_GPF0;
	input->pins_desc[1].pin = S3C2410_GPF2;
	input->pins_desc[2].pin = S3C2410_GPG3;
	input->pins_desc[3].pin = S3C2410_GPG11;

	/*  设置按键对应的按键值 */
	input->pins_desc[0].key_val = KEY_L;
	input->pins_desc[1].key_val = KEY_S;
	input->pins_desc[2].key_val = KEY_LEFTSHIFT;
	input->pins_desc[3].key_val = KEY_ENTER;

	/* 按键中断号设置 */
	input->pins_desc[0].irq = IRQ_EINT0;
	input->pins_desc[1].irq = IRQ_EINT2;
	input->pins_desc[2].irq = IRQ_EINT11;
	input->pins_desc[3].irq = IRQ_EINT19;

	/* 按键的名字 */
	input->pins_desc[0].name = "s2";
	input->pins_desc[1].name = "s3";
	input->pins_desc[2].name = "s4";
	input->pins_desc[3].name = "s5";

	/* 设置定时器，用于按键对按键反复产生中断去抖动 */
	init_timer(&input->timer);
	input->timer.function = button_timer_func;
	add_timer(&input->timer);

	/* 申请中断 */
	for (i = 0; i < sizeof(input->pins_desc) / sizeof(*input->pins_desc); i++) {
		ret = request_irq(input->pins_desc[i].irq, irq_button_handler, 
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
			input->pins_desc[i].name, input->pins_desc + i);
		if (ret) {
			/* FIXME:do something */
			goto request_irq_error;
		}
	}

	return 0;
request_irq_error:
	while (i--) {
		free_irq(input->pins_desc[i].irq, input->pins_desc + i);
	}
	del_timer(&input->timer);
	input_unregister_device(input->button_dev);
input_register_device_error:
	input_free_device(input->button_dev);
input_allocate_device_err:
	kfree(input);
kzalloc_err:
	input = NULL;
	return ret;
}

static void __exit input_button_exit(void)
{
	int i = 0;
	del_timer(&input->timer);
	for (i = 0; i < sizeof(input->pins_desc) / sizeof(*input->pins_desc); i++) {
		free_irq(input->pins_desc[i].irq, input->pins_desc + i);
	}
	input_unregister_device(input->button_dev);
	input_free_device(input->button_dev);
	kfree(input);
	input = NULL;
	return;
}

module_init(input_button_init);
module_exit(input_button_exit);

MODULE_LICENSE("GPL");
