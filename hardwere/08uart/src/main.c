#include <common.h>
#include <hardware.h>
#include <uart.h>
/*
 可以直接在minicom中输入字符，也可以直接向串口文件打印
 sudo -i
 echo "s" > /dev/ttyUSB0
*/
int main(void)
{
	int i = 0;
	char c = 0;
	uart_init(0);
#if 0
	c = uart_recv(2);
	uart_send(2, c);
	c = 'a';
	while ('z' >= c) {
		uart_send(2, c);
		udelay(1000 * 1000);
		c++;
	}
#endif
	uprintf("int:%d, char:%c, string:%s, hex:%#x, nagetive:%d\n",
		10001, 'A', "hello", 0xefffa501, -23101);
	

	return 0;
}
