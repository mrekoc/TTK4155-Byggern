#include <avr/io.h>
#include <stdio.h>

#ifndef ADC_33
#define ADC_33

typedef enum {CHANNEL0, CHANNEL1, CHANNEL2, CHANNEL3} channel_t;
	
void adc_init(void);	
char ADC_read(channel_t);

#endif