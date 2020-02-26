#include "I2C.h"
void I2C_Init()
{
    SYSCTL_RCGCI2C_R |= 0x08; /* enable clock to I2C3 */
    
    I2C3_MCR_R = 0x10; /* master mode */
    I2C3_MTPR_R = 7; /* 100 kHz @ 16 MHz */
}
static int I2C_wait_till_done(void)
{
    while (I2C3_MCS_R & 1)
        ; /* wait until I2C master is not busy */
    return I2C3_MCS_R & 0xE; /* return I2C error code */
}

char I2C_Write(int slaveAddr, char memAddr, char data)
{
    char error;
    I2C3_MSA_R = slaveAddr << 1;
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 3;
    error = I2C_wait_till_done();
    if (error)
        return error;
    I2C3_MDR_R = data;
    I2C3_MCS_R = 5;
    error = I2C_wait_till_done();
    while (I2C3_MCS_R & 0x40)
        ;
    error = I2C3_MCS_R & 0xE;
    if (error)
        return error;
    return 0;
}

char I2C_Read(int slaveAddr, char memAddr, int byteCount, char* data)
{
    char error;

    if (byteCount <= 0)
        return -1; /* no read was performed */

    /* send slave address and starting address */
    I2C3_MSA_R = slaveAddr << 1;
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */
    error = I2C_wait_till_done();
    if (error)
        return error;

    /* to change bus from write to read, send restart with slave addr */
    I2C3_MSA_R = (slaveAddr << 1) + 1; /* restart: -R-(saddr+r)-ACK */

    if (byteCount == 1) /* if last byte, don't ack */
        I2C3_MCS_R = 7; /* -data-NACK-P */
    else
        /* else ack */
        I2C3_MCS_R = 0xB; /* -data-ACK- */
    error = I2C_wait_till_done();
    if (error)
        return error;

    *data++ = I2C3_MDR_R; /* store the data received */

    if (--byteCount == 0) /* if single byte read, done */
    {
        while (I2C3_MCS_R & 0x40)
            ; /* wait until bus is not busy */
        return 0; /* no error */
    }

    /* read the rest of the bytes */
    while (byteCount > 1)
    {
        I2C3_MCS_R = 9; /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error)
            return error;
        byteCount--;
        *data++ = I2C3_MDR_R; /* store data received */
    }

    I2C3_MCS_R = 5; /* -data-NACK-P */
    error = I2C_wait_till_done();
    *data = I2C3_MDR_R; /* store data received */
    while (I2C3_MCS_R & 0x40)
        ; /* wait until bus is not busy */

    return 0; /* no error */
}
/*void I2C_Write(uint32_t address,uint8_t dataByte, uint8_t conditions)
{
    if(conditions == 3)
    {
      I2C1_MSA_R = address <<1;
      I2C1_MDR_R = dataByte;
      I2C1_MCS_R = 3;
    }
    else if(conditions == 5)
    {
      I2C1_MDR_R = dataByte;
      I2C1_MCS_R = 5;
    }
    else if(conditions == 1)
    {
      I2C1_MDR_R = dataByte;
      I2C1_MCS_R = 1;
    }
    while(I2C1_MCS_R & (1<<0) !=0);
}
uint8_t I2C_Read(uint32_t address, uint8_t conditions)
{
    if(conditions == 0xB)
    {
      I2C1_MSA_R = (address <<1)+1;
      I2C1_MCS_R = 0xB;
    }
    else if(conditions == 5)
    {
      I2C1_MCS_R = 5;
    }
    else if(conditions == 9)
    {
      I2C1_MCS_R = 9;
    }
    while(I2C1_MCS_R & (1<<0) !=0);
    return I2C1_MDR_R;
}*/

