/*
 * Keypad.c
 *
 *  Created on: Aug 24, 2025
 *      Author: barte
 */

#include "Keypad.h"
#include <stdlib.h>
#include <stm32f1xx_it.h>

#define DEBOUNCE_DELAY 2



void Keypad_init(Keypad* keypad, pin_t* R_pins, pin_t* C_pins, uint8_t rows, uint8_t columns, char* keymap)
{

	keypad->rows = rows;
	keypad->columns = columns;
	keypad-> key_map = malloc(rows*columns*sizeof(char));

	for(int i = 0; i < rows * columns; i++) {
	    keypad->key_map[i] = keymap[i];
	}

	keypad->R = R_pins;
	keypad->C = C_pins;

	keypad->read = malloc(rows*columns*sizeof(bool));
	keypad->stable = malloc(rows*columns*sizeof(bool));
	keypad->last_read = malloc(rows*columns*sizeof(bool));
	keypad->pressed = malloc(rows*columns*sizeof(bool));
	keypad->last_debounce = malloc(rows*columns*sizeof(uint32_t));

	for(uint8_t r = 0; r < rows; r++)
	{
		for(uint8_t c = 0; c < columns; c++)
		{
			keypad->stable[r*columns + c] = false;
			keypad->read[r*columns + c] = false;
			keypad->last_read[r*columns + c] = false;
			keypad->last_debounce[r*columns + c] = 0;
		}
	}
}

void Keypad_scan(Keypad* keypad)
{
	for(uint8_t r = 0; r < keypad->rows; r++ )
	{

		HAL_GPIO_WritePin(keypad->R[r].GPIO_PORT, keypad->R[r].GPIO_PIN, GPIO_PIN_RESET);


		for(uint8_t c = 0; c<keypad->columns; c++)
		{
            GPIO_PinState state = HAL_GPIO_ReadPin(keypad->C[c].GPIO_PORT, keypad->C[c].GPIO_PIN);


            keypad->read[r*keypad->columns + c] = (state == GPIO_PIN_RESET);
		}
		HAL_GPIO_WritePin(keypad->R[r].GPIO_PORT, keypad->R[r].GPIO_PIN, GPIO_PIN_SET);

	}
}

void Keypad_get_pressed(Keypad* keypad)
{
	Keypad_scan(keypad);


	for(uint8_t r = 0; r < keypad->rows; r++)
	{
		for(uint8_t c = 0; c < keypad->columns; c++){

			uint8_t index = r*keypad->columns + c;
			if(keypad->last_read[index] != keypad->read[index])
				keypad->last_debounce[index] = SysTicks;

			if((SysTicks - keypad->last_debounce[index]) > DEBOUNCE_DELAY)
			{
				if(!keypad->stable[index] && keypad->read[index])
					keypad->pressed[index] = true;


				keypad->stable[index] = keypad->read[index];
			}
			keypad->last_read[index] = keypad->read[index];

		}

	}

}


char Keypad_return_val(Keypad* keypad)
{
	char c ='\0';
	for(int r = 0; r < keypad->rows * keypad->columns; r++)
		{


				if(keypad->pressed[r])
				{
					keypad->pressed[r] = false;
					c = keypad->key_map[r];

				}
		}
	return c;
}
