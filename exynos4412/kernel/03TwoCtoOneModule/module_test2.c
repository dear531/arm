#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

int my_print(int no)
{
	int i = 0;
	for (i = 0; no > i; i++) {
		printk("no : %d\n", i);
	}

	return i - 1;
}
