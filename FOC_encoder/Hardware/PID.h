#ifndef __PID_H
#define __PID_H

typedef struct PID{
		float SetPoint;			
		
		float P;						
		float I;						
		float D;						
		
		float LastError;		
		float PreError;			
		float SumError;			

		float IMax;					
		
		float POut;					
		float IOut;					
		float DOut;					
}PID;			

void PID_Init(void);
float PIDCalc(PID *P, float NextPoint);

#endif
