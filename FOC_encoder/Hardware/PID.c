#include "pid.h"

PID pid_P;
PID pid_Position;
void PID_Init(void)
{
	pid_P.P = 0.0f;
	pid_P.I = 0.0f;
	pid_P.D = 0.0f;
	pid_P.IMax = 0.0f;
	pid_P.SetPoint =0.0f;
	
	pid_Position.P = 0.005f;
	pid_Position.I = 0.002f;
	pid_Position.D = 0.005f;
	pid_Position.IMax = 1000.0f;
	pid_Position.SetPoint =0.0f;
}
float PIDCalc(PID *P, float NextPoint)
{
		float dError,Error;
		Error = P->SetPoint - NextPoint;
		
		dError = Error-P->LastError;
		P->PreError = P->LastError;
		P->LastError = Error;
		P->SumError+= Error;
		if(P->SumError >= P->IMax)
			P->SumError = P->IMax;
		else if(P->SumError <= -P->IMax)
			P->SumError = -P->IMax;
		
		P->POut = P->P*Error;
		P->IOut = P->I*P->SumError;
		P->DOut = P->D*dError;
		
		return P->POut+P->IOut+P->DOut;
}
void Cal(void)
{
	
}
