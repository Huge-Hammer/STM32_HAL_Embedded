#ifndef _OV2640_H
#define _OV2640_H

/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal.h"
#include "rtc.h"
#include "sccb.h"
#include "usart.h"
#include "dcmi.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
/* OV2640 Registers definition when DSP bank selected (0xFF = 0x00) */
#define OV2640_DSP_R_BYPASS     0x05
#define OV2640_DSP_Qs           0x44		// Quantization Scale Factor
#define OV2640_DSP_CTRL         0x50
#define OV2640_DSP_HSIZE        0x51
#define OV2640_DSP_VSIZE        0x52
#define OV2640_DSP_XOFFL        0x53
#define OV2640_DSP_YOFFL        0x54
#define OV2640_DSP_VHYX         0x55
#define OV2640_DSP_DPRP         0x56
#define OV2640_DSP_TEST         0x57
#define OV2640_DSP_ZMOW         0x5A
#define OV2640_DSP_ZMOH         0x5B
#define OV2640_DSP_ZMHH         0x5C
#define OV2640_DSP_BPADDR       0x7C
#define OV2640_DSP_BPDATA       0x7D
#define OV2640_DSP_CTRL2        0x86
#define OV2640_DSP_CTRL3        0x87
#define OV2640_DSP_SIZEL        0x8C
#define OV2640_DSP_HSIZE2       0xC0
#define OV2640_DSP_VSIZE2       0xC1
#define OV2640_DSP_CTRL0        0xC2
#define OV2640_DSP_CTRL1        0xC3
#define OV2640_DSP_R_DVP_SP     0xD3
#define OV2640_DSP_IMAGE_MODE   0xDA
#define OV2640_DSP_RESET        0xE0
#define OV2640_DSP_MS_SP        0xF0
#define OV2640_DSP_SS_ID        0x7F
#define OV2640_DSP_SS_CTRL      0xF8
#define OV2640_DSP_MC_BIST      0xF9
#define OV2640_DSP_MC_AL        0xFA
#define OV2640_DSP_MC_AH        0xFB
#define OV2640_DSP_MC_D         0xFC
#define OV2640_DSP_P_STATUS     0xFE
#define OV2640_DSP_RA_DLMT      0xFF

/* OV2640 Registers definition when sensor bank selected (0xFF = 0x01) */
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09
#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B
#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A
#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D
#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62
/* Image Sizes enumeration */
/*USER CODE END Private defines */

/* USER CODE BEGIN PV */
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL[5][8] =
{
	{
		0xFF,0x01,
		0x24,0x20,
		0x25,0x18,
		0x26,0x60,
	},
	{
		0xFF,0x01,
		0x24,0x34,
		0x25,0x1c,
		0x26,0x00,
	},
	{
		0xFF,0x01,	
		0x24,0x3e,	
		0x25,0x38,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x48,
		0x25,0x40,
		0x26,0x81,
	},
	{
		0xFF,0x01,	
		0x24,0x58,	
		0x25,0x50,	
		0x26,0x92,	
	}
};

/* USER CODE END PV */

/* USER CODE BEGIN PTD */

typedef enum
{	
 	BMP_QQVGA             =   0x00,	    /* BMP Image QQVGA 160x120 Size */
 	BMP_QVGA              =   0x01,     /* BMP Image QVGA 320x240 Size */
 	JPEG_160x120          =   0x02,	    /* JPEG Image 160x120 Size */
 	JPEG_176x144          =   0x03,	    /* JPEG Image 176x144 Size */
 	JPEG_320x240          =   0x04,	    /* JPEG Image 320x240 Size */
  	JPEG_352x288          =   0x05,	    /* JPEG Image 352x288 Size */
	JPEG_800x600		  =   0x06		/* JPEG Image 800x600 Size */
}ImageFormat_TypeDef;

typedef struct
{
	uint8_t Manufacturer_ID1;
	uint8_t Manufacturer_ID2;
	uint8_t PIDH;
	uint8_t PIDL;
}OV2640_IDTypeDef;

typedef enum
{
	MIRROR_NONE        = 0x00,
	MIRROR_VERTICAL    = 0x01,
	MIRROR_HORIZONTAL  = 0x02,
	MIRROR_BOTH        = 0x03
}MirrorMode_TypeDef;

/* USER CODE BEGIN PTD */

/* USER CODE BEGIN Prototypes */
uint8_t OV2640_WriteReg(uint8_t Addr, uint8_t Data);
uint8_t OV2640_ReadReg(uint8_t Addr);

void OV2640_SoftReset(void);
void OV2640_ReadID(OV2640_IDTypeDef *OV2640ID);

uint8_t OV2640_Init(void);
void OV2640_JPEGConfig(ImageFormat_TypeDef ImageFormat);
void OV2640_MirrorTransform(MirrorMode_TypeDef Mirror_Mode);
void OV2640_BrightnessConfig(uint8_t Brightness);
void OV2640_AutoExposure(uint8_t level);
void OV2640_LightMode(uint8_t Light_Mode);

void Handle_Data(uint32_t buf[]);
void StartOV2640(void);
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi);
/* USER CODE END Prototypes */

#endif
