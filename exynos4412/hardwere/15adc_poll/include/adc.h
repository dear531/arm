#ifndef __ADC_H__
#define __ADC_H__

extern void adc_init(void);
extern void adc_start(void);
extern void wait_for_adc(void);
extern int get_adc(void);

#endif
