#ifndef IMU_H_
#define IMU_H_

#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include "I2C.h"
#include <math.h>
#include "stdio.h"

#define SLAVE_ADDR 0x68


void  IMU_Init();
void  IMU_Calibrate();
void  IMU_CalculateAcc();
float IMU_GetAccX();
float IMU_GetAccY();

void delayMs(int n);
#endif