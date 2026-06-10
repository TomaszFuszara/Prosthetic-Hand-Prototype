/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define LY2_Pin GPIO_PIN_14
#define LY2_GPIO_Port GPIOC
#define LY1_Pin GPIO_PIN_15
#define LY1_GPIO_Port GPIOC
#define LG_Pin GPIO_PIN_0
#define LG_GPIO_Port GPIOA
#define LR_Pin GPIO_PIN_1
#define LR_GPIO_Port GPIOA
#define Palec1Roskorcz_Pin GPIO_PIN_6
#define Palec1Roskorcz_GPIO_Port GPIOA
#define Palec1Skorcz_Pin GPIO_PIN_7
#define Palec1Skorcz_GPIO_Port GPIOA
#define Palec2Skorcz_Pin GPIO_PIN_15
#define Palec2Skorcz_GPIO_Port GPIOB
#define Palec2Roskorcz_Pin GPIO_PIN_8
#define Palec2Roskorcz_GPIO_Port GPIOA
#define Palec3Skorcz_Pin GPIO_PIN_9
#define Palec3Skorcz_GPIO_Port GPIOA
#define Palec3Roskorcz_Pin GPIO_PIN_10
#define Palec3Roskorcz_GPIO_Port GPIOA
#define Palec4Skorcz_Pin GPIO_PIN_11
#define Palec4Skorcz_GPIO_Port GPIOA
#define Palec4Roskorcz_Pin GPIO_PIN_15
#define Palec4Roskorcz_GPIO_Port GPIOA
#define Palec5Skorcz_Pin GPIO_PIN_3
#define Palec5Skorcz_GPIO_Port GPIOB
#define Palec5Roskorcz_Pin GPIO_PIN_4
#define Palec5Roskorcz_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
