#include "main.h"
#include "I2C.h"

#define SLAVE_ADDR 0x68



double accel_angle;
float axf, ayf, azf;
float Acc_angle_x, Acc_angle_y;  
extern float Acc_angle_error_x, Acc_angle_error_y;
extern float rad_to_deg;
int main()
{
    char accel_data[6];
    int16_t ax, ay, az;
    init();
    I2C_Init();
    IMU_Init();
    
    while (1)
    {
        I2C_Read(SLAVE_ADDR, 0x3B, 6, accel_data);

        ax = (int16_t) ((accel_data[0] << 8) | (accel_data[1]));
        ay = (int16_t) ((accel_data[2] << 8) | (accel_data[3]));
        az = (int16_t) ((accel_data[4] << 8) | (accel_data[5]));


        axf = (((float) ax) / 4096.0);
        ayf = (((float) ay) / 4096.0);
        azf = (((float) az) / 4096.0);
        
         /*---X---*/
        Acc_angle_x = (atan((ayf)/sqrt(pow((axf),2) + pow((azf),2)))*rad_to_deg);
        /*---Y---*/
        Acc_angle_y = (atan(-1*(axf)/sqrt(pow((ayf),2) + pow((azf),2)))*rad_to_deg);   
 
        printf("axf= %f \n",Acc_angle_x);
        printf("ayf= %f \n",Acc_angle_y);
        //printf("azf= %f \n",azf);
    }
}





void init(){
  SYSCTL_RCGCGPIO_R |= 0x08; /* enable clock to GPIOD */

    /* PORTD 1, 0 for I2C3 */

  GPIO_PORTD_AFSEL_R |= 0x03; /* PORTD 1, 0 for I2C3  */ //D0=scl D1=sda
  GPIO_PORTD_PCTL_R &= ~0x000000FF; /* PORTD 1, 0 for I2C3 */
  GPIO_PORTD_PCTL_R |= 0x00000033;
  GPIO_PORTD_DEN_R |= 0x03; /* PORTD 1, 0 as digital pins */
  GPIO_PORTD_ODR_R |= 0x02; /* PORTD 1 as open drain */

 
  
}
