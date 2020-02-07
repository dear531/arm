#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

/* nrf24l01无线模块 */

struct nrf_st {
	int no;
	int light;
	char msg[16];
};

void ctor(void *data)
{
	static int counter = 0;
	struct nrf_st *nrf = data;
	nrf->no = counter++;
	printk("nrf->no:%x, nrf:%p\n", nrf->no, nrf);
	return;
}

static __init int mm_init(void)
{
#if 0
struct kmem_cache *
kmem_cache_create(const char *name, size_t size, size_t align,
		  unsigned long flags, void (*ctor)(void *))
#endif
	struct kmem_cache *kmem = NULL;
	void *nrf = NULL;
	/* 在分配缓存当中每一个结构体时调用，会把结构体指针传给函数 */
	kmem = kmem_cache_create("nrt_st", sizeof(struct nrf_st), 0, SLAB_HWCACHE_ALIGN, ctor);
	printk("kmem:%p\n", kmem);
#if 0
void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
#endif
	nrf = kmem_cache_alloc(kmem, GFP_KERNEL);
	printk("nrf:%p\n", nrf);
#if 0
void kmem_cache_free(struct kmem_cache *cachep, void *objp)
#endif
	kmem_cache_free(kmem, nrf);
	kmem_cache_destroy(kmem);
	return 0;
}

static __exit void mm_exit(void)
{
}

module_init(mm_init);
module_exit(mm_exit);

MODULE_LICENSE("GPL");
