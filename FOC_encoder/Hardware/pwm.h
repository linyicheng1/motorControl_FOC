#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"
#include "svpwm.h"
#define PWM_PERIOD  1800//72 000 000---10kHZ的控制频率
#define DEADTIME_NS	((u16) 500)  //in nsec; range is [0...3500]
#define DEADTIME  (u16)((unsigned long long)72000000uL/2 \
          *(unsigned long long)DEADTIME_NS/1000000000uL) //通过DEADTIME_NS计算得到的寄存器的值

void PWM_Motor_Configuration(void);
void set_PWM_x(int chx , uint16_t pwm);
void CHx_ENABLE(int chx , int8_t enable);
#endif
