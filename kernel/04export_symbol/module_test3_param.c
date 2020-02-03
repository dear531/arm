#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

static int no = 0;
static char *buff = NULL;
bool bl = true;

/* when install then perform */
static __init int module_test1_init(void)
{
	printk("%s:%d, no:%d\n", __func__, __LINE__, no);
	printk("%s:%d, buff:%s\n", __func__, __LINE__, buff);
	printk("%s:%d, bl:%d\n", __func__, __LINE__, bl);
	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	printk("%s:%d\n", __func__, __LINE__);
	printk("%s:%d, buff:%s\n", __func__, __LINE__, buff);
	printk("%s:%d, bl:%d\n", __func__, __LINE__, bl);
}

module_init(module_test1_init);
module_exit(module_test1_exit);
module_param_named(n, no, int, 0);
module_param_named(p, buff, charp, 0);
module_param_named(invb, bl, invbool, 0);
#if 0
[root@broad 04export_symbol] # insmod module_test3_param.ko invb=1 p=123 n=2
[ 5127.555106] module_test1_init:12, no:2
[ 5127.557476] module_test1_init:13, buff:123
[ 5127.561617] module_test1_init:14, bl:0
[root@broad 04export_symbol] # rmmod module_test3_param
[ 5137.226695] module_test1_exit:21
[ 5137.228546] module_test1_exit:22, buff:123
[ 5137.232612] module_test1_exit:23, bl:0

#endif

MODULE_LICENSE("GPL");
