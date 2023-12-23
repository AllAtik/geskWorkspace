/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, _EMPTY_PINC13_Pin|_EMPTY_PINC14_Pin|_EMPTY_PINC15_Pin|NTC_ACTIVE_Pin
                          |_EMPTY_PINC2_Pin|_EMPTY_PINC3_Pin|TOP_COVER_HALL_SWITCH_POWER_Pin|_EMPTY_PINC7_Pin
                          |_EMPTY_PINC8_Pin|DC_DC_POWER_ON_OFF_Pin|_EMPTY_PINC12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, _EMPTY_PINH0_Pin|_EMPTY_PINH1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, _EMPTY_PINA0_Pin|_EMPTY_PINA1_Pin|RGB_LED_BLUE_PWM_Pin|RGB_LED_GREEN_PWM_Pin
                          |RGB_LED_RED_PWM_Pin|_EMPTY_PINA7_Pin|DISTANCE_SENSOR_ON_OFF_Pin|_EMPTY_PINA11_Pin
                          |VBAT_ADC_ON_OFF_Pin|_EMPTY_PINA15_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ACC_POWER_Pin|_EMPTY_PINB1_Pin|PWRKEY_CONTROL_Pin|GPRS_POWER_ON_OFF_Pin
                          |GSM_PROCESS_STATUS_MCU_Pin|BATTERY_COVER_HALL_SWITCH_POWER_Pin|_EMPTY_PINB5_Pin|SIM_DETECT_POWER_Pin
                          |_EMPTY_PINB8_Pin|_EMPTY_PINB9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(_EMPTY_PIND2_GPIO_Port, _EMPTY_PIND2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = _EMPTY_PINC13_Pin|_EMPTY_PINC14_Pin|_EMPTY_PINC15_Pin|NTC_ACTIVE_Pin
                          |_EMPTY_PINC2_Pin|_EMPTY_PINC3_Pin|TOP_COVER_HALL_SWITCH_POWER_Pin|_EMPTY_PINC7_Pin
                          |_EMPTY_PINC8_Pin|DC_DC_POWER_ON_OFF_Pin|_EMPTY_PINC12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PHPin PHPin */
  GPIO_InitStruct.Pin = _EMPTY_PINH0_Pin|_EMPTY_PINH1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = _EMPTY_PINA0_Pin|_EMPTY_PINA1_Pin|_EMPTY_PINA7_Pin|DISTANCE_SENSOR_ON_OFF_Pin
                          |_EMPTY_PINA11_Pin|VBAT_ADC_ON_OFF_Pin|_EMPTY_PINA15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = RGB_LED_BLUE_PWM_Pin|RGB_LED_GREEN_PWM_Pin|RGB_LED_RED_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin
                           PBPin */
  GPIO_InitStruct.Pin = ACC_POWER_Pin|_EMPTY_PINB1_Pin|PWRKEY_CONTROL_Pin|GPRS_POWER_ON_OFF_Pin
                          |GSM_PROCESS_STATUS_MCU_Pin|BATTERY_COVER_HALL_SWITCH_POWER_Pin|_EMPTY_PINB5_Pin|_EMPTY_PINB8_Pin
                          |_EMPTY_PINB9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = ACC_INT_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ACC_INT_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = TOP_COVER_HALL_SWITCH_OUT_INT_Pin|BATTERY_COVER_HALL_SWITCH_OUT_INT_Pin|SIM_DETECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = _EMPTY_PIND2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(_EMPTY_PIND2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SIM_DETECT_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SIM_DETECT_POWER_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
