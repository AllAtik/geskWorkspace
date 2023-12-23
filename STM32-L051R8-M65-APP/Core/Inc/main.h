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
#include "stm32l0xx_hal.h"

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
#define _EMPTY_PINC13_Pin GPIO_PIN_13
#define _EMPTY_PINC13_GPIO_Port GPIOC
#define _EMPTY_PINC14_Pin GPIO_PIN_14
#define _EMPTY_PINC14_GPIO_Port GPIOC
#define _EMPTY_PINC15_Pin GPIO_PIN_15
#define _EMPTY_PINC15_GPIO_Port GPIOC
#define _EMPTY_PINH0_Pin GPIO_PIN_0
#define _EMPTY_PINH0_GPIO_Port GPIOH
#define _EMPTY_PINH1_Pin GPIO_PIN_1
#define _EMPTY_PINH1_GPIO_Port GPIOH
#define NTC_ACTIVE_Pin GPIO_PIN_0
#define NTC_ACTIVE_GPIO_Port GPIOC
#define TEMP_ADC_Pin GPIO_PIN_1
#define TEMP_ADC_GPIO_Port GPIOC
#define _EMPTY_PINC2_Pin GPIO_PIN_2
#define _EMPTY_PINC2_GPIO_Port GPIOC
#define _EMPTY_PINC3_Pin GPIO_PIN_3
#define _EMPTY_PINC3_GPIO_Port GPIOC
#define _EMPTY_PINA0_Pin GPIO_PIN_0
#define _EMPTY_PINA0_GPIO_Port GPIOA
#define _EMPTY_PINA1_Pin GPIO_PIN_1
#define _EMPTY_PINA1_GPIO_Port GPIOA
#define GSM_RX_Pin GPIO_PIN_2
#define GSM_RX_GPIO_Port GPIOA
#define GSM_TX_Pin GPIO_PIN_3
#define GSM_TX_GPIO_Port GPIOA
#define RGB_LED_BLUE_PWM_Pin GPIO_PIN_4
#define RGB_LED_BLUE_PWM_GPIO_Port GPIOA
#define RGB_LED_GREEN_PWM_Pin GPIO_PIN_5
#define RGB_LED_GREEN_PWM_GPIO_Port GPIOA
#define RGB_LED_RED_PWM_Pin GPIO_PIN_6
#define RGB_LED_RED_PWM_GPIO_Port GPIOA
#define _EMPTY_PINA7_Pin GPIO_PIN_7
#define _EMPTY_PINA7_GPIO_Port GPIOA
#define VBAT_ADC_HIGH_Pin GPIO_PIN_4
#define VBAT_ADC_HIGH_GPIO_Port GPIOC
#define VBAT_ADC_LOW_Pin GPIO_PIN_5
#define VBAT_ADC_LOW_GPIO_Port GPIOC
#define ACC_POWER_Pin GPIO_PIN_0
#define ACC_POWER_GPIO_Port GPIOB
#define _EMPTY_PINB1_Pin GPIO_PIN_1
#define _EMPTY_PINB1_GPIO_Port GPIOB
#define ACC_INT_1_Pin GPIO_PIN_2
#define ACC_INT_1_GPIO_Port GPIOB
#define ACC_INT_1_EXTI_IRQn EXTI2_3_IRQn
#define ACC_SCL_Pin GPIO_PIN_10
#define ACC_SCL_GPIO_Port GPIOB
#define ACC_SDA_Pin GPIO_PIN_11
#define ACC_SDA_GPIO_Port GPIOB
#define PWRKEY_CONTROL_Pin GPIO_PIN_12
#define PWRKEY_CONTROL_GPIO_Port GPIOB
#define GPRS_POWER_ON_OFF_Pin GPIO_PIN_13
#define GPRS_POWER_ON_OFF_GPIO_Port GPIOB
#define GSM_PROCESS_STATUS_MCU_Pin GPIO_PIN_14
#define GSM_PROCESS_STATUS_MCU_GPIO_Port GPIOB
#define TOP_COVER_HALL_SWITCH_OUT_INT_Pin GPIO_PIN_15
#define TOP_COVER_HALL_SWITCH_OUT_INT_GPIO_Port GPIOB
#define TOP_COVER_HALL_SWITCH_POWER_Pin GPIO_PIN_6
#define TOP_COVER_HALL_SWITCH_POWER_GPIO_Port GPIOC
#define _EMPTY_PINC7_Pin GPIO_PIN_7
#define _EMPTY_PINC7_GPIO_Port GPIOC
#define _EMPTY_PINC8_Pin GPIO_PIN_8
#define _EMPTY_PINC8_GPIO_Port GPIOC
#define DC_DC_POWER_ON_OFF_Pin GPIO_PIN_9
#define DC_DC_POWER_ON_OFF_GPIO_Port GPIOC
#define DISTANCE_SENSOR_ON_OFF_Pin GPIO_PIN_8
#define DISTANCE_SENSOR_ON_OFF_GPIO_Port GPIOA
#define TX_SENSOR_Pin GPIO_PIN_9
#define TX_SENSOR_GPIO_Port GPIOA
#define RX_SENSOR_Pin GPIO_PIN_10
#define RX_SENSOR_GPIO_Port GPIOA
#define _EMPTY_PINA11_Pin GPIO_PIN_11
#define _EMPTY_PINA11_GPIO_Port GPIOA
#define VBAT_ADC_ON_OFF_Pin GPIO_PIN_12
#define VBAT_ADC_ON_OFF_GPIO_Port GPIOA
#define _EMPTY_PINA15_Pin GPIO_PIN_15
#define _EMPTY_PINA15_GPIO_Port GPIOA
#define _EMPTY_PINC12_Pin GPIO_PIN_12
#define _EMPTY_PINC12_GPIO_Port GPIOC
#define _EMPTY_PIND2_Pin GPIO_PIN_2
#define _EMPTY_PIND2_GPIO_Port GPIOD
#define BATTERY_COVER_HALL_SWITCH_OUT_INT_Pin GPIO_PIN_3
#define BATTERY_COVER_HALL_SWITCH_OUT_INT_GPIO_Port GPIOB
#define BATTERY_COVER_HALL_SWITCH_POWER_Pin GPIO_PIN_4
#define BATTERY_COVER_HALL_SWITCH_POWER_GPIO_Port GPIOB
#define _EMPTY_PINB5_Pin GPIO_PIN_5
#define _EMPTY_PINB5_GPIO_Port GPIOB
#define SIM_DETECT_POWER_Pin GPIO_PIN_6
#define SIM_DETECT_POWER_GPIO_Port GPIOB
#define SIM_DETECT_Pin GPIO_PIN_7
#define SIM_DETECT_GPIO_Port GPIOB
#define _EMPTY_PINB8_Pin GPIO_PIN_8
#define _EMPTY_PINB8_GPIO_Port GPIOB
#define _EMPTY_PINB9_Pin GPIO_PIN_9
#define _EMPTY_PINB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void SystemClock_Config(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
