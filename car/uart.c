#include "uart.h"


void UART_Init(void){
  SYSCTL_RCGCUART_R |= 0x0002; // activate UART1
  SYSCTL_RCGCGPIO_R |= 0x0002; // activate port B
  UART1_CTL_R &= ~0x0001;      // disable UART
  UART1_IBRD_R = 104; 
// IBRD=int(50000000/(16*115,200)) = int(27.1267)
  UART1_FBRD_R = 11;  
// FBRD = round(0.1267 * 64) = 8
  UART1_LCRH_R = 0x0070;  // 8-bit length, enable FIFO
  UART1_CTL_R = 0x0301;   // enable RXE, TXE and UART
  GPIO_PORTB_AFSEL_R |= 0x03; // alt funct on PB1-0
  GPIO_PORTB_PCTL_R = 
            (GPIO_PORTB_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTB_DEN_R |= 0x03;   // digital I/O on PB1-0
  GPIO_PORTB_AMSEL_R &= ~0x03; // No analog on PB1-0
}

uint8_t UART_InChar(void) {
  while((UART1_FR_R&0x0010) != 0); 
  // wait until RXFE is 0
  return((uint8_t)(UART1_DR_R&0xFF));
}
