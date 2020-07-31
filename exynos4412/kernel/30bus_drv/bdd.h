#ifndef __BDD_H__
#define __BDD_H__
struct led_bus {
	int id;
	struct bus_type bus;
};
struct led_device {
	int id;
	struct device dev;
};

struct led_driver {
	int id;
	struct device_driver drv;
};

extern struct led_bus ledbus;

enum {
	RED, GREEN, BLUE,
};

#endif
