#include <common.h>
#include <clock.h>

void clock_for_uart2(void)
{
	printf("CLK_DIV_PERIL0\t%#010x\n", CLK_DIV_PERIL0);
	printf("CLK_SRC_PERIL0\t%#010x\n", CLK_SRC_PERIL0);
	printf("CLK_SRC_TOP1\t%#010x\n", CLK_SRC_TOP1);
	printf("CLK_SRC_DMC\t%#010x\n", CLK_SRC_DMC);
	printf("MPLL_CON0\t%#010x\n", MPLL_CON0);

	return;
}

void clock_for_lcd(void)
{
	/*
	 * FIMD0_SEL [3:0] RW Controls MUXFIMD0 
	 * 0000 = XXTI 
	 * 0001 = XusbXTI 
	 * 0010 = SCLK_HDMI24M 
	 * 0011 = SCLK_USBPHY0 
	 * 0101 = SCLK_HDMIPHY 
	 * 0110 = SCLKMPLL_USER_T 
	 * 0111 = SCLKEPLL 
	 */
	/* XusbXTI = 24MHz from coreborad pdf  */
	printf("CLK_SRC_LCD0\t%#010x\n", CLK_SRC_LCD0);
	/*
	 * FIMD0_RATIO [3:0] RW DIVFIMD0 Clock Divider Ratio 
	 * SCLK_FIMD0 = MOUTFIMD0/(FIMD0_RATIO + 1) 0x0
	 */
	CLK_DIV_LCD &= ~0xf;
	CLK_DIV_LCD |= 1;
	printf("CLK_DIV_LCD\t%#010x\n", CLK_DIV_LCD);
	/* lcd clock SCLK_FIMD0 = 24MHz / 2 = 12M */
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

	while (CLK_DIV_STAT_LCD & 1) {
		/* 等待DIVFIMD0分频器状态稳定 */
	}
}
