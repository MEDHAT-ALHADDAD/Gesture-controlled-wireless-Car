
#ifndef I2C_H_
#define I2C_H_

#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include "stdio.h"
//*****************************************************************************
//
// I2C Driver Functions
//
//*****************************************************************************
void I2C_Init();


char I2C_Write(int slaveAddr, char memAddr, char data);
char I2C_Read(int slaveAddr, char memAddr, int byteCount, char* data);
#endif /* I2C_H_ */
