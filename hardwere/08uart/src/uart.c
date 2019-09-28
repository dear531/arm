#include <uart.h>

void uart_init(int nu)
{

#if 0
GPA1CON[1] [7:4] RW 

0x0 = Input 
0x1 = Output 
0x2 = UART_2_TXD 
0x3 = Reserved 
0x4 = UART_AUDIO_TXD 
0x5 to 0xE = Reserved 
0xF = EXT_INT2[1] 

0x00 

GPA1CON[0] [3:0] RW 

0x0 = Input 
0x1 = Output 
0x2 = UART_2_RXD 
0x3 = Reserved 
0x4 = UART_AUDIO_RXD 
0x5 to 0xE = Reserved 
0xF = EXT_INT2[0] 
	/* 1.配置时钟，100M省略 */
	/* 2.配置gpio为uart功能 */
#endif
	switch (nu) {
		case 2:
			GPA1CON	&= 0xff;
			GPA1CON |= 0x22;
			break;
		default:
			;
	}
	/* 3.8N1 */
	ULCON(nu) = 0x3;
	/*
	Transmit Mode [3:2] RW 
	Determines which function is able to Write Tx data to the 
	UART transmit buffer. 
	00 = Disables 
	01 = Interrupt request or polling mode 
	10 = DMA mode 
	11 = Reserved 
	Receive Mode [1:0] RW Determines which function is able to Read data from UART 
	00 = Disables 
	01 = Interrupt request or polling mode 
	10 = DMA mode 
	11 = Reserved 
	*/
	UCON(nu) = (1 << 0) | (1 << 2);
	/* 5.关闭FIFO FIFO Enable [0] RW 0 = Disables 1 = Enables */
	UFCON(nu) = 0;
	/* Auto Flow Control (AFC) [4] RW 0 = Disables 1 = Enables */
	UMCON(nu) = 0;
	/* 6.设置波特率115200
	 DIV_VAL = (SCLK_UART/(bps x16)) - 1 
	 Where, the divisor should be from 1 to (216 – 1). 
	 By using UFRACVALn, you can generate the Baud rate more accurately. 

	 For example, if the Baud rate is 115200 bps and SCLK_UART is 40 MHz, UBRDIVn and UFRACVALn are: 
	 DIV_VAL = (40000000/(115200 x 16)) – 1 
	 = 21.7 – 1 
	 = 20.7 
	 UBRDIVn = 20 (integer part of DIV_VAL) 
	 UFRACVALn/16 = 0.7 
	 Therefore, UFRACVALn = 11 
	*/
	UBRDIV(nu) = 53;
	UFRACVAL(nu) = 4;
}

void uart_send(int nu, char c)
{
	/* UTXHn [7:0] RWX Transmits data for UARTn */
	UTXH(nu) = c;
	/*
	buffer empty [1] R This bit is automatically
	set to 1 when transmit buffer is empty. 
	0 = Buffer is not empty 
	1 = Buffer is empty (in non-FIFO mode, it requests interrupt or DMA). 
	*/
	while (0 == (UTRSTAT(nu) & (1 << 1))) {}
}

char uart_recv(int nu)
{
	
	/*
	Receive buffer data ready [0] R 
	It automatically sets this bit to 1 when receive buffer contains 
	valid data, which is received over the RXDn port. 
	0 = Buffer is empty 
	1 = Buffer has a received data 
	*/
	while (0 == (UTRSTAT(nu) & (1 << 0))) {}
	/* URXHn [7:0] R Receives data for UARTn */
	return URXH(nu);
}

