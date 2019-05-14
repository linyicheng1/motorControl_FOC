#ifndef __CAN_H__
#define __CAN_H__
#define CAN_ID_CRASH 0x64
void CAN_Configuration(void);
void CANWriteData(unsigned char add,char *s);
void CANSendCrash(short send);

#endif 
