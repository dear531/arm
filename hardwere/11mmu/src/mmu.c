#include <mmu.h>
#include <common.h>

#if 1
void section_create_table(unsigned int *ttb)
{
	unsigned int va = 0, pa = 0;
	for (va = 0; va < 0x55000000; va += 0x100000) {
		pa = va;
		ttb[va >> 20] = (pa & 0xfff00000) | 2;
	}
	__asm__ __volatile__ (
		"mrc p15, 0, r4, c3, c0, 0\n"
		"orr r4, r4, #3\n"
		"mcr p15, 0, r4, c3, c0, 0\n"
		:
		:
		:"r4"
	);
	__asm__ __volatile__ (
		"MCR p15, 0, %0, c2, c0, 0\n"
		:
		:"r"(ttb)
		:
	);
}

void section_table_mmap(unsigned int va, unsigned int pa)
{
	unsigned int *ttb = 0;
	asm volatile (
		"mrc p15, 0, %0, c2, c0, 0\n"
		:"=&r"(ttb)
		:
		:
	);
	ttb[va >> 20] = (pa & 0xfff00000) | 2;
}

#endif

#if 1
void page_create_table(unsigned int *ttb1, unsigned int *ttb2)
{
	unsigned int va1 = 0, pa = 0, va2 = 0;
	for (va1 = 0; va1 < 0x54000000; va1 += 0x100000) {
		ttb1[va1 >> 20] = (unsigned int)ttb2 & 0xfffffc00 | 1;
		for (va2 = va1; va2 < va1 + 0x100000; va2 += 0x1000) {
			pa = va2;
			ttb2[(va2 >> 12) & 0xff] = pa & ~0xfff | 2;
		}
		ttb2 += 0x100;
	}
	__asm__ __volatile__ (
		"mrc p15, 0, r4, c3, c0, 0\n"
		"orr r4, r4, #3\n"
		"mcr p15, 0, r4, c3, c0, 0\n"
		:
		:
		:"r4"
	);
	__asm__ __volatile__ (
		"MCR p15, 0, %0, c2, c0, 0\n"
		:
		:"r"(ttb1)
		:
	);
}

void page_table_mmap(unsigned int va, unsigned int pa)
{
	unsigned int *ttb1 = 0, *ttb2 = 0;
	/* get page table from register c2 */
	asm volatile (
		"mrc p15, 0, %0, c2, c0, 0\n"
		:"=&r"(ttb1)
		:
		:
	);
	/* 找到二级页表首地址 */
	printf("ttb1:%#010x\n", ttb1);
	printf("va:%#010x\n", va);
	ttb2 = (unsigned int *)(ttb1[va >> 20] & ~0x3ff);
	/* 修改虚拟地址对应二级页表项所对应条目 */
	printf("%s:%d,ttb2:%#010x\n", __FILE__, __LINE__, ttb2);
	printf("va:%#010x, ttb2[%#04x]:%#010x\n",
		va, (va >> 12) & 0xff, ttb2[(va >> 12) & 0xff]);
	ttb2[(va >> 12) & 0xff] &= 0xfff;
	ttb2[(va >> 12) & 0xff] |= pa & ~0xfff;
}

#endif
void mmu_enable()
{
	asm volatile (
		"MRC p15, 0, r4, c1, c0, 0\n" /* Read SCTLR */
		"orr r4, r4, #1\n"
		"MCR p15, 0, r4, c1, c0, 0\n" /* Write SCTLR */
		:
		:
		:"r4"
	);
}

void mmu_disable()
{
	asm volatile (
		"MRC p15, 0, r4, c1, c0, 0\n" /* Read SCTLR */
		"bic r4, r4, #1\n"
		"MCR p15, 0, r4, c1, c0, 0\n" /* Write SCTLR */
		:
		:
		:"r4"
	);
}
