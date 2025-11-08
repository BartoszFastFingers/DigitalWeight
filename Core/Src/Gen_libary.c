/*
 * Gen_libary.c
 *
 *  Created on: Aug 9, 2025
 *      Author: barte
 */
#include "gpio.h"
#include "Gen_libary.h"

TIM_HandleTypeDef* General_lib_timer;

void Gen_libary_Timer_init(TIM_HandleTypeDef* htim)
{
	General_lib_timer = htim;
	HAL_TIM_Base_Start(htim);
}

void delay_us(uint32_t us)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(General_lib_timer);
    while ((__HAL_TIM_GET_COUNTER(General_lib_timer) - start)*10 < us) {
    }
}


void CHANGE_IO_GPIO_PUSH_PULL(pin_t gpio, uint32_t GPIO_INIT_MODE)
{
    HAL_GPIO_DeInit(gpio.GPIO_PORT, gpio.GPIO_PIN);
    GPIO_InitTypeDef new_gp = {0};
    new_gp.Mode = GPIO_INIT_MODE;
    new_gp.Pin = gpio.GPIO_PIN;
    new_gp.Pull = GPIO_PULLDOWN;
    new_gp.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio.GPIO_PORT, &new_gp);
}

