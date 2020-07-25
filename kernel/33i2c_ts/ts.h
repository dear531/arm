#ifndef __TS_H__
#define __TS_H__
/* i2c_transefer--可能睡眠 */
struct ts_info_st {
        int irq; 
        int xres;
        int yres;
        int pressure;
};

struct ts_st {
	int x;
	int y;
	int irq;
	int xres;
	int yres;
	int pressure;

	struct i2c_client *client;
	struct input_dev *dev;
	struct work_struct work;
};
#endif
