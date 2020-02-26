#include "systick.h"



void delayUs(int us){
  NVIC_ST_CTRL_R=0;
  int delay = us * 15;
  NVIC_ST_RELOAD_R = delay;
  NVIC_ST_CTRL_R = 5; //7 for interrupts
  NVIC_ST_CURRENT_R = 0;
  while ((NVIC_ST_CTRL_R & 0x10000) == 0);
}

void delayMs2(int ms){
  NVIC_ST_CTRL_R=0;
  int delay = ms * 15999;
  NVIC_ST_RELOAD_R = delay;
  NVIC_ST_CTRL_R = 5; //7 for interrupts
  NVIC_ST_CURRENT_R = 0;
  while ((NVIC_ST_CTRL_R & 0x10000) == 0);
}

void delayS(void){
  NVIC_ST_CTRL_R=0;
  int delay = 15999999;
  NVIC_ST_RELOAD_R = delay;
  NVIC_ST_CTRL_R = 5; //7 for interrupts
  NVIC_ST_CURRENT_R = 0;
  while ((NVIC_ST_CTRL_R & 0x10000) == 0);
}