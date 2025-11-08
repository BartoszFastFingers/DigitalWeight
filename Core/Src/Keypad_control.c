/*
 * Keypad_control.c
 *
 *  Created on: Aug 28, 2025
 *      Author: barte
 */
#include "Keypad_control.h"


void Keypad_digit_control(HD44780* HD, Keypad_control* Kc, char minus_plus, int digits, void (*Keypad_enter_fun)(HD44780* HD))
{
	char c = Keypad_return_val(Kc->K);

		if (c != '\0') {
				switch (c) {
				case '*':
					if (Kc->K_offset > 0) {
						Kc->K_str[Kc->K_offset] = NULL;
						Kc->K_str[--Kc->K_offset] = '\0';
					}
					else
					{
						if(minus_plus == '-')
							Kc->K_str[Kc->K_offset++] = '-';
						Kc->K_str[Kc->K_offset] = '\0';
					}
					break;
				case '#':
						Keypad_enter_fun(HD);
					break;
				default:
					int max_offset = minus_plus == '-' ? digits - 1 : digits;
					if(Kc->K_str[0] == '-') max_offset++;

					if (Kc->K_offset < max_offset) {
						Kc->K_str[Kc->K_offset++] = c;
						Kc->K_str[Kc->K_offset] = '\0';
					}
					break;
				}
			}
}
