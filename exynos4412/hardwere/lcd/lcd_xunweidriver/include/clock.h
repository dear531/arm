#ifndef ___CLOCK_H__
#define ___CLOCK_H__

/* MUX选择器寄存器名字开头都是SRC加块名组合，分频器寄存器名字都是块名字加DIV组合 */
#define CLK_DIV_PERIL0 (*(volatile unsigned int *)(0x10030000 + 0xC550))
#define CLK_SRC_PERIL0 (*(volatile unsigned int *)(0x10030000 + 0xC250))
#define CLK_SRC_TOP1 (*(volatile unsigned int *)(0x10030000 + 0xC214))
#define CLK_SRC_DMC (*(volatile unsigned int *)(0x10040000 + 0x0200))
#define MPLL_CON0 (*(volatile unsigned int *)(0x10040000 + 0x0108))

#define CLK_SRC_LCD0 (*(volatile unsigned int *)(0x10030000 + 0xC234))
#define CLK_DIV_LCD (*(volatile unsigned int *)(0x10030000 + 0xC534))
#define CLK_SRC_MASK_LCD (*(volatile unsigned int *)(0x10030000 + 0xC334))
#define CLK_DIV_STAT_LCD (*(volatile unsigned int *)(0x10030000 + 0xC634))
#define CLK_GATE_IP_LCD (*(volatile unsigned int *)(0x10030000 + 0xC934))
#define LCDBLK_CFG (*(volatile unsigned int *)(0x10010000 + 0x0210))
#define LCDBLK_CFG2 (*(volatile unsigned int *)(0x10010000 + 0x0214))




#if 0
寄存器值：
CLK_DIV_PERIL0  0x00077777
CLK_SRC_PERIL0  0x00066666
CLK_SRC_TOP1    0x00011000
CLK_SRC_DMC     0x00011000
MPLL_CON0       0xa0640300
1.SCLK_UART0~4 -> 
	DIVUART0~4(是PERIL_BLK所在的块，通过图可看到是分频器DIVUART0~4，决定了其分频大小) -> 
	寄存器值：CLK_DIV_PERIL0 0x077777:SCLK_UART2 = MOUTUART2/(UART2_RATIO + 1):8倍
2.MUXUART0~4(还是通过PERIL_BLK所在的块图，察看到选择器MUXUART0~4决定了其上级频率输入) ->
	寄存器值：CLK_SRC_PERIL0 0x066666:0110 = SCLKMPLL_USER_T ->
3.SCLKMPLL_USER_T(所在块为CMU_TOP，为时钟管理单元的top块名，决定其上层输入选择器MUXMPLL_CTRL_USER_T) -> MUXMPLL_CTRL_USER_T(根据块名top和选择器src名组合得到src_top选择器，去找寄存器是这个的，哪个选择了MUXMPLL_CTRL_USER_T
4.CLK_SRC_TOP1:MUX_MPLL_USER_SEL_T | [12] | RW | Controls MUXMPLL 0 = FINPLL 1 = SCLKMPLLL
	寄存器值：CLK_SRC_TOP1 0x011000(即第12位为1)，选择了SCLKMPLL锁相环作为输入
5.在图块CMU_DMC中找到SCLKMPLL的输入是由MUXMPLL选择器决定的，按选择器名src+块名dmc查找寄存器
	找到寄存器CLK_SRC_DMC:MUX_MPLL_SEL [12] RW Controls MUXMPLL 0 = FINPLL 1 = MOUTMPLLFOUT
	第12位值为1，选择了MPLL锁项环为输入
6.MPLL锁项环的输入频率察看：是由MPLL_CON0决定
寄存器值为： MPLL_CON0       0xa0640300
MDIV [25:16] RW PLL M Divide Value 0x64
PDIV [13:8] RW PLL P Divide Value 0x3
SDIV [2:0] RW PLL S Divide Value 0x0
FOUT = MDIV x FIN/(PDIV x 2^SDIV)
MDIV=0x64 = 100
PDIV=0x03 = 3
SDIV=0
100 x 24M / (3 X 2 ^ 0) = 100 x 24M / 3 = 800M

800M经过分频器分频最终输出给串口，结果为100M

#endif

extern void clock_for_uart2(void);

#endif
