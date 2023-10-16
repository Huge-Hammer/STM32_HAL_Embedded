/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : ov2640.c
  * @brief          : ov2640 driver file
  * @version		: 1.0.0
  * @date			: 2023-10-01
  * @author			: hammer
  ******************************************************************************
  */
/* USER CODE END Header */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OV2640.h"
#include "OV2640_config.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BufSize 			1024*10
/* USER CODE END PD */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
OV2640_IDTypeDef 		OV2640ID;
/* USER CODE END PTD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PV */
volatile uint8_t frame_count = 0;
volatile uint8_t frame_no = 0;
static uint32_t JpegBuf1[BufSize];
static uint32_t JpegBuf2[BufSize];
/* USER CODE END PV */

void OV2640_SoftReset(void)
{
	OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
	OV2640_WriteReg(OV2640_SENSOR_COM7, 0x80);
	HAL_Delay(50);
}

void OV2640_ReadID(OV2640_IDTypeDef *OV2640ID)
{
	OV2640ID->Manufacturer_ID1 = OV2640_ReadReg(OV2640_SENSOR_MIDH);
	OV2640ID->Manufacturer_ID2 = OV2640_ReadReg(OV2640_SENSOR_MIDL);
	OV2640ID->PIDH = OV2640_ReadReg(OV2640_SENSOR_PIDH);
	OV2640ID->PIDL = OV2640_ReadReg(OV2640_SENSOR_PIDL);
}

void OV2640_JPEGConfig(ImageFormat_TypeDef ImageFormat)
{
	uint32_t i;

	for(i=0; i<(sizeof(OV2640_SVGA_INIT)/2); i++)
	{
		OV2640_WriteReg(OV2640_SVGA_INIT[i][0], OV2640_SVGA_INIT[i][1]);
	}


	for(i=0; i<(sizeof(OV2640_YUV422)/2); i++)
	{
		OV2640_WriteReg(OV2640_YUV422[i][0], OV2640_YUV422[i][1]);
	}

	OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
	OV2640_WriteReg(OV2640_SENSOR_COM10, 0x00);

	for(i=0; i<(sizeof(OV2640_JPEG)/2); i++)
	{
		OV2640_WriteReg(OV2640_JPEG[i][0], OV2640_JPEG[i][1]);
	}

	HAL_Delay(100);

	// can add other format
	switch(ImageFormat)
	{
		case JPEG_800x600:
		{
			for(i=0; i<(sizeof(OV2640_800x600_JPEG)/2); i++)
			{
				OV2640_WriteReg(OV2640_800x600_JPEG[i][0], OV2640_800x600_JPEG[i][1]);
			}
			break;
		}
		default:
		{
			for(i=0; i<(sizeof(OV2640_800x600_JPEG)/2); i++)
			{
				OV2640_WriteReg(OV2640_800x600_JPEG[i][0], OV2640_800x600_JPEG[i][1]);
			}
			break;
		}
	}
}

void OV2640_MirrorTransform(MirrorMode_TypeDef Mirror_Mode)
{
	switch(Mirror_Mode)
	{
		case MIRROR_NONE:
		{
			OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
			OV2640_WriteReg(OV2640_SENSOR_REG04, 0x28);
			break;
		}
		case MIRROR_VERTICAL:
		{
			OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
			OV2640_WriteReg(OV2640_SENSOR_REG04, 0x58);
			break;
		}
		case MIRROR_HORIZONTAL:
		{
			OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
			OV2640_WriteReg(OV2640_SENSOR_REG04, 0x98);
			break;
		}
		case MIRROR_BOTH:
		{
			OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
			OV2640_WriteReg(OV2640_SENSOR_REG04, 0xd8);
			break;
		}
		default:
		{
			OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
			OV2640_WriteReg(OV2640_SENSOR_REG04, 0x28);
			break;
		}
	}
}

