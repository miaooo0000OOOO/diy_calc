//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
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
#include "ctpiic.h"
#include "delay.h"	 

/*****************************************************************************
 * @name       :void CTP_Delay(void)
 * @date       :2020-05-13 
 * @function   :Delay in controlling IIC speed
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_Delay(void)
{
	delay_us(1);
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Init(void)
 * @date       :2020-05-13 
 * @function   :Initialize IIC
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Init(void)
{	
	// 已用CubeMX定义
	// GPIO_InitTypeDef GPIO_InitStructure;
	// RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOC, &GPIO_InitStructure);

	// CTP_IIC_SCL=1;
	// CTP_IIC_SDA=1;
}

/*****************************************************************************
 * @name       :void CTP_IIC_Start(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void CTP_IIC_Start(void)
{
	CTP_SDA_OUT();     //sda线输出
	CTP_IIC_SDA=1;	  	  
	CTP_IIC_SCL=1;
	CTP_Delay();
 	CTP_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CTP_Delay();
	CTP_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Stop(void)
 * @date       :2020-05-13 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/   
void CTP_IIC_Stop(void)
{ 
	CTP_SDA_OUT();//sda线输出
	CTP_IIC_SCL=0;
	CTP_IIC_SDA=0;
	CTP_Delay();
	CTP_IIC_SCL=1;
	CTP_Delay();
	CTP_IIC_SDA=1;//STOP:when CLK is high DATA change form low to high 
}

/*****************************************************************************
 * @name       :u8 CTP_IIC_Wait_Ack(void)
 * @date       :2020-05-13 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
u8 CTP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CTP_SDA_IN();      //SDA设置为输入  
	CTP_IIC_SDA=1;delay_us(1);	   
	CTP_IIC_SCL=1;delay_us(1);	 
	while(CTP_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CTP_IIC_Stop();
			return 1;
		} 
	}
	CTP_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

/*****************************************************************************
 * @name       :void CTP_IIC_Ack(void)
 * @date       :2020-05-13 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void CTP_IIC_Ack(void)
{
	CTP_IIC_SCL=0;
	CTP_SDA_OUT();
	CTP_IIC_SDA=0;
	CTP_Delay();
	CTP_IIC_SCL=1;
	CTP_Delay();
	CTP_IIC_SCL=0;
}

/*****************************************************************************
 * @name       :void CTP_IIC_NAck(void)
 * @date       :2020-05-13 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	    
void CTP_IIC_NAck(void)
{
	CTP_IIC_SCL=0;
	CTP_SDA_OUT();
	CTP_IIC_SDA=1;
	CTP_Delay();
	CTP_IIC_SCL=1;
	CTP_Delay();
	CTP_IIC_SCL=0;
}	

/*****************************************************************************
 * @name       :void CTP_IIC_Send_Byte(u8 txd)
 * @date       :2020-05-13 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/					 				     		  
void CTP_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	CTP_SDA_OUT(); 	    
  CTP_IIC_SCL=0;//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    CTP_IIC_SDA=(txd&0x80)>>7;
    txd<<=1; 	      
		CTP_IIC_SCL=1;
		CTP_Delay();
		CTP_IIC_SCL=0;	
		CTP_Delay();
  }	 
} 	

/*****************************************************************************
 * @name       :u8 CTP_IIC_Read_Byte(unsigned char ack)
 * @date       :2020-05-13 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/	    
u8 CTP_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CTP_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        CTP_IIC_SCL=0; 	    	   
		delay_us(3);
		CTP_IIC_SCL=1;  
		receive<<=1;
		if(CTP_READ_SDA)receive++;   
	}	  				 
	if (!ack)CTP_IIC_NAck();//发送nACK
	else CTP_IIC_Ack(); //发送ACK   
 	return receive;
}




























