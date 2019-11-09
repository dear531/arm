void draw_point(int fb, int x, int y, int r, int g, int b)
{
        unsigned short *v = 0;
        if (0 == fb)
                v = (void*) 0x41000000;
        else
                v = (void*) 0x42000000;
        //*(v + y * 480 + x) = r << 11 | g < 5 | b;
        *(v + y * 480 + x) = 0xffff;
}

int main(void)
{
#define LCD_BASE	0x11C00000
#define VIDCON0          (*(volatile unsigned int*)(LCD_BASE + 0x00))
#define VIDCON1          (*(volatile unsigned int*)(LCD_BASE + 0x04))
#define VIDTCON0         (*(volatile unsigned int*)(LCD_BASE + 0x10))
#define VIDTCON1         (*(volatile unsigned int*)(LCD_BASE + 0x14))
#define VIDTCON2         (*(volatile unsigned int*)(LCD_BASE + 0x18))
#define WINCON0          (*(volatile unsigned int*)(LCD_BASE + 0x20))
#define VIDOSD0C         (*(volatile unsigned int*)(LCD_BASE + 0x48))
#define SHADOWCON        (*(volatile unsigned int*)(LCD_BASE + 0x34))
#define WINCHMAP2        (*(volatile unsigned int*)(LCD_BASE + 0x3c))
#define VIDOSD0A         (*(volatile unsigned int*)(LCD_BASE + 0x40))
#define VIDOSD0B         (*(volatile unsigned int*)(LCD_BASE + 0x44))
#define VIDW00ADD0B0     (*(volatile unsigned int*)(LCD_BASE + 0xA0))
#define VIDW00ADD1B0     (*(volatile unsigned int*)(LCD_BASE + 0xD0))

#define CLK_BASE 0x10030000
#define CLK_SRC_LCD0     (*(volatile unsigned int*)(CLK_BASE + 0x234))
#define CLK_SRC_MASK_LCD (*(volatile unsigned int*)(CLK_BASE + 0x334))
#define CLK_DIV_LCD      (*(volatile unsigned int*)(CLK_BASE + 0x534))
#define CLK_GATE_IP_LCD  (*(volatile unsigned int*)(CLK_BASE + 0x934))

#define BITS_PER_PIXEL   32

#define LCDBLK_BASE	0x10010000
#define LCDBLK_CFG       (*(volatile unsigned int*)(LCDBLK_BASE + 0x00))
#define LCDBLK_CFG2      (*(volatile unsigned int*)(LCDBLK_BASE + 0x04))

#define LCD_LENTH        1024
#define LCD_WIDTH        600
	/* enable display */
	VIDCON0 |= 0x3;
	/* set CLKVAL = 87 --> VCLK = FIMD  SCLK/(CLKVAL+1)*/
	VIDCON0 &= (0xff << 6);
	VIDCON0 |= 87;
	/*
	* VIDTCON1:
	* [5]:IVSYNC  ===> 1 : Inverted(反转)
	* [6]:IHSYNC  ===> 1 : Inverted(反转)
	* [7]:IVCLK   ===> 1 : Fetches video data at VCLK rising edge (下降沿触发)
	* [10:9]:FIXVCLK  ====> 01 : VCLK running
	*/
	VIDCON1	|= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 9);
#if 0
VSPW+1          tvp(2)  10
VBPD+1          tvb(2)  2
LINEVAL+1       tvd      272
VFPD+1          tvf     2
HSPW+1          thp(2)  41
NBPD+1          thb(2)  2
NOZVAL+1        thd     480
NFPD+1          thf     2
#endif
#if 0
VBPD [23:16] RW Vertical back porch specifies the number of inactive lines at the start of a frame after vertical synchronization period. 0x00 
VFPD [15:8] RW Vertical front porch specifies the number of inactive lines at the end of a frame before vertical synchronization period. 0x00 
VSPW [7:0] RW Vertical synchronization pulse width determines the high-
level width of VSYNC pulse by counting the number of inactive lines. 
#endif
        VIDTCON0 |= (1 << 16) | (1 << 8) | 9;
#if 0
HBPD [23:16] RW Horizontal back porch specifies the number of VCLK periods between the falling edge of HSYNC and start of active data. 0x00 
HFPD [15:8] RW Horizontal front porch specifies the number of VCLK periods between the end of active data and rising edge of HSYNC. 0x00 
HSPW [7:0] RW Horizontal synchronization pulse width determines the high-level width of HSYNC pulse by counting the number of VCLK. 
#endif
        VIDTCON1 |= (1 << 16) | (1 << 8) | 40;
#if 0
LINEVAL [21:11] RW Determines vertical size of display. In the Interlace mode, (LINEVAL + 1) should be even. 0 
HOZVAL [10:0] RW Determines horizontal size of display. 0
#endif
        VIDTCON2 |= (271 << 11) | 479;
