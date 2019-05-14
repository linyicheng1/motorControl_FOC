#include "adc.h"
#include "spi.h"
#include "svpwm.h"
#include "transform.h"
unsigned short ADC_ConvertedValue[3];
/******************** hardware introduction ***********************************
* ADC1&ADC2
* Curent A --- channel 1 --- PA1
* Curent B --- channel 2 --- PA2
*******************************************************/
void ADC_config(void)
{
	GPIO_InitTypeDef gpio;
    ADC_InitTypeDef  adc;
    DMA_InitTypeDef  dma;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA, ENABLE); 
    gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);
	
	DMA_DeInit(DMA1_Channel1);
	dma.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
    dma.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = 2;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_High;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &dma);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);
	
	adc.ADC_Mode = ADC_Mode_Independent;	             
    adc.ADC_ScanConvMode = ENABLE;			//各通道顺序扫描			             
    adc.ADC_ContinuousConvMode = DISABLE;	               
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = 2;						//扫描三个通道
    ADC_Init(ADC1, &adc);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_28Cycles5);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_Cmd(ADC1, ENABLE);
  	ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);     
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
	NVIC_Init(&NVIC_InitStructure);
 
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ADC中断服务函数
  * @param  
  * @retval 
  */
Curr_Components Global_Currents_abc;
Curr_Components Global_Currents_Iqd;
Curr_Components Global_Currents_alphaBeta;
float Ia,Ib,Ic;
float Ialpha,Ibelta;
float Id,Iq;
extern struct as5048_data encoder;
extern unsigned short ADC_ConvertedValue[3];
short num = 0;
float aheadAngel = 0;
void ADC1_2_IRQHandler(void)
{
  if (ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET) 
	{
		if(DMA1_Channel1->CNDTR == 2)
		{
			Ia = (ADC_ConvertedValue[0] - 2048)*16/4096;
			Ib = (ADC_ConvertedValue[1] - 2048)*16/4096;
		}
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}
/**
  * @brief  
  * @param  
  * @retval 
  */

void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
    {
		Ia = (ADC_ConvertedValue[0] - 2048)*16/4096;
		Ib = (ADC_ConvertedValue[1] - 2048)*16/4096;
//		Ic = 0 - Ia - Ib;
//		Ialpha = Ia - (Ib + Ic)*0.5;
//		Ibelta = (Ib - Ic)*0.5*sqrt(3);
//		Id = Ialpha*cos(-(encoder.angle*14  + aheadAngel)*3.14159/180) + Ibelta*sin(-(encoder.angle*14  + aheadAngel)*3.14159/180);
//		Iq = -Ialpha*sin(-(encoder.angle*14  + aheadAngel)*3.14159/180) + Ibelta*cos(-(encoder.angle*14  + aheadAngel)*3.14159/180);
		DMA_ClearITPendingBit(DMA1_IT_GL1); //清除全部中断标志
    }
}

