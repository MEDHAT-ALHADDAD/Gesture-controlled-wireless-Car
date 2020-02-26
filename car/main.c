#include "tm4c123gh6pm.h"
#include "main.h"

//IN1, PE0 Fowards, RightSide
//IN2, PE1 Backwards, RightSide
//IN3, PE2 Forwards, LeftSide
//IN4, PE3 Backwards, LeftSide
//PWM PinS, PB6.
//IN4,IN3,IN2,IN1
//Right= 0110
//Left= 1001
//Fowards= 0101
//Backwards= 1010

void PWM_init(int cmp){
  SYSCTL_RCGC0_R |=(0x100000);
  SYSCTL_RCC_R |= 0x140000;  
  GPIO_PORTB_AFSEL_R |=(0x40);
  GPIO_PORTB_PCTL_R |=(0x04000000);
  PWM0_0_CTL_R &= ~(0x01);
  PWM0_0_CTL_R &= ~(0x02);
  PWM0_0_GENA_R |= (0xC2);
  PWM0_0_LOAD_R = 1000-1;
  PWM0_0_CMPA_R = cmp-1;
  PWM0_0_CTL_R |=(0x01);
  PWM0_ENABLE_R |= (PWM_ENABLE_PWM0EN);
  
}
int main()
{
  UART_Init();
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
  GPIO_PORTE_DEN_R |= (0xFF);
  GPIO_PORTE_DIR_R |= (0xFF);
  PWM_init(1);
  uint8_t x ;
  HBridge &= ~(0xFF);
 while(1){
   
   
  x = UART_InChar();
 // printf("%x\n",x);
  if(x/16 == 0x8){ //Right
    HBridge &= ~(0xFF);
    HBridge |=(1<<2) | (1<<0);
    if(x%16== 0x1){ //25%
      //PWM_init(750);
      PWM0_0_CMPA_R= 750-1;
      
    }
    else if(x%16 == 0x02){ //50%
      //PWM_init(500);
      PWM0_0_CMPA_R=500-1;
    }
    else if(x%16== 0x04){ //75%
      //PWM_init(250);
       PWM0_0_CMPA_R=250-1;
    }
    else{
      //PWM_init(1);
       PWM0_0_CMPA_R=0;
    }
  }
  else if(x/16 == 0x4){ //Left 
    HBridge &= ~(0xFF);
    HBridge |=(1<<3) | (1<<1);
    
    if(x%16== 0x1){ //25%
      //PWM_init(750);
       PWM0_0_CMPA_R=750-1;
    }
    else if(x%16 == 0x02){
      //PWM_init(500);
     PWM0_0_CMPA_R=500-1;
    }
    else if(x%16== 0x04){
      //PWM_init(250);
     PWM0_0_CMPA_R=250-1;
    }
    else{
      //PWM_init(1);
      PWM0_0_CMPA_R=0;
    }
  }
  else if(x/16 == 0x02){ //Fowards
    HBridge &= ~(0xFF);
    HBridge |=(1<<3) | (1<<0);
    if(x%16== 0x1){ //25%
      //PWM_init(750);
      PWM0_0_CMPA_R=750-1;

    }
    else if(x%16 == 0x02){
      //PWM_init(500);
      PWM0_0_CMPA_R=500-1;

    }
    else if(x%16== 0x04){
      //PWM_init(250);
     PWM0_0_CMPA_R=250-1;

    }
    else{
      //PWM_init(1);
      PWM0_0_CMPA_R=0;

    }
  }
   else if(x/16 == 0x01){ //Backwards
     HBridge &= ~(0xFF);
     HBridge |=(1<<2) | (1<<1);
   //HBridge &= ~(0x05);
    if(x%16== 0x1){ //25%
      //PWM_init(750);
     PWM0_0_CMPA_R=750-1;

    }
    else if(x%16 == 0x02){
      //PWM_init(500);
     PWM0_0_CMPA_R=500-1;

    }
    else if(x%16== 0x04){
      //PWM_init(250);
    PWM0_0_CMPA_R=250-1;

    }
    else{
      //PWM_init(1);
    PWM0_0_CMPA_R=0;

    }
  }
   else{ //Free Engine Stop
     HBridge &= ~(0xFF);
    PWM_init(1000);
     
   }
  delayMs2(10);
  }
}
