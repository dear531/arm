#ifndef __chipid_h__
#define __chipid_h__
#define ID_BASE_ADDRESS	0x10000000
#define PRO_ID		(*(volatile unsigned int *)(ID_BASE_ADDRESS + 0x0000))
#define PACKAGE_ID	(*(volatile unsigned int *)(ID_BASE_ADDRESS + 0x0004))
extern void printf_chip_id(void);
#endif
