/**
 * @file    Joystick_Menu.h
 * @brief   Menu navigation handling using a joystick for HD44780 LCD.
 *
 * Provides function prototypes for mapping joystick inputs (up, down, left, right, click)
 * to menu navigation callbacks on the HD44780 LCD display.
 *
 * @author  Bartosz Terlecki
 * @date    2025-08-20
 */
#ifndef INC_JOYSTICK_MENU_H_
#define INC_JOYSTICK_MENU_H_

#define NULL 0
#include "Joystick.h"
#include "HD44780_menu.h"

extern void (*key_next_fun)(HD44780* HD);
extern void (*key_prev_fun)(HD44780* HD);
extern void (*key_enter_fun)(HD44780* HD);
extern void (*key_back_fun)(HD44780* HD);
extern void (*key_on_fun)(HD44780* HD);


void key_next_pressed(HD44780* HD, Joystick* J);
void key_prev_pressed(HD44780* HD, Joystick* J);
void key_enter_pressed(HD44780* HD, Joystick* J);
void key_back_pressed(HD44780* HD, Joystick* J);
void key_on_pressed(HD44780* HD, Joystick* J);


#endif /* INC_JOYSTICK_MENU_H_ */