void OV2640_LightMode(uint8_t Light_Mode)
{
	uint8_t reg1_val;
	uint8_t reg2_val;
	uint8_t reg3_val;

	switch (Light_Mode)
	{
		case 0:	// auto
		{
			OV2640_WriteReg(0xff, 0x00);
			OV2640_WriteReg(0xc7, 0x10); // AWB ON
			return;
		}
		case 1:	// sunny
		{
			reg1_val = 0x5e;
			reg2_val = 0x41;
			reg3_val = 0x54;
			break;
		}
		case 2:	// cloudy
		{
			reg1_val = 0x65;
			reg2_val = 0x41;
			reg3_val = 0x4f;
			break;
		}
		case 3:	// office
		{
			reg1_val = 0x52;
			reg2_val = 0x41;
			reg3_val = 0x66;
			break;
		}
		case 4:	// home
		{
			reg1_val = 0x42;
			reg2_val = 0x3f;
			reg3_val = 0x71;
			break;
		}
	}

	OV2640_WriteReg(0xff, 0x00);
	OV2640_WriteReg(0xc7, 0x40); // AWB OFF
	OV2640_WriteReg(0xcc, reg1_val);
	OV2640_WriteReg(0xcd, reg2_val);
	OV2640_WriteReg(0xce, reg3_val);
}

void OV2640_BrightnessConfig(uint8_t Brightness)
{
	OV2640_WriteReg(0xff, 0x00);
	OV2640_WriteReg(0x7c, 0x00);
	OV2640_WriteReg(0x7d, 0x04);
	OV2640_WriteReg(0x7c, 0x09);
	OV2640_WriteReg(0x7d, Brightness);
	OV2640_WriteReg(0x7d, 0x00);
}

void OV2640_AutoExposure(uint8_t level)
{
	uint8_t i;
	uint8_t *p = (uint8_t *)OV2640_AUTOEXPOSURE_LEVEL[level];
	for(i=0; i<4; i++)
	{
		OV2640_WriteReg(p[i*2], p[i*2+1]);
	}
}

uint8_t OV2640_Init()
{
	OV2640_ReadID(&OV2640ID);
	if(OV2640ID.PIDH != 0x26 && OV2640ID.PIDL != 0x42)
	{
		printf("OV2640_ReadID error, 0x%x%x!\r\n", OV2640ID.PIDH, OV2640ID.PIDL);
		return 1;
	}
	else
		printf("OV2640 ID: 0x%x%x\r\n", OV2640ID.PIDH, OV2640ID.PIDL);

	OV2640_JPEGConfig(JPEG_800x600);
	OV2640_MirrorTransform(MIRROR_VERTICAL);
	OV2640_LightMode(0);	// AWB
	OV2640_BrightnessConfig(0x20);
	OV2640_AutoExposure(0);

	return 0;
}

void Handle_Data(uint32_t buf[])
{
	uint16_t i, sof, jpgLen=0;
	uint8_t *pBuf = (uint8_t *)buf;
	uint8_t head_flag = 0;

	for(i = 0 ; i < 65535; i++)
	{
		if((pBuf[i] == 0xFF) && (pBuf[i+1] == 0xD8) && (!head_flag))
		{
			sof = i;
			head_flag = 1;	// find the head of jpg
		}
		if((pBuf[i] == 0xFF) && (pBuf[i+1] == 0xD9) && head_flag)	// find the end of jpg when head_flag is 1
		{
			jpgLen = i - sof + 2;
			break;
		}
	}

	pBuf += sof;	// point to the head of jpg

	printf("jpgLen: %d\r\n", jpgLen);

	HAL_UART_Transmit(&huart3, pBuf, jpgLen, 0xFFFF);	// send jpg to PC
}

void StartOV2640(void)
{
	__HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);//使用帧中断
	memset((void *)JpegBuf1, 0, sizeof(JpegBuf1));
	memset((void *)JpegBuf2, 0, sizeof(JpegBuf2));
	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)JpegBuf1, BufSize);//启动拍照
}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	frame_count++;
	HAL_DCMI_Stop(hdcmi);//拍照完成，停止DMA传输

	if(frame_no == 0)
	{
		frame_no = 1;
		printf("current frame %d\r\n", frame_count);

		HAL_DCMI_Start_DMA(hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)JpegBuf2, BufSize);//启动拍照
	}
	else
	{
		frame_no = 0;
		printf("current frame %d\r\n", frame_count);

		if(frame_count >= 2) {
			frame_count = 0;
			HAL_DCMI_Stop(hdcmi);//拍照完成，停止DMA传输
			HAL_DCMI_Suspend(hdcmi);//拍照完成，挂起DCMI

			Handle_Data(JpegBuf1);
			Handle_Data(JpegBuf2);
		}
	}
}
