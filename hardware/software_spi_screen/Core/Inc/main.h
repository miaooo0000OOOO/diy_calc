/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CTP_RST_Pin GPIO_PIN_13
#define CTP_RST_GPIO_Port GPIOC
#define CTP_SCL_Pin GPIO_PIN_0
#define CTP_SCL_GPIO_Port GPIOC
#define CTP_SDA_Pin GPIO_PIN_3
#define CTP_SDA_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_10
#define LCD_RS_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOB
#define SCK_Pin GPIO_PIN_13
#define SCK_GPIO_Port GPIOB
#define SDO_MISO_Pin GPIO_PIN_14
#define SDO_MISO_GPIO_Port GPIOB
#define SDI_MOSI_Pin GPIO_PIN_15
#define SDI_MOSI_GPIO_Port GPIOB
#define CTP_INT_Pin GPIO_PIN_10
#define CTP_INT_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_9
#define LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
