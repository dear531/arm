#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>

static __init int mm_init(void)
{
	struct page *page = NULL;
	void *virtual = NULL;
	void *virtual2 = NULL;

	page = alloc_pages(GFP_KERNEL, 1);
	virtual = page_address(page);
	memcpy(virtual, "123", 4);
	printk("%p,%s\n", virtual, (char*)virtual);

	virtual2 = page_address(page+1);
	memcpy(virtual2, "456", 4);
	printk("%p,%s\n", virtual2, (char*)virtual2);
	__free_pages(page, 1);

	page = NULL;
	page = alloc_page(GFP_KERNEL);
	virtual = page_address(page);
	memcpy(virtual, "abc", 4);
	printk("%p,%s\n", virtual, (char*)virtual);
	__free_page(page);

	page = NULL;
	virtual = (void *)__get_free_pages(GFP_KERNEL, 1);
	memcpy(virtual, "efg", 4);
	printk("%p,%s\n", virtual, (char*)virtual);
	free_pages((unsigned long)virtual, 1);
	virtual = NULL;
	return 0;
}

static __exit void mm_exit(void)
{
}

module_init(mm_init);
module_exit(mm_exit);

MODULE_LICENSE("GPL");
