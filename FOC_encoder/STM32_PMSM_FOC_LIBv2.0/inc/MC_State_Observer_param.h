/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : MC_State_Observer_param.h
* Author             : FOCSDK development tool ver. 1.0.0 by IMS Systems Lab
* Creation date      : Thu Dec 01 14:52:47 2016
* Description        : Contains the PMSM State Observer related parameters
*                      (module MC_State_Observer_Interface.c)
*
********************************************************************************
* History:
* 21/11/07 v1.0
* 29/05/08 v2.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MC_STATE_OBSERVER_PARAM_H
#define __MC_STATE_OBSERVER_PARAM_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MAX_CURRENT 20.5             /* max current value, Amps */

// Values showed on LCD display must be here multiplied by 10 
#define K1 (s32)(-13752)             /* State Observer Gain 1 */
// Values showed on LCD display must be here multiplied by 100 
#define K2 (s32)(49789)             /* State Observer Gain 2 */

#define PLL_KP_GAIN (s16)(202*MOTOR_MAX_SPEED_RPM*POLE_PAIR_NUM/SAMPLING_FREQ)

#define PLL_KI_GAIN (s16)(1806742*POLE_PAIR_NUM/SAMPLING_FREQ * MOTOR_MAX_SPEED_RPM/SAMPLING_FREQ)

                                  
/******************* START-UP PARAMETERS ***************************************

              Speed /|\
FINAL_START_UP_SPEED |              /
                     |            /
                     |          /          
                     |        /	      
                     |      /          
                     |    / 
                     |  / 
                     |/_______________________________________\                       
                     0          FREQ_START_UP_DURATION      t /               */

#define FREQ_START_UP_DURATION    (u16) 1500 //in msec
#define FINAL_START_UP_SPEED      (u16) 1500 //Rotor mechanical speed (rpm)

/*
                 |I|/|\
                     |
      FINAL_I_STARTUP|       __________________    
                     |     /          
                     |    /	      
                     |   /          
                     |  / 
                     | / 
      FIRST_I_STARTUP|/ 
                     |_______________________________________________\                      
                     0 I_START_UP_DURATION  FREQ_START_UP_DURATION t /        */
                                                                              
// With MB459 phase current = (X_I_START_UP * 0.64)/(32767 * Rshunt)
#define FIRST_I_STARTUP           (u16) 1358
#define FINAL_I_STARTUP           (u16) 2118
#define I_START_UP_DURATION       (u16) 550 //in msec

// Alignment settings 
#ifdef NO_SPEED_SENSORS_ALIGNMENT

//Alignemnt duration
#define SLESS_T_ALIGNMENT           (u16) 100    // Alignment time in ms

#define SLESS_ALIGNMENT_ANGLE       (u16) 90 //Degrees [0..359]  
//  90° <-> Ia = SLESS_I_ALIGNMENT, Ib = Ic =-SLESS_I_ALIGNMENT/2) 

// With SLESS_ALIGNMENT_ANGLE equal to 90° final alignment 
// phase current = (SLESS_I_ALIGNMENT * 1.65/ Av)/(32767 * Rshunt)  
// being Av the voltage gain between Rshunt and A/D input
#define SLESS_I_ALIGNMENT           (u16) 2117

#endif

/**************************** STATISTIC PARAMETERS ****************************/
//Threshold for the speed measurement variance.   
#define VARIANCE_THRESHOLD        0.0625  //Percentage of mean value

// Number of consecutive tests on speed variance to be passed before start-up is
// validated. Checked every PWM period
#define NB_CONSECUTIVE_TESTS      (u16) 60
// Number of consecutive tests on speed variance before the variable containing
// speed reliability change status. Checked every SPEED_SAMPLING_TIME
#define RELIABILITY_HYSTERESYS    (u8)  4
//Minimum Rotor speed to validate the start-up
#define MINIMUM_SPEED_RPM             (u16) 580

#define F1 (s16)(16384)
#define F2 (s16)(2048)

//The parameters below shouldn't be modified
/*max phase voltage, 0-peak Volts*/
#define MAX_VOLTAGE (s16)((3.3/2)/BUS_ADC_CONV_RATIO) 

#define C1 (s32)((F1*RS)/(LS*SAMPLING_FREQ))
#define C2 (s32)((F1*K1)/SAMPLING_FREQ)
#define C3 (s32)((F1*MAX_BEMF_VOLTAGE)/(LS*MAX_CURRENT*SAMPLING_FREQ))
#define C4 (s32)((((K2*MAX_CURRENT)/(MAX_BEMF_VOLTAGE))*F2)/(SAMPLING_FREQ))
#define C5 (s32)((F1*MAX_VOLTAGE)/(LS*MAX_CURRENT*SAMPLING_FREQ))

#define MOTOR_MAX_SPEED_DPP (s32)((1.2*MOTOR_MAX_SPEED_RPM*65536*POLE_PAIR_NUM)\
                                                            /(SAMPLING_FREQ*60))

#define FREQ_STARTUP_PWM_STEPS (u32) ((FREQ_START_UP_DURATION * SAMPLING_FREQ)\
                                                                          /1000) 
#define FREQ_INC (u16) ((FINAL_START_UP_SPEED*POLE_PAIR_NUM*65536/60)\
                                                        /FREQ_STARTUP_PWM_STEPS)
#define I_STARTUP_PWM_STEPS (u32) ((I_START_UP_DURATION * SAMPLING_FREQ)/1000) 
#define I_INC (u16)((FINAL_I_STARTUP -FIRST_I_STARTUP)*1024/I_STARTUP_PWM_STEPS)
#define PERCENTAGE_FACTOR    (u16)(VARIANCE_THRESHOLD*128)      
#define MINIMUM_SPEED        (u16) (MINIMUM_SPEED_RPM/6)
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MC_STATE_OBSERVER_PARAM_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
