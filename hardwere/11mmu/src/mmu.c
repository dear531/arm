#include <mmu.h>
#include <common.h>

void create_table(unsigned int *ttb)
{
	unsigned int va = 0, pa = 0;
	for (va = 0; va < 0x55000000; va += 0x1000000) {
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

void table_mmap(unsigned int va, unsigned int pa)
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
