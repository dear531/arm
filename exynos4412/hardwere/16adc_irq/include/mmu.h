#ifndef __MMU_H__
#define __MMU_H__
void section_create_table(unsigned int *add);
void section_table_mmap(unsigned int va, unsigned pa);
void mmu_enable();
void mmu_disable();

#endif
