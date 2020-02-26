#include "imu.h"

float Acc_rawX, Acc_rawY, Acc_rawZ;   
float Acc_angle_error_x, Acc_angle_error_y;
float rad_to_deg = 180/3.141592654;

void IMU_Init()
{
    I2C_Write(SLAVE_ADDR, 0x6B, 0x01); // clock 8 mhz pll with x axis gyro reference
    delayMs(100);
    I2C_Write(SLAVE_ADDR, 0x68, 0x06); // signal path reset
    delayMs(100);
    I2C_Write(SLAVE_ADDR, 0x6A, 0x00); // i2c_if_dis = 0
    delayMs(100);
    I2C_Write(SLAVE_ADDR, 0x1A, 0x00); //fsync and dlpf disabled
    delayMs(100);
    I2C_Write(SLAVE_ADDR, 0x19, 0x07); //sample rate set to 1 khz
    delayMs(100);
    I2C_Write(SLAVE_ADDR, 0x1B, 0x00); // +/- 250dps gyrometer  configuration
    delayMs(100);
    I2C_Write(SLAVE_ADDR, 0x1C, 0x10); // +/- 2g  accelerometer configuration
    delayMs(100);
    IMU_Calibrate();
}
void IMU_Calibrate()
{
   char accel_data[6];
   int a;
   int16_t ax, ay, az;
   for( a=0; a<200; a++)
    {
      I2C_Read(SLAVE_ADDR, 0x3B, 6, accel_data);
      
      ax = (int16_t) ((accel_data[0] << 8) | (accel_data[1]));
      ay = (int16_t) ((accel_data[2] << 8) | (accel_data[3]));
      az = (int16_t) ((accel_data[4] << 8) | (accel_data[5]));


      Acc_rawX = (((float) ax) / 4096.0);
      Acc_rawY = (((float) ay) / 4096.0);
      Acc_rawZ = (((float) az) / 4096.0);
      /*---X---*/
     Acc_angle_error_x = Acc_angle_error_x + ((atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg));
      /*---Y---*/
     Acc_angle_error_y = Acc_angle_error_y + ((atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg)); 
      
      if(a==199)
      {
        Acc_angle_error_x = Acc_angle_error_x/200;
        Acc_angle_error_y = Acc_angle_error_y/200;
      }
    }
}

void delayMs(int n)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++)
        {
        } /* do nothing for 1 ms */
}
