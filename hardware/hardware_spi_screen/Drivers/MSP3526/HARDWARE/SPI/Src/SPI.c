//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
// QDtech-TFT液晶驱动 for STM32 IO模拟
// xiao冯@ShenZhen QDtech co.,LTD
// 公司网站:www.qdtft.com
// 淘宝网站：http://qdtech.taobao.com
// wiki技术网站：http://www.lcdwiki.com
// 我司提供技术支持，任何技术问题欢迎随时交流学习
// 固话(传真) :+86 0755-23594567
// 手机:15989313508（冯工）
// 邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// 技术支持QQ:3002773612  3002778157
// 技术交流QQ群:324828016
// 创建日期:2018/08/09
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机
//    SDI(MOSI)      接          PB15         //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB14         //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机
//       LED         接          PB9          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//     LCD_RS        接          PB10         //液晶屏数据/命令控制信号
//     LCD_RST       接          PB12         //液晶屏复位控制信号
//     LCD_CS        接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机
//     CTP_INT       接          PC10         //电容触摸屏中断信号
//     CTP_SDA       接          PC3          //电容触摸屏IIC数据信号
//     CTP_RST       接          PC13         //电容触摸屏复位信号
//     CTP_SCL       接          PC0          //电容触摸屏IIC时钟信号
**************************************************************************************************/
/* @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 **************************************************************************************************/
#include "spi.h"
#include "retarget.h"

/*****************************************************************************
 * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 byte)
 * @date       :2018-08-09
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
				byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef *SPIx, u8 byte)
{
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET)
		;			 // 等待发送区空
	SPIx->DR = byte; // 发送一个byte
	// printf("byte: %d\n", byte);
	// printf("DR: %d SR: %d\n", SPIx->DR, SPIx->SR);
	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET)
		;			 // 等待接收完一个byte
	return SPIx->DR; // 返回收到的数据

	// u8 res;
	// HAL_StatusTypeDef s = HAL_SPI_Transmit(&hspi2, &byte, 1, 1000);
	// s = HAL_SPI_Receive(&hspi2, &res, 1, 1000);
	// return res;
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
 * @date       :2018-08-09
 * @function   :Set hardware SPI Speed
 * @parameters :SPIx: SPI type,x for 1,2,3
				SpeedSet:0-high speed
												 1-low speed
 * @retvalue   :None
******************************************************************************/
void SPI_SetSpeed(SPI_TypeDef *SPIx, u8 SpeedSet)
{
	SPIx->CR1 &= 0XFFC7;
	if (SpeedSet == 1) // 高速
	{
		SPIx->CR1 |= SPI_BaudRatePrescaler_2; // Fsck=Fpclk/2
	}
	else // 低速
	{
		SPIx->CR1 |= SPI_BaudRatePrescaler_4; // Fsck=Fpclk/32
	}
	SPIx->CR1 |= 1 << 6; // SPI设备使能
}

/**
 * @brief  Enables or disables the specified SPI peripheral.
 * @param  SPIx: where x can be 1, 2 or 3 to select the SPI peripheral.
 * @param  NewState: new state of the SPIx peripheral.
 *   This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_SPI_ALL_PERIPH(SPIx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the selected SPI peripheral */
		SPIx->CR1 |= CR1_SPE_Set;
	}
	else
	{
		/* Disable the selected SPI peripheral */
		SPIx->CR1 &= CR1_SPE_Reset;
	}
}

/*****************************************************************************
 * @name       :void SPI2_Init(void)
 * @date       :2018-08-09
 * @function   :Initialize the STM32 hardware SPI2
 * @parameters :None
 * @retvalue   :None
 ******************************************************************************/
// 使用HAL库配置
void SPI2_Init(void)
{
	// SPI_InitTypeDef  SPI_InitStructure;
	// GPIO_InitTypeDef GPIO_InitStructure;

	// //配置SPI2管脚
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);

	// //SPI2配置选项
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);

	// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// SPI_InitStructure.SPI_CRCPolynomial = 7;
	// SPI_Init(SPI2, &SPI_InitStructure);

	// //使能SPI2
	SPI_Cmd(SPI2, ENABLE);
}
