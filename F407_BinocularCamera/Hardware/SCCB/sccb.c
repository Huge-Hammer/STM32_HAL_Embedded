#include "sccb.h"

void IIC_Delay(uint8_t time)
{
    uint32_t i = time * 100; 

    while (i--)
    {
        ;
    }
}

void SCCB_Init(void)
{
    IIC_SDA_H();
    IIC_SCL_H();
}

static void SCCB_Output(void)
{
    GPIO_InitTypeDef SCCB_GPIO;
    SCCB_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
    SCCB_GPIO.Pin = IIC_SDA_PIN;
    SCCB_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(IIC_SDA_PORT, &SCCB_GPIO);
}

static void SCCB_Input(void)
{
    GPIO_InitTypeDef SCCB_GPIO;
    SCCB_GPIO.Mode = GPIO_MODE_INPUT;
    SCCB_GPIO.Pin = IIC_SDA_PIN;
    SCCB_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(IIC_SDA_PORT, &SCCB_GPIO);
}

void SCCB_Start(void)
{
    SCCB_Output();
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

void SCCB_Stop(void)
{
    SCCB_Output();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
}

void SCCB_ACK(void)
{
    SCCB_Output();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

void SCCB_NACK(void)
{
    SCCB_Output();
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
}

uint8_t SCCB_Wait_ACK(void)
{
    uint8_t wait;
    SCCB_Output();
    IIC_SDA_H();
    SCCB_Input();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    while (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN))
    {
        wait++;
        if (wait > 200)
        {
            SCCB_Stop();
            return 0;
        }
    }
    IIC_SCL_L();
    return 1;
}

void SCCB_Write_High(void)
{
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

void SCCB_Write_Low(void)
{
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

uint8_t SCCB_Write_Byte(uint8_t Byte)
{
    uint8_t i;
    SCCB_Output();
    for (i = 0x80; i != 0; i >>= 1)
    {
        if (Byte & i)
        {
            SCCB_Write_High();
        }
        else
        {
            SCCB_Write_Low();
        }
    }
    return (SCCB_Wait_ACK());
}

uint8_t SCCB_Recv_Byte(ACK_STATUS ack_sta)
{
    uint8_t data = 0, i;
    SCCB_Input();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    for (i = 0x80; i != 0; i >>= 1)
    {
        if (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN) == 1)
        {
            data |= i;
        }
        IIC_Delay(IIC_DELAY_TIME);
        IIC_SCL_L();
        IIC_Delay(IIC_DELAY_TIME);
        IIC_SCL_H();
        IIC_Delay(IIC_DELAY_TIME);
    }
    if (ack_sta == ACK)
    {
        SCCB_ACK();
    }
    else
    {
        SCCB_NACK();
    }
    return data;
}


uint8_t OV2640_WriteReg(uint8_t Addr, uint8_t Data)
{
    uint8_t res = 0;

    SCCB_Start();
    if (SCCB_Write_Byte(CAM_ADDR)) res = 1;
    HAL_Delay(100);
    if (SCCB_Write_Byte(Addr)) res = 1;
    HAL_Delay(100);
    if (SCCB_Write_Byte(Data)) res = 1;
    SCCB_Stop();

    return res;
}

uint8_t OV2640_ReadReg(uint8_t Addr)
{
    uint8_t val = 0;

    SCCB_Start();
    SCCB_Write_Byte(CAM_ADDR);
    HAL_Delay(100);
    SCCB_Write_Byte(Addr);
    HAL_Delay(100);
    SCCB_Stop();
    HAL_Delay(100);

    SCCB_Start();
    SCCB_Write_Byte(CAM_ADDR | 0x01);
    HAL_Delay(100);
    val = SCCB_Recv_Byte(NACK);
    SCCB_Stop();

    return val;
}
