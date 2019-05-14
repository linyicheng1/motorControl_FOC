#include "main.h"

int main(void)
{
	ADC_config();
	PWM_Motor_Configuration();
	while(SysTick_Config(36000)){}		//0.5ms定时中断,作为一个电机控制周期
    SPI_Configuration();
	while(1)
	{

	}
}
Volt_Components Stat_Volt_Input;
float theta;
float deltaTheta = 0;
float scale = 1;
struct as5048_data encoder;
short initFlag = 0;
float initAngle = 0;
float encoderAngleBias = 0;
float angleAhead = 90;
float testAnlge = 0;
short direct = 1;
void SysTick_Handler(void)
{
	encoder = CollectData();//6.4286
	if(initFlag <= 100)
	{
		Stat_Volt_Input.qV_Component1 =32768/2*sin(initAngle)*scale;
		Stat_Volt_Input.qV_Component2 =32768/2*cos(initAngle)*scale;
		encoderAngleBias += encoder.angle;		
		if(initFlag==100)
		{
			encoderAngleBias /= 101; 
		}
		initFlag ++;
	}
	else if(initFlag>100)
	{
		if(direct>0)
		{
			angleAhead = -90;
		}
		else if(direct<0)
		{
			angleAhead = 90;
		}
		theta = -(encoder.angle*14  + angleAhead)*3.14159/180;
		Stat_Volt_Input.qV_Component1 =32768/2*cos(theta)*scale;
		Stat_Volt_Input.qV_Component2 =-32768/2*sin(theta)*scale;
	}
}

