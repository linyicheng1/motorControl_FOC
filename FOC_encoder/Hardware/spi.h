#ifndef __SPI_H__
#define __SPI_H__
#include "stm32f10x.h"
void SPI_Configuration(void);
struct as5048_data 
{
    uint8_t iserror;
    uint16_t value;
    uint16_t mag;
    uint8_t agc;
    double angle;
    
};
struct as5048_data CollectData(void);
#define CMD_ANGLE            0xffff
#define CMD_AGC              0x7ffd
#define CMD_MAG              0x7ffe
#define CMD_CLAER            0x4001
#define CMD_NOP              0xc000


#endif //__SPI_H__
