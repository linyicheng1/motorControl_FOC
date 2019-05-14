#include "transform.h"
/*******************************************************************************
* Function Name  : Clarke Transformation
* Description    : 三相电流转换到旋转坐标系电流下
*                  qIalpha = qIas
*                  qIbeta = -(2*qIbs+qIas)/sqrt(3)
* Input          : Stat_Curr_a_b
* Output         : Stat_Curr_alfa_beta
* Return         : none.
*******************************************************************************/
Curr_Components Clarke(Curr_Components Curr_Input)
{
  Curr_Components Curr_Output;
  
  s32 qIa_divSQRT3_tmp;
  s32 qIb_divSQRT3_tmp ;
  
  s16 qIa_divSQRT3;
  s16 qIb_divSQRT3 ;

  // qIalpha = qIas
  Curr_Output.qI_Component1= Curr_Input.qI_Component1;

  qIa_divSQRT3_tmp = divSQRT_3 * Curr_Input.qI_Component1; 
  qIa_divSQRT3_tmp /=32768;   
    
  qIb_divSQRT3_tmp = divSQRT_3 * Curr_Input.qI_Component2;
  qIb_divSQRT3_tmp /=32768;
  
  qIa_divSQRT3=((s16)(qIa_divSQRT3_tmp));		
  		
  qIb_divSQRT3=((s16)(qIb_divSQRT3_tmp));				
    
  //qIbeta = -(2*qIbs+qIas)/sqrt(3)
  Curr_Output.qI_Component2=(-(qIa_divSQRT3)-(qIb_divSQRT3)-(qIb_divSQRT3));
  
  return(Curr_Output); 
}

/*******************************************************************************
* Function Name  : Park Transformation
* Description    : Park变换，将旋转坐标系下的电流Ialpha，Ibeta 变换到固定坐标系下 Id Iq
*                  qId=qIalpha_tmp*sin(theta)+qIbeta_tmp*cos(Theta)
*                  qIq=qIalpha_tmp*cos(Theta)-qIbeta_tmp*sin(Theta)                 
* Input          : Stat_Curr_alfa_beta
* Output         : Stat_Curr_q_d.
* Return         : none.
*******************************************************************************/

Curr_Components Park(Curr_Components Curr_Input, s16 Theta)
{
  Curr_Components Curr_Output;
  s32 qId_tmp_1, qId_tmp_2;
  s32 qIq_tmp_1, qIq_tmp_2;     
  s16 qId_1, qId_2;  
  s16 qIq_1, qIq_2;  
  
  Trig_Components Vector_Components = Trig_Functions(Theta);
  
  //No overflow guaranteed
  qIq_tmp_1 = Curr_Input.qI_Component1 * Vector_Components.hCos;  	
  qIq_tmp_1 /= 32768;
  
  //No overflow guaranteed
  qIq_tmp_2 = Curr_Input.qI_Component2 *Vector_Components.hSin;
  qIq_tmp_2 /= 32768;
 
  qIq_1 = ((s16)(qIq_tmp_1));
  qIq_2 = ((s16)(qIq_tmp_2));

  //Iq component in Q1.15 Format 
  Curr_Output.qI_Component1 = ((qIq_1)-(qIq_2));	
  
  //No overflow guaranteed
  qId_tmp_1 = Curr_Input.qI_Component1 * Vector_Components.hSin;
  qId_tmp_1 /= 32768;
  
  //No overflow guaranteed
  qId_tmp_2 = Curr_Input.qI_Component2 * Vector_Components.hCos;
  qId_tmp_2 /= 32768;
  
  qId_1 = (s16)(qId_tmp_1);		
  
  qId_2 = (s16)(qId_tmp_2);					

   //Id component in Q1.15 Format   
  Curr_Output.qI_Component2 = ((qId_1)+(qId_2));  
  
  return (Curr_Output);
  
}
/*******************************************************************************
* Function Name  : RevPark_Circle_Limitation
* Description    : 对park逆变换的结果进行限幅
*                  Stat_Volt_q_d.qV_Component1^2 + Stat_Volt_q_d.qV_Component2^2 <= 32767^2
*                  Apply limitation if previous condition is not met,
*                  by keeping a constant ratio 
*                  Stat_Volt_q_d.qV_Component1/Stat_Volt_q_d.qV_Component2
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define START_INDEX     60
#define MAX_MODULE      31783
const u16 circle_limit_table[68]=     
{
	32483,32206,31936,31672,31415,31289,31041,30799,30563,30331,
	30105,29884,29668,29456,29352,29147,28947,28750,28557,28369,
	28183,28002,27824,27736,27563,27393,27226,27062,26901,26743,
	26588,26435,26360,26211,26065,25921,25780,25641,25504,25369,
	25236,25171,25041,24913,24788,24664,24542,24422,24303,24186,
	24129,24015,23902,23791,23681,23573,23467,23362,23258,23206,
	23105,23004,22905,22808,22711,22616,22521,22429
};
void RevPark_Circle_Limitation(Volt_Components Stat_Volt_q_d)
{
	s32 temp;
				 
	temp = Stat_Volt_q_d.qV_Component1 * Stat_Volt_q_d.qV_Component1 
				 + Stat_Volt_q_d.qV_Component2 * Stat_Volt_q_d.qV_Component2;  // min value 0, max value 2*32767*32767
				  
	if ( temp > (u32)(( MAX_MODULE * MAX_MODULE) ) ) // (Vd^2+Vq^2) > MAX_MODULE^2 ?
	   {
		   u16 index;
					  
		   temp /= (u32)(512*32768);  // min value START_INDEX, max value 127
		   temp -= START_INDEX ;   // min value 0, max value 127 - START_INDEX
		   index = circle_limit_table[(u8)temp];
					  
		   temp = (s16)Stat_Volt_q_d.qV_Component1 * (u16)(index); 
		   Stat_Volt_q_d.qV_Component1 = (s16)(temp/32768);  
					  
		   temp = (s16)Stat_Volt_q_d.qV_Component2 * (u16)(index); 
		   Stat_Volt_q_d.qV_Component2 = (s16)(temp/32768);  
	   }
} 

/*******************************************************************************
* Function Name  : Rev_Park Transformation
* Description    : part逆变换，把旋转坐标系下的Vq，Vd转换到固定坐标系下的Valfa，Vbeta
*                  qValfa=qVq*Cos(theta)+qVd*Sin(theta)
*                  qVbeta=-qVq*Sin(theta)+qVd*Cos(theta)                  
* Input          : Stat_Volt_q_d.
* Output         : Stat_Volt_a_b
* Return         : none.
*******************************************************************************/

