/*
 * Joystick_lib.c
 *
 *  Created on: Aug 19, 2025
 *      Author: barte
 */
#include "Joystick.h"
#include "stm32f1xx_it.h"

#define DEBOUNCE_DELAY 20

void JoystickInit(Joystick* J, GPIO_TypeDef* up_port, uint16_t up_pin,
		GPIO_TypeDef* right_port, uint16_t right_pin,
		GPIO_TypeDef* down_port, uint16_t down_pin,
		GPIO_TypeDef* left_port, uint16_t left_pin,
		GPIO_TypeDef* click_port, uint16_t click_pin)
{
	J->up.GPIO_PORT = up_port;
	J->up.GPIO_PIN = up_pin;
	J->left.GPIO_PORT = left_port;
	J->left.GPIO_PIN = left_pin;
	J->down.GPIO_PORT = down_port;
	J->down.GPIO_PIN = down_pin;
	J->right.GPIO_PORT = right_port;
	J->right.GPIO_PIN = right_pin;
	J->click.GPIO_PORT = click_port;
	J->click.GPIO_PIN = click_pin;
}

bool is_up_pressed(Joystick* J)
{
	if(HAL_GPIO_ReadPin(J->up.GPIO_PORT, J->up.GPIO_PIN) == GPIO_PIN_RESET) return true;
	else return false;
}
bool is_right_pressed(Joystick* J)
{
	if(HAL_GPIO_ReadPin(J->right.GPIO_PORT, J->right.GPIO_PIN) == GPIO_PIN_RESET) return true;
	else return false;
}
bool is_down_pressed(Joystick* J)
{
	if(HAL_GPIO_ReadPin(J->down.GPIO_PORT, J->down.GPIO_PIN) == GPIO_PIN_RESET) return true;
	else return false;
}
bool is_left_pressed(Joystick* J)
{
	if(HAL_GPIO_ReadPin(J->left.GPIO_PORT, J->left.GPIO_PIN) == GPIO_PIN_RESET) return true;
	else return false;
}

bool is_click_pressed(Joystick* J)
{
	if(HAL_GPIO_ReadPin(J->click.GPIO_PORT, J->click.GPIO_PIN) == GPIO_PIN_RESET) return true;
	else return false;
}

void pool_joystick(Joystick* J)
{
	bool reading[5];
	reading[0] = is_up_pressed(J);
	reading[1] = is_right_pressed(J);
	reading[2] = is_down_pressed(J);
	reading[3] = is_left_pressed(J);
	reading[4] = is_click_pressed(J);

	for(int i = 0; i < 5; i ++)
	{
		if(J->last_read[i] != reading[i])
			J->last_debounce[i] = SysTicks;

		if((SysTicks - J->last_debounce[i]) > DEBOUNCE_DELAY)
		{

			if(!J->stable[i] && reading[i])
				J->pressed[i] = true;

			J->stable[i] = reading[i];
		}
		J->last_read[i] = reading[i];
	}

}
