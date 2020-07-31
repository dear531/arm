#include <lcd.h>
#include <common.h>

void lcd_init(void)
{
	/* LCD_VCLK LCD_VDEN LCD_VSYNC LCD_HSYNC */	
	GPF0CON &=~0xffff;
	GPF0CON |=0x2222;
	/* LCD_VD3-LCD_VD7 */
	GPF0CON &=~0xf0000000;
	GPF0CON |=0x20000000;
	GPF1CON &=~0xffff;
	GPF1CON |=0x2222;
	/* LCD_VD10-LCD_VD15 */
	GPF1CON &=~0xff000000;
	GPF1CON |=0x22000000;
	GPF2CON &=~0xffff;
	GPF2CON |=0x2222;
	/* LCD_VD19-LCD_VD23 */
	GPF2CON &=~0xf0000000;
	GPF2CON |=0x20000000;
	GPF3CON &=~0xffff;
	GPF3CON |=0x2222;
}

void lcd_for_time(void)
{

#if 0
DSI_EN [30] RW Enables MIPI DSI. 
0 = Disables 1 = Enables (i80 24-bit data interface, SYS_ADD[1]) 
VIDOUT [28:26] RW Determines output format of Video Controller. 
000 = RGB interface 
001 = Reserved 
010 = Indirect i80 interface for LDI0 
011 = Indirect i80 interface for LDI1 
100 = Write-Back interface and RGB interface 
101 = Reserved 
110 = WB Interface and i80 interface for LDI0 
111 = WB Interface and i80 interface for LDI1 
/*
Selects display mode (VIDOUT[1:0] == 2'b00). 
*/
0 = RGB parallel format 
1 = RGB serial format 
Name Bit Type Description Reset Value 
0 = RGB parallel format 

PNRMODE [17] RW Controls inverting RGB_ORDER (atVIDCON3). 
0 = Normal: RGBORDER[2] atVIDCON3 
1 = Invert: to RGBORDER[2] atVIDCON3 
NOTE: You can use this bit for the previous version of FIMD. 
You do not have to use this bit if you use RGB_ORDER 
atVIDCON3 register. 

CLKVALUP [16] RW Selects CLKVAL_F update timing control. 
0 = Always 
1 = Start of a frame (only once per frame) 

0 

CLKVAL_F [13:6] RW Determines rates of VCLK and CLKVAL[7:0]. 
VCLK = FIMD  SCLK/(CLKVAL+1), where CLKVAL  1 
NOTE: The maximum frequency of VCLK is 80 MHz. (80 MHz 
for Display Controller) 
VCLKFREE [5] RW Controls VCLK Free Run (only valid at RGB IF mode). 
0 = Normal mode (controls using ENVID) 
1 = Free-run mode 
ENVID [1] RW 
Enables/disables video output and logic immediately. 
0 = Disables the video output and display control signal 
1 = Enables the video output and display control signal 

ENVID_F [0] RW 
Enables/disables video output and logic at current frame end. 
0 = Disables the video output and display control signal 
1 = Enables the video output and display control signal 
If this bit is set to "on" and "off", then "H" is Read and enables 
the video controller until the end of current frame. (NOTE) 
#endif

	VIDCON0 = 3;
#if 0
IVCLK [7] RW 
0 = Fetches video data at VCLK falling edge 
1 = Fetches video data at VCLK rising edge 
IHSYNC [6] RW Specifies HSYNC pulse polarity. 
0 = Normal 
1 = Inverted 
IVSYNC [5] RW Specifies VSYNC pulse polarity. 
0 = Normal 
1 = Inverted 
IVDEN [4] RW Specifies VDEN signal polarity. 
0 = Normal 
1 = Inverted 
#endif
	VIDCON1	= (1 << 7) | (1 << 6) | (1 << 5);
#if 0
VSPW+1	 	tvp(2) 	10
VBPD+1		tvb(2) 	2
LINEVAL+1	tvd 	 272
VFPD+1		tvf	2
HSPW+1		thp(2)	41
NBPD+1	 	thb(2)	2
NOZVAL+1	thd	480
NFPD+1		thf	2
#endif
#if 0
VBPD [23:16] RW Vertical back porch specifies the number of inactive lines at the start of a frame after vertical synchronization period. 0x00 
VFPD [15:8] RW Vertical front porch specifies the number of inactive lines at the end of a frame before vertical synchronization period. 0x00 
VSPW [7:0] RW Vertical synchronization pulse width determines the high-
level width of VSYNC pulse by counting the number of inactive lines. 
#endif
	VIDTCON0 = (1 << 16) | (1 << 8) | 9;
#if 0
HBPD [23:16] RW Horizontal back porch specifies the number of VCLK periods between the falling edge of HSYNC and start of active data. 0x00 
HFPD [15:8] RW Horizontal front porch specifies the number of VCLK periods between the end of active data and rising edge of HSYNC. 0x00 
HSPW [7:0] RW Horizontal synchronization pulse width determines the high-level width of HSYNC pulse by counting the number of VCLK. 
#endif
	VIDTCON1 = (1 << 16) | (1 << 8) | 40;
#if 0
LINEVAL [21:11] RW Determines vertical size of display. In the Interlace mode, (LINEVAL + 1) should be even. 0 
HOZVAL [10:0] RW Determines horizontal size of display. 0
#endif
	VIDTCON2 = (271 << 11) | 479;

}

/* fd 0 */
/* 0x60000000*/
void lcd_wind0_init(void)
{
	WINCON0 = (1 << 16) | (5 << 2) | 1;
	SHADOWCON &= ~(1 << 10);
	SHADOWCON &= ~(1 << 5);
	SHADOWCON |= 1;

	WINCHMAP2 &= ~(7 << 16);
	WINCHMAP2 |= (1 << 16);
	WINCHMAP2 &= ~(7 << 0);
	WINCHMAP2 |= (1 << 0);

	VIDOSD0A = 0;
	VIDOSD0B = (479 << 11) | 271;
	VIDOSD0C = (480 * 272 * 2) / 4;
	VIDOSD1D = 480 * 272;

	VIDW00ADD0B0 = 0x41000000;
	VIDW00ADD1B0 = 0x41000000 + 480 * 272 * 2;
}

/* fd 1 */
/* 0x70000000*/
void lcd_wind1_init(void)
{
	WINCON1 = (1 << 16) | (5 << 2) | 1;
	SHADOWCON &= ~(1 << 11);
	SHADOWCON &= ~(1 << 6);
	SHADOWCON |= (1 << 1);

	WINCHMAP2 &= ~(7 << 19);
	WINCHMAP2 |= (2 << 19);
	WINCHMAP2 &= ~(7 << 3);
	WINCHMAP2 |= (2 << 3);

	VIDOSD1A = 0;
	VIDOSD1B = (479 << 11) | 271;
	VIDOSD1C = 0x777 < 12;

	VIDW01ADD0B0 = 0x42000000;
	VIDW01ADD1B0 = 0x42000000 + 480 * 272 * 2;
}

void draw_point(int fb, int x, int y, int r, int g, int b)
{
	unsigned short *v = 0;
	if (0 == fb)
		v = (void*) 0x41000000;
	else
		v = (void*) 0x42000000;
	//*(v + y * 480 + x) = r << 11 | g < 5 | b;
	*(v + y * 480 + x) = 0xff;
}
