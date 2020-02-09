#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

struct proc_dir_entry *dir = NULL;
char data[PAGE_SIZE] = {0};
#if 1
static int proc_dir_show(struct seq_file *m, void *v)
{

	seq_printf(m, "This show func:%s:%d:%s\n",
		__func__, __LINE__, data);
        return 0;
}

#endif
ssize_t  dir_write(struct file *seq, const char __user *data1, size_t len, loff_t *l)
{
	int n = 0;
	n = copy_from_user(data, data1, len);
	printk("len:%d, n:%d, data:%s\n", len, n, data);
	return len - n;
}

static int dir_devices_proc_open(struct inode *inode, struct file *file)
{
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
void remove_proc_entry(const char *name, struct proc_dir_entry *parent)
#endif
	remove_proc_entry("devices", dir);
	remove_proc_entry("uplooking", NULL);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
