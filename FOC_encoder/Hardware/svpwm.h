#ifndef __SVPWM_H__
#define __SVPWM_H__
#include <stdio.h>
#include "stm32f10x.h"
#include "pwm.h"
#include "adc.h"
#include "math.h"
#include "FOC_type.h"

//六个扇区的宏定义
#define SECTOR_1	(u8)1
#define SECTOR_2	(u8)2
#define SECTOR_3	(u8)3
#define SECTOR_4	(u8)4
#define SECTOR_5	(u8)5
#define SECTOR_6	(u8)6

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

#define T		    (PWM_PERIOD * 4)//45000*4=180 000
#define T_SQRT3     (u16)(T * 1.732051)//T*sqrt(3)
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))

// Setting for sampling of VBUS and Temp after currents sampling
#define PHASE_A_MSK       (u32)((u32)(PHASE_A_ADC_CHANNEL) << 10)
#define PHASE_B_MSK       (u32)((u32)(PHASE_B_ADC_CHANNEL) << 10)
#define PHASE_C_MSK       (u32)((u32)(PHASE_C_ADC_CHANNEL) << 10)

typedef struct SVPWM{
		uint8_t  bSector;//目前所处的扇区
        int32_t wUAlpha;
        int32_t wUBeta;  	
}SVPWM;			
Curr_Components SVPWM_3ShuntGetPhaseCurrentValues(void);
void SVPWM_3ShuntCalcDutyCycles(Volt_Components Stat_Volt_Input);
#endif //__SVPWM_H__