Volt_Components Rev_Park(Volt_Components Volt_Input , s16 Theta)
{ 	
  s32 qValpha_tmp1,qValpha_tmp2,qVbeta_tmp1,qVbeta_tmp2;
  s16 qValpha_1,qValpha_2,qVbeta_1,qVbeta_2;
  Volt_Components Volt_Output;
	
  Trig_Components Vector_Components = Trig_Functions(Theta);
  //No overflow guaranteed
  qValpha_tmp1 = Volt_Input.qV_Component1 * Vector_Components.hCos;
  qValpha_tmp1 /= 32768;
  
  qValpha_tmp2 = Volt_Input.qV_Component2 * Vector_Components.hSin;
  qValpha_tmp2 /= 32768;
		
  qValpha_1 = (s16)(qValpha_tmp1);		
  qValpha_2 = (s16)(qValpha_tmp2);			

  Volt_Output.qV_Component1 = ((qValpha_1)+(qValpha_2));
 
  
  qVbeta_tmp1 = Volt_Input.qV_Component1 * Vector_Components.hSin;
  qVbeta_tmp1 /= 32768;
  
  qVbeta_tmp2 = Volt_Input.qV_Component2 * Vector_Components.hCos;
  qVbeta_tmp2 /= 32768;

  qVbeta_1 = (s16)(qVbeta_tmp1);				
  qVbeta_2 = (s16)(qVbeta_tmp2);
   				
  Volt_Output.qV_Component2 = -(qVbeta_1)+(qVbeta_2);
 
  return(Volt_Output);
}
const s16 hSin_Cos_Table[256] = SIN_COS_TABLE;
Trig_Components Trig_Functions(s16 hAngle)
{
  u16 hindex;
  Trig_Components Local_Components;
  
  /* 10 bit index computation  */  
  hindex = (u16)(hAngle + 32768);  
  hindex /= 64;      
  
  switch (hindex & SIN_MASK) 
  {
  case U0_90:
    Local_Components.hSin = hSin_Cos_Table[(u8)(hindex)];
    Local_Components.hCos = hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
    break;
  
  case U90_180:  
     Local_Components.hSin = hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
     Local_Components.hCos = -hSin_Cos_Table[(u8)(hindex)];
    break;
  
  case U180_270:
     Local_Components.hSin = -hSin_Cos_Table[(u8)(hindex)];
     Local_Components.hCos = -hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
    break;
  
  case U270_360:
     Local_Components.hSin =  -hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
     Local_Components.hCos =  hSin_Cos_Table[(u8)(hindex)]; 
    break;
  default:
    break;
  }
  return (Local_Components);
}
