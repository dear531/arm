#include <common.h>
#include <hardware.h>
#include <lib.h>

int main(void)
{
	int *p = (void *)0x52345678;
	*p = 100;
#if 0
	/* section table */
	section_create_table(0x55000000);
	section_table_mmap(0x12345678, 0x52345678);
#else
	/* small page table */
	page_create_table(0x55000000, 0x55100000);
	page_table_mmap(0x12345678, 0x52345678);
#endif
	mmu_enable();
	printf("*(0x12345678)=%d, *(0x52345678)=%d\n",
		*(unsigned int *)0x12345678, *(unsigned int *)0x52345678);
	return 0;
}
