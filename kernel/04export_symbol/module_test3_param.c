#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

static int no = 0;
static char *buff = NULL;
static bool bl = true;
static int array[10] = {0};
static int n = 0;

/* when install then perform */
static __init int module_test1_init(void)
{
	int i = 0;
	printk("%s:%d, no:%d\n", __func__, __LINE__, no);
	printk("%s:%d, buff:%s\n", __func__, __LINE__, buff);
	printk("%s:%d, bl:%d\n", __func__, __LINE__, bl);
	for (i = 0; sizeof(array) / sizeof(*array) > i; i++) {
		printk("%s:%d, array[%d]:%d\n", __func__, __LINE__, i, array[i]);
	}
	printk("%s:%d, n:%d\n", __func__, __LINE__, n);

	return 0;
}

/* when uninstall then perfrom */
static __exit void module_test1_exit(void)
{
	int i = 0;

	printk("%s:%d\n", __func__, __LINE__);
	printk("%s:%d, buff:%s\n", __func__, __LINE__, buff);
	printk("%s:%d, bl:%d\n", __func__, __LINE__, bl);

	for (i = 0; sizeof(array) / sizeof(*array) > i; i++) {
		printk("%s:%d, array[%d]:%d\n", __func__, __LINE__, i, array[i]);
	}
	printk("%s:%d, n:%d\n", __func__, __LINE__, n);
}

module_init(module_test1_init);
module_exit(module_test1_exit);
#if 0
show param's permissions

[root@broad 04export_symbol] # ls /sys/module/module_test3_param/parameters/ -lrt
total 0
-rw-rw-r--    1 0        0             4096 Jan  1 02:43 p
-rw-rw-r--    1 0        0             4096 Jan  1 02:43 n
-rw-rw-r--    1 0        0             4096 Jan  1 02:43 invb
-rw-rw-r--    1 0        0             4096 Jan  1 02:43 a
#endif
module_param_named(n, no, int, 0664);
module_param_named(p, buff, charp, 0664);
module_param_named(invb, bl, invbool, 0664);
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

module_param_array_named(a, array, int, &n, 0664);
#if 0
/**
 * module_param_array_named - renamed parameter which is an array of some type
 * @name: a valid C identifier which is the parameter name
 * @array: the name of the array variable
 * @type: the type, as per module_param()
 * @nump: optional pointer filled in with the number written
 * @perm: visibility in sysfs
 *
 * This exposes a different name than the actual variable name.  See
 * module_param_named() for why this might be necessary.
 */
#define module_param_array_named(name, array, type, nump, perm)         \
        param_check_##type(name, &(array)[0]);                          \
        static const struct kparam_array __param_arr_##name             \
        = { .max = ARRAY_SIZE(array), .num = nump,                      \
            .ops = &param_ops_##type,                                   \
            .elemsize = sizeof(array[0]), .elem = array };              \
        __module_param_call(MODULE_PARAM_PREFIX, name,                  \
                            &param_array_ops,                           \
                            .arr = &__param_arr_##name,                 \
                            perm, -1, 0);                               \
        __MODULE_PARM_TYPE(name, "array of " #type)

#endif


MODULE_LICENSE("GPL");
