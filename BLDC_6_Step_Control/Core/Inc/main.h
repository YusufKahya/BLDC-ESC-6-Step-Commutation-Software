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
#include "stm32f3xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Phase_A_HIGH_Pin GPIO_PIN_0
#define Phase_A_HIGH_GPIO_Port GPIOC
#define Phase_B_HIGH_Pin GPIO_PIN_1
#define Phase_B_HIGH_GPIO_Port GPIOC
#define Phase_C_HIGH_Pin GPIO_PIN_2
#define Phase_C_HIGH_GPIO_Port GPIOC
#define Comp_A_OUT_Pin GPIO_PIN_0
#define Comp_A_OUT_GPIO_Port GPIOA
#define Comp_A_INP_Pin GPIO_PIN_1
#define Comp_A_INP_GPIO_Port GPIOA
#define Phase_B_LOW_Pin GPIO_PIN_4
#define Phase_B_LOW_GPIO_Port GPIOA
#define Comp_A_INM_Pin GPIO_PIN_5
#define Comp_A_INM_GPIO_Port GPIOA
#define Phase_A_LOW_Pin GPIO_PIN_6
#define Phase_A_LOW_GPIO_Port GPIOA
#define Toggle_Pin GPIO_PIN_0
#define Toggle_GPIO_Port GPIOB
#define Comp_C_INM_Pin GPIO_PIN_10
#define Comp_C_INM_GPIO_Port GPIOB
#define Comp_B_INM_Pin GPIO_PIN_12
#define Comp_B_INM_GPIO_Port GPIOB
#define Comp_C_INP_Pin GPIO_PIN_13
#define Comp_C_INP_GPIO_Port GPIOB
#define Comp_B_INP_Pin GPIO_PIN_14
#define Comp_B_INP_GPIO_Port GPIOB
#define Comp_C_OUT_Pin GPIO_PIN_7
#define Comp_C_OUT_GPIO_Port GPIOC
#define Phase_C_LOW_Pin GPIO_PIN_8
#define Phase_C_LOW_GPIO_Port GPIOC
#define Comp_B_Out_Pin GPIO_PIN_8
#define Comp_B_Out_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
void Set_Six_Step_Position(uint8_t State, uint16_t DutyCycle);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