#if 0
ENLOCAL_F [22] RW Selects Data access method.  0 = Dedicated DMA 1 = Local Path 
BYTSWP_F [17] RW Specifies Byte swaps control bit.  0 = Disables swap 1 = Enables swap NOTE: It should be set to 0 when ENLOCAL is 1.  
HAWSWP_F [16] RW Specifies Half-Word swap control bit.  0 = Disables swap 1 = Enables swap NOTE: It should be set to 0 when ENLOCAL is 1. 
WSWP_F [15] RW Specifies Word swap control bit.  0 = Disables swap 1 = Enables swap NOTE: It should be set to 0 when ENLOCAL is 1. 
InRGB [13] RW Specifies input color space of source image.  (Only for "ENLOCAL" enable).  0 = RGB 1 = YCbCr 
#endif
	WINCON0 = (1 << 16) | (5 << 2) | 1;
#if 0
OSDSIZE [23:0] RW Specifies the Window Size For example, Height x Width (number of word)
#endif
	VIDOSD0C = (480 * 272 * 2) / 4;
#if 0
_PROTECT [10] RW Protects to update window 0s shadow register (xxx_F) 0 = Updates shadow register per frame 1 = Protects to update (update shadow register at next frame after "SHADOW_PROTECT" turns to be 1b0)
C0_ENLOCAL_F [5] RW Enables Channel 0 Local Path.  0 = Disables 1 = Enables 
C0_EN_F [0] RW Enables Channel 0.  0 = Disables 1 = Enables 
#endif
        SHADOWCON &= ~(1 << 10);
        SHADOWCON &= ~(1 << 5);
        SHADOWCON |= 1;
#if 0
CH0FISEL [18:16] RW Selects Channel 0s channel.  001 = Window 0 010 = Window 1 101 = Window 2 110 = Window 3 111 = Window 4 
W0FISEL [2:0] RW Selects Window 0s channel.  001 = Channel 0 010 = Channel 1 101 = Channel 2 110 = Channel 3 111 = Channel 4 
#endif
        WINCHMAP2 &= ~(7 << 16);
        WINCHMAP2 |= (1 << 16);
        WINCHMAP2 &= ~(7 << 0);
        WINCHMAP2 |= (1 << 0);
#if 0
OSD_LeftTopX_F [21:11] RW Specifies the horizontal screen coordinate for left top pixel of OSD image. 0 
OSD_LeftTopY_F [10:0] RW Specifies the vertical screen coordinate for left top pixel of OSD image. 
(For interlace TV output, this value should be set to half of the original screen y coordinate. The original screen y coordinate should be even.) 
#endif
        VIDOSD0A = 0;
#if 0
RightBotX_F [21:11] RW Specifies horizontal screen coordinate for right bottom pixel of OSD image. 0 
RightBotY_F [10:0] RW Specifies vertical screen coordinate for right bottom pixel of OSD image. 
(For interlace TV output, this value should be set to half of the original screen y coordinate. The original screen y coordinate should be odd value.) 
#endif
        VIDOSD0B = (479 << 11) | 271;
#if 0
VIDW00ADD0B0 0x00A0 Specifies window 0s buffer start address register, buffer 0.
#endif
	VIDW00ADD0B0 = 0x41000000;
#if 0
VIDW00ADD1B0 0x00D0 Specifies window 0s buffer end address register, buffer 0.
#endif
	VIDW00ADD1B0 = 0x41000000 + 480 * 272 * 2;
#if 0
FIMD0_SEL [3:0] RW Controls MUXFIMD0 0000 = XXTI 0001 = XusbXTI 0010 = SCLK_HDMI24M 0011 = SCLK_USBPHY0 0101 = SCLK_HDMIPHY 0110 = SCLKMPLL_USER_T 0111 = SCLKEPLL
#endif
	CLK_SRC_LCD0 &= ~0x0f;
	CLK_SRC_LCD0 |= 0x06;
        /*
         * FIMD0_RATIO [3:0] RW DIVFIMD0 Clock Divider Ratio 
         * SCLK_FIMD0 = MOUTFIMD0/(FIMD0_RATIO + 1) 0x0
         */
        CLK_DIV_LCD &= ~0xf;
        /*
         *FIMD0_MASK [0] RW Mask output clock of MUXFIMD0
         * 0 = Mask 1 = Unmask
         */
        CLK_SRC_MASK_LCD |= 1;
        /*
        * CLK_FIMD0 [0] RW Gating all clocks for FIMD0 
        * 0 = Mask 1 = Pass
        */
        CLK_GATE_IP_LCD |= 1;
        /*
         * FIMDBYPASS_LBLK0 [1] RW FIMD of LBLK0 Bypass Selection 
         * 0 = MIE/MDNIE 1 = FIMD Bypass
        */
        LCDBLK_CFG |= (1 << 1);
#if 0
MIE0_DISPON [0] RW MIE0_DISPON: PWM output control 0 = Disable 1 = PWM outpupt enable 0x1
#endif
	LCDBLK_CFG2 |= (1 << 0);
	int x = 0, y = 0;
	for (x = 0; 480 > x; x++) {
		for (y = 0; 272 > y; y++) {
			draw_point(0, x, y, 0xf, 0xf, 0xf);
		}
	}

	return 0;
}
