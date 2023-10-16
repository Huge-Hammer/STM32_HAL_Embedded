/*
 * @Description: 
 * @Version: 1.0
 * @Author: Hammer_Zhuge
 * @Date: 2023-09-27 16:26:39
 * @LastEditors: Hammer_Zhuge
 * @LastEditTime: 2023-10-12 10:28:07
 */
#ifndef __SCCB_H
#define __SCCB_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define CAM_ADDR  			0X60  			    //OV2640 device address

typedef enum{
    NACK = 0,
    ACK  = 1
}ACK_STATUS;

#define IIC_DELAY_TIME 	    10

#define IIC_SCL_PIN 		SCCB_SCL_Pin
#define IIC_SCL_PORT 		SCCB_SCL_GPIO_Port

#define IIC_SDA_PIN 		SCCB_SDA_Pin
#define IIC_SDA_PORT 		SCCB_SDA_GPIO_Port

#define IIC_SCL_H()         HAL_GPIO_WritePin(SCCB_SCL_GPIO_Port, SCCB_SCL_Pin, GPIO_PIN_SET)
#define IIC_SCL_L()         HAL_GPIO_WritePin(SCCB_SCL_GPIO_Port, SCCB_SCL_Pin, GPIO_PIN_RESET)
#define IIC_SDA_H()         HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_SET)
#define IIC_SDA_L()         HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_RESET)

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_ACK(void);
void SCCB_NACK(void);

uint8_t SCCB_Wait_ACK(void);
uint8_t SCCB_Write_Byte(uint8_t Byte);
uint8_t SCCB_Recv_Byte(ACK_STATUS ack_sta);

uint8_t OV2640_WriteReg(uint8_t Addr, uint8_t Data);
uint8_t OV2640_ReadReg(uint8_t Addr);
#endif














