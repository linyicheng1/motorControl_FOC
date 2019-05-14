#include "svpwm.h"

SVPWM svpwm;
extern unsigned short ADC_ConvertedValue[3];
/**
  * @brief  生成svpwm的函数       
  * @param  电压 Stat_Volt_Input
  * @retval 
  */
void SVPWM_3ShuntCalcDutyCycles(Volt_Components Stat_Volt_Input)
{
    int32_t wX,wY,wZ;
    uint16_t hTimePhA = 0,hTimePhB = 0,hTimePhC = 0;
	/***************计算出相电压***************/
    svpwm.wUAlpha = Stat_Volt_Input.qV_Component1 * T_SQRT3 ;
    svpwm.wUBeta = -(Stat_Volt_Input.qV_Component2 * T);
    wX = svpwm.wUBeta;
    wY = (svpwm.wUBeta + svpwm.wUAlpha)/2;
    wZ = (svpwm.wUBeta - svpwm.wUAlpha)/2;
	/***************判断电压矢量所处的扇区***************/
	if (wY<0)
    {
      if (wZ<0)
      {
        svpwm.bSector = SECTOR_5;
      }
      else if (wX<=0)
      {
        svpwm.bSector = SECTOR_4;
      }
      else // wX > 0
      {
        svpwm.bSector = SECTOR_3;
      }
    }
    else // wY > 0
    {
	if (wZ>=0)
     {
       svpwm.bSector = SECTOR_2;
     }
     else if (wX<=0)
     {  
       svpwm.bSector = SECTOR_6;
     }
     else // wX > 0
     {
       svpwm.bSector = SECTOR_1;
     }
    }
	/***************根据不同的扇区确定电压作用的时间***************/
   switch(svpwm.bSector)
   {   
    case SECTOR_1:
                hTimePhA = (T/8) + ((((T + wX) - wZ)/2)/131072);
				hTimePhB = hTimePhA + wZ/131072;
				hTimePhC = hTimePhB - wX/131072;       	
          break;
    case SECTOR_2:
                hTimePhA = (T/8) + ((((T + wY) - wZ)/2)/131072);
				hTimePhB = hTimePhA + wZ/131072;
				hTimePhC = hTimePhA - wY/131072;
          break;

    case SECTOR_3:
                hTimePhA = (T/8) + ((((T - wX) + wY)/2)/131072);
				hTimePhC = hTimePhA - wY/131072;
				hTimePhB = hTimePhC + wX/131072;
         break;
	
    case SECTOR_4:
                hTimePhA = (T/8) + ((((T + wX) - wZ)/2)/131072);
                hTimePhB = hTimePhA + wZ/131072;
                hTimePhC = hTimePhB - wX/131072;
         break;  
    
    case SECTOR_5:
                hTimePhA = (T/8) + ((((T + wY) - wZ)/2)/131072);
				hTimePhB = hTimePhA + wZ/131072;
				hTimePhC = hTimePhA - wY/131072;
		 break;
                
    case SECTOR_6:
                hTimePhA = (T/8) + ((((T - wX) + wY)/2)/131072);
				hTimePhC = hTimePhA - wY/131072;
				hTimePhB = hTimePhC + wX/131072;
    default:
		break;
   }
   /***************改变寄存器的值***************/
//   CH1_ENABLE(1);
//   CH2_ENABLE(1);
//   CH3_ENABLE(1);
   set_PWM_x(1,hTimePhA);
   set_PWM_x(2,hTimePhB);
   set_PWM_x(3,hTimePhC);
}
/*****************************************************************************
* Function Name  : SVPWM_3ShuntGetPhaseCurrentValues
* Description    : This function computes current values of Phase A and Phase B 
*                 in q1.15 format starting from values acquired from the A/D 
*                 Converter peripheral.
* Input          : None
* Output         : Stat_Curr_a_b
* Return         : None
*******************************************************************************/
Curr_Components SVPWM_3ShuntGetPhaseCurrentValues(void)
{
	Curr_Components Local_Stator_Currents;
	s32 wAux;

//	switch (svpwm.bSector)
//	{
//	case 4:
//	case 5: //Current on Phase C not accessible     
		// Ia = (hPhaseAOffset)-(ADC Channel 11 value)    
		wAux =(s32)(2048) - ADC_ConvertedValue[0];          
		//Saturation of Ia 
	    Local_Stator_Currents.qI_Component1 = LIMIT_MAX_MIN(wAux,S16_MAX,S16_MIN);                     
		// Ib = (hPhaseBOffset)-(ADC Channel 12 value)
		wAux = (s32)(2048)-ADC_ConvertedValue[1];
		// Saturation of Ib
	    Local_Stator_Currents.qI_Component2 = LIMIT_MAX_MIN(wAux,S16_MAX,S16_MIN);   
//			break;
//	case 6:
//	case 1:  //Current on Phase A not accessible     
//		// Ib = (hPhaseBOffset)-(ADC Channel 12 value)
//		wAux = (s32)(2048)-ADC_ConvertedValue[1];
//        //Saturation of Ib 
//		Local_Stator_Currents.qI_Component2 = LIMIT_MAX_MIN(wAux,S16_MAX,S16_MIN);
//		// Ia = -Ic -Ib 
//		wAux = ADC_ConvertedValue[2]-(s32)(2048) - Local_Stator_Currents.qI_Component2;
//        //Saturation of Ia
//	    Local_Stator_Currents.qI_Component1 = LIMIT_MAX_MIN(wAux,S16_MAX,S16_MIN);
//			break;       
//   case 2:
//   case 3:  // Current on Phase B not accessible
//		// Ia = (hPhaseAOffset)-(ADC Channel 11 value)     
//		wAux = (s32)(2048)-ADC_ConvertedValue[0];
//		//Saturation of Ia 
//		Local_Stator_Currents.qI_Component1 = LIMIT_MAX_MIN(wAux,S16_MAX,S16_MIN);  
//		// Ib = -Ic-Ia;
//		wAux = ADC_ConvertedValue[2] - (s32)(2048) - Local_Stator_Currents.qI_Component1;
//        // Saturation of Ib
//		Local_Stator_Currents.qI_Component2 = LIMIT_MAX_MIN(wAux,S16_MAX,S16_MIN);                    
//           break;
//   default:
//           break;
//   } 
  return(Local_Stator_Currents); 
}


 
