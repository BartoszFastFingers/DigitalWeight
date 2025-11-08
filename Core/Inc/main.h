/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define LCD1602_D7_Pin GPIO_PIN_0
#define LCD1602_D7_GPIO_Port GPIOC
#define LCD1602_D6_Pin GPIO_PIN_1
#define LCD1602_D6_GPIO_Port GPIOC
#define LCD1602_D5_Pin GPIO_PIN_2
#define LCD1602_D5_GPIO_Port GPIOC
#define LCD1602_D4_Pin GPIO_PIN_3
#define LCD1602_D4_GPIO_Port GPIOC
#define J_UP_Pin GPIO_PIN_0
#define J_UP_GPIO_Port GPIOA
#define J_DOWN_Pin GPIO_PIN_1
#define J_DOWN_GPIO_Port GPIOA
#define J_RIGHT_Pin GPIO_PIN_2
#define J_RIGHT_GPIO_Port GPIOA
#define J_LEFT_Pin GPIO_PIN_3
#define J_LEFT_GPIO_Port GPIOA
#define J_CLICK_Pin GPIO_PIN_5
#define J_CLICK_GPIO_Port GPIOC
#define KEYPAD_C3_Pin GPIO_PIN_9
#define KEYPAD_C3_GPIO_Port GPIOD
#define KEYPAD_R3_Pin GPIO_PIN_11
#define KEYPAD_R3_GPIO_Port GPIOD
#define KEYPAD_R2_Pin GPIO_PIN_13
#define KEYPAD_R2_GPIO_Port GPIOD
#define HX711_DATA_Pin GPIO_PIN_8
#define HX711_DATA_GPIO_Port GPIOA
#define HX711_CLK_Pin GPIO_PIN_9
#define HX711_CLK_GPIO_Port GPIOA
#define TIM2_PWM_Pin GPIO_PIN_15
#define TIM2_PWM_GPIO_Port GPIOA
#define LCD1602_E_Pin GPIO_PIN_10
#define LCD1602_E_GPIO_Port GPIOC
#define LCD1602_RW_Pin GPIO_PIN_11
#define LCD1602_RW_GPIO_Port GPIOC
#define LCD1602_RS_Pin GPIO_PIN_12
#define LCD1602_RS_GPIO_Port GPIOC
#define KEYPAD_C2_Pin GPIO_PIN_1
#define KEYPAD_C2_GPIO_Port GPIOD
#define KEYPAD_R1_Pin GPIO_PIN_3
#define KEYPAD_R1_GPIO_Port GPIOD
#define KEYPAD_C1_Pin GPIO_PIN_5
#define KEYPAD_C1_GPIO_Port GPIOD
#define KEYPAD_R4_Pin GPIO_PIN_7
#define KEYPAD_R4_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
