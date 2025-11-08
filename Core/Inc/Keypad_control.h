/**
 * @file    Keypad_control.h
 * @brief   Keypad input handling for numeric entry on HD44780 LCD.
 *
 * Provides functions to control numeric input using a matrix keypad.
 * Supports optional minus sign, digit limit, and callback function
 * on "enter" key press.
 *
 * @author  Bartosz Terlecki
 * @date    2025-08-28
 */

#ifndef INC_KEYPAD_CONTROL_H_
#define INC_KEYPAD_CONTROL_H_

#include "HD44780.h"
#include "HD44780_control.h"

/**
 * @brief Handles digit input from keypad and updates Keypad_control buffer.
 *
 * Reads a key from the keypad, handles special characters '*' (backspace)
 * and '#' (enter), supports optional minus sign, digit limit, and calls
 * a callback function on enter key press.
 *
 * @param HD Pointer to HD44780 LCD structure.
 * @param Kc Pointer to Keypad_control structure storing input buffer.
 * @param minus_plus Character flag: '-' if negative numbers allowed, otherwise any other char.
 * @param digits Maximum number of digits allowed.
 * @param Keypad_enter_fun Callback function to execute when '#' is pressed.
 */
void Keypad_digit_control(HD44780* HD, Keypad_control* Kc, char minus_plus, int digits, void (*Keypad_enter_fun)(HD44780* HD));

#endif /* INC_KEYPAD_CONTROL_H_ */
