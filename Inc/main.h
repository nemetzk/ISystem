/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define Y10_Pin GPIO_PIN_1
#define Y10_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define Y11_Pin GPIO_PIN_3
#define Y11_GPIO_Port GPIOA
#define Y12_Pin GPIO_PIN_4
#define Y12_GPIO_Port GPIOA
#define Y13_Pin GPIO_PIN_5
#define Y13_GPIO_Port GPIOA
#define Y14_Pin GPIO_PIN_6
#define Y14_GPIO_Port GPIOA
#define Y15_Pin GPIO_PIN_7
#define Y15_GPIO_Port GPIOA
#define X01_Pin GPIO_PIN_0
#define X01_GPIO_Port GPIOB
#define Y00_Pin GPIO_PIN_8
#define Y00_GPIO_Port GPIOA
#define Y01_Pin GPIO_PIN_11
#define Y01_GPIO_Port GPIOA
#define X00_Pin GPIO_PIN_12
#define X00_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB
#define Y03_Pin GPIO_PIN_4
#define Y03_GPIO_Port GPIOB
#define Y02_Pin GPIO_PIN_5
#define Y02_GPIO_Port GPIOB
#define X03_Pin GPIO_PIN_6
#define X03_GPIO_Port GPIOB
#define X02_Pin GPIO_PIN_7
#define X02_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
