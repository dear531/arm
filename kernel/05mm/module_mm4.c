#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/slab.h>

/* 临时映射 */

static __init int mm_init(void)
{
	void *v = NULL;
	/* 物理和虚拟都连续，内存中内容随机 */
	v = kmalloc(4, GFP_KERNEL);
	printk("before memcpy:%s, %s\n", current->comm, (char*)v);
	memcpy(v, "123", 4);
	printk("after memcpy:%s, %s\n", current->comm, (char*)v);
	kfree(v);
	/* 物理和虚拟都连续，内存清零 */
	v = kzalloc(4, GFP_KERNEL);
	printk("before memcpy:%s, %s\n", current->comm, (char*)v);
	memcpy(v, "456", 4);
	printk("after memcpy:%s, %s\n", current->comm, (char*)v);
	kfree(v);
	/* 虚拟内存连续，物理内存不保证，内存内容随机，可能睡眠 */
	v = vmalloc(4);
	printk("before memcpy:%s, %s\n", current->comm, (char*)v);
	memcpy(v, "789", 4);
	printk("after memcpy:%s, %s\n", current->comm, (char*)v);
	vfree(v);
	/* 虚拟内存连续，物理内存不保证，内存内情清零，可能睡眠 */
	v = vzalloc(4);
	printk("before memcpy:%s, %s\n", current->comm, (char*)v);
	memcpy(v, "abc", 4);
	printk("after memcpy:%s, %s\n", current->comm, (char*)v);
	vfree(v);

	return 0;
}

static __exit void mm_exit(void)
{
}

module_init(mm_init);
module_exit(mm_exit);

MODULE_LICENSE("GPL");
