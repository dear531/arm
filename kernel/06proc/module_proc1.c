#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>

#if 0
[root@broad 06proc] # insmod module_proc1.ko 
[root@broad 06proc] # cat /proc/uplooking/devices 
[root@broad 06proc] # echo 1 > /proc/uplooking/devices 
[root@broad 06proc] # echo 2 > /proc/uplooking/devices 
[root@broad 06proc] # echo 3 > /proc/uplooking/devices 
[root@broad 06proc] # cat /proc/uplooking/devices 
This show func:proc_dir_show:52:1

This show func:proc_dir_show:52:2

This show func:proc_dir_show:52:3

[root@broad 06proc] # rmmod module_proc1
#endif

struct proc_dir_entry *dir = NULL;

struct head_st {
	struct list_head h;
	int count;
} head = {
	.count = 0,
};

struct data_st {
	struct list_head i;
	char *content;
	int len;
};
#if 1
static int proc_dir_show(struct seq_file *m, void *v)
{
#if 0
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))
#endif
	struct data_st *tmp = NULL;
	list_for_each_entry(tmp, &head.h, i) {
		seq_printf(m, "This show func:%s:%d:%s\n",
			__func__, __LINE__, tmp->content);
	}
        return 0;
}

#endif
ssize_t  dir_write(struct file *seq, const char __user *data, size_t len, loff_t *l)
{
	int ret = 0;
	int n = 0;
	struct data_st *pdata = NULL;
	/* 申请data_st+content长度空间，将结构体和content的内容都一次申请好  */
	pdata = kzalloc(sizeof(*pdata) + len + 1, GFP_KERNEL);
	if (NULL == pdata) {
		ret = -ENOMEM;
		goto Ekalloc;
	}
	/* 将直接申请好的结构体后续跟着的coutent内容赋值给content结构体指针 */
	pdata->content = (char *)(pdata + 1);
	n = copy_from_user(pdata->content, data1, len);
	if (0 > n) {
		ret = -EFAULT;
		goto Ecopy;
	}
#if 0
	list_add_tail(struct list_head *entry, struct list_head *head)
#endif
	list_add_tail(&pdata->i, &head.h);
	pdata->len = len;
	head.count += len;

	return len - n;
Ecopy:
	kfree(pdata);
	pdata = NULL;
Ekalloc:
	return ret;
#endif
}

static int dir_devices_proc_open(struct inode *inode, struct file *file)
{
#if 0
int single_open_size(struct file *file, int (*show)(struct seq_file *, void *),
#endif
	return single_open(file, proc_dir_show, NULL);
}

static const struct file_operations proc_dir = {
        .owner          = THIS_MODULE,
        .open           = dir_devices_proc_open,
        .read           = seq_read,
#if 0
int seq_write(struct seq_file *seq, const void *data, size_t len)
ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	.write		= seq_write,
#endif
	.write		= dir_write,
        .llseek         = seq_lseek,
        .release        = single_release,
};


static __init int proc_init(void)
{
	int ret = 0;
#if 0
struct proc_dir_entry *proc_mkdir(const char *name,
		struct proc_dir_entry *parent)
#endif
	dir = proc_mkdir("uplooking", NULL);
	if (NULL == dir) {
		goto Emkdir;
		ret = -ENOMEM;
	}
#if 0
struct proc_dir_entry *proc_create(const char *name, umode_t mode,
				   struct proc_dir_entry *parent,
				   const struct file_operations *proc_fops)
#endif
	if (NULL == proc_create("devices", 0, dir, &proc_dir))
	{
		goto Edevices;
		ret = -ENOMEM;
	}
	INIT_LIST_HEAD(&head.h);
	return ret;
Emkdir:
	remove_proc_entry("devices", dir);
Edevices:
	remove_proc_entry("uplooking", NULL);

	return ret;
}

static __exit void proc_exit(void)
{
#if 0
#define list_for_each_entry_safe(pos, tmp, head, member)		\
    for (pos = __container_of((head)->next, pos, member),		\
	 tmp = __container_of(pos->member.next, pos, member);		\
	 &pos->member != (head);					\
	 pos = tmp, tmp = __container_of(pos->member.next, tmp, member))
#endif
	struct data_st *pos = NULL, *tmp = NULL;
	list_for_each_entry_safe(pos, tmp, &head.h, i) {
		list_del(&pos->i);
		kfree(pos);
	}
#if 0
void remove_proc_entry(const char *name, struct proc_dir_entry *parent)
#endif
	remove_proc_entry("devices", dir);
	remove_proc_entry("uplooking", NULL);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
