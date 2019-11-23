#include <common.h>
#include <hardware.h>
#include <lib.h>

void do_unde(void)
{
	int c = 0;
	__asm__ __volatile__ (
		"mrs %0, cpsr\n"
		:"=&r"(c)
		:
		:
	);
	printf("do_unde: %#010x, cpsr = %#010x\n", do_unde, c);
	return;
}

int main(void)
{
	int *p = (void *)0x52345678;
	*p = 100;
#if 1
	/* section table */
	section_create_table(0x55000000);
	section_table_mmap(0xffff0000, 0x41000000);
#else
	/* small page table */
	page_create_table(0x55000000, 0x55100000);
	page_table_mmap(0xffff0000, 0x41000000);
	/*
	 * 0xfffffxxx-->0x523xxxxx
	 * 0xffffxxxx-->0x5234xxxx
	 * 0xffff0000-->0x52340000
	 * 0xffff0004-->0x52340004
	 */
#endif
#if 0
	delay(1000);
#endif
	mmu_enable();
	printf("%s:%d\n", __FILE__, __LINE__);

	int c = 0;
	__asm__ __volatile__ (
		"mrs %0, cpsr\n"
		:"=&r"(c)
		:
		:
	);
	printf("main cpsr = %#010x\n", c);
	memcpy((void *)0xffff0000, (void *)0x40008000, 0x200);

	__asm__ __volatile__ (
		".word 0x77777777\n"
	);

	printf("I am back!\n");
#if 1
	__asm__ __volatile__ (
		/* instruction encoding farmat [xxxx|1111|no] */
		"swi 1\n"
		"swi 2\n"
		"svc 3\n" /* svc 等同于swi */
		"svc 4\n"
	);
#endif

	return 0;
}
