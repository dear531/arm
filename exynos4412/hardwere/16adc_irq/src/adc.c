#include <common.h>
#include <adc.h>
#include <irq.h>

#define ADC_BASE	0x126C0000 
#define ADCCON		(*(volatile unsigned int *)(ADC_BASE + 0x0000))
#define ADCDLY		(*(volatile unsigned int *)(ADC_BASE + 0x0008))
#define ADCDAT		(*(volatile unsigned int *)(ADC_BASE + 0x000C))
#define CLRINTADC	(*(volatile unsigned int *)(ADC_BASE + 0x0018))
#define ADCMUX		(*(volatile unsigned int *)(ADC_BASE + 0x001C))
#define IESR2 (*(volatile unsigned int *)(0x10440000 + 0x0020))
#define IECR2 (*(volatile unsigned int *)(0x10440000 + 0x0024))


void do_adc(void)
{
	int n = 0;
	n = get_adc();
#if 1
	printf("%d\n", n);
#endif
	/* 清理adc中断 */
	CLRINTADC = 1;
}

void adc_init(void)
{
#if 0
RES [16] RW ADC output resolution selection 0 = 10bit A/D conversion 1 = 12bit A/D conversion
PRSCEN [14] RW A/D converter prescaler enable 0 = Disable 1 = Enable 

PRSCVL [13:6] RW 
A/D converter prescaler value Data value: 19 ~ 255 
The division factor is (N+1) when the prescaler value is N. For 
example, ADC frequency is 5MHz if APB bus clock is 100MHz 
and the prescaler value is 19. 
Note: This A/D converter is designed to operate at maximum 
5MHz clock, so the prescaler value should be set such that the 
resulting clock does not exceed 5MHz. 

STANDBY [2] RW 
Standby mode select 
0 = Normal operation mode 
1 = Standby mode 
Note: In standby mode, prescaler should be disabled to reduce 
more leakage power consumption. 
#endif
	ADCCON = (1 << 16) | (1 << 14) | (25 << 6);
	ADCDLY = 0xffff;
#if 0
ADCMUX (ADC Channel Mux Register) 
Address = Base Address + 0x001C, Reset Value = 0x0000_0000 
Name Bit Type Description Reset Value 
SEL_MUX [3:0] RW 
Analog input channel select 
0000 = AIN 0 
0001 = AIN 1 
0010 = AIN 2 
0011 = AIN 3 
#endif
	ADCMUX = 0;
	/* enable combiner */
	IESR2 |= 1 << 19;
	request_irq(42, do_adc);
}

void adc_start(void)
{
#if 0
ENABLE_START [0] RW 
A/D conversion starts by enable. 
If READ_START is enabled, this value is not valid. 
0 = No operation 
1 = A/D conversion starts and this bit is automatically cleared 
after the start-up.
#endif
	ADCCON |= 1;
}

void wait_for_adc(void)
{
	while (0 == (ADCCON & (1 << 15)));

}

int get_adc(void)
{
	return ADCDAT & 0xfff;
}

