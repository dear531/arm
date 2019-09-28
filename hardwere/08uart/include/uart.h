#ifndef __UART_H__
#define __UART_H__
#define GPA1CON	(*(volatile unsigned int *)(0x11400000 + 0x0020))
#define UART_BASE_ADDRESS(n) (0x13800000 + 0x10000 * n)
#define ULCON(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0000))
#define UCON(n)		(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0004))
#define UFCON(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0008))
#define UMCON(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x000C))
#define UTRSTAT(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0010))
#define UERSTAT(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0014))
#define UFSTAT(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0018))
#define UMSTAT(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x001C))
#define UTXH(n)		(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0020))
#define URXH(n)	  	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0024))
#define UBRDIV(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0028))
#define UFRACVAL(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x002C))
#define UINTP(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0030))
#define UINTSP(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0034))
#define UINTM(n)	(*(volatile unsigned int *)(UART_BASE_ADDRESS(n) + 0x0038))
void uart_init(int nu);
void uart_send(int nu, char c);
char uart_recv(int nu);
#endif
