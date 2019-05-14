#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#define PHASE_A_ADC_CHANNEL ADC_Channel_3
#define PHASE_B_ADC_CHANNEL ADC_Channel_6
#define PHASE_C_ADC_CHANNEL ADC_Channel_14
void ADC_config(void);
#endif


