#include <linux/init.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>

static __init int mm_init(void)
{
#if 0
static inline void *dma_alloc_coherent(struct device *dev, size_t size,
		dma_addr_t *dma_handle, gfp_t flag)
#endif
	/* 如dma用到的申请虚拟内存和物理内存都返回地址方法 */
	void *v = NULL;
	dma_addr_t dma_handle = 0;
	v = dma_alloc_coherent(NULL, 480 * 800 * 4, &dma_handle, GFP_KERNEL);
	memcpy(v, "123", 4);
	printk("v:%s\n", (char *)v);
#if 0
static inline void dma_free_coherent(struct device *dev, size_t size,
		void *cpu_addr, dma_addr_t dma_handle)
#endif
	dma_free_coherent(NULL, 480 * 800 * 4, v, dma_handle);
	return 0;
}

static __exit void mm_exit(void)
{
}

module_init(mm_init);
module_exit(mm_exit);

MODULE_LICENSE("GPL");
