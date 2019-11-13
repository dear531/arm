#include <common.h>
#include <hardware.h>
#include <lib.h>

int main(void)
{
	int *p = (void *)0x52345678;
	*p = 100;
	printf("*(%#010x)=%d\n", p, *p);
	section_create_table(0x55000000);
	section_table_mmap(0x12345678, 0x52345678);
	mmu_enable();
	printf("*(0x12345678)=%d, *(0x52345678)=%d\n",
		*(unsigned int *)0x12345678, *(unsigned int *)0x52345678);
	return 0;
}
