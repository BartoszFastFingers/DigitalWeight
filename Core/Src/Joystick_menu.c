/*
 * Joystick_menu.c
 *
 *  Created on: Aug 20, 2025
 *      Author: barte
 */

#include "Joystick_menu.h"
#define NULL 0

void (*key_next_fun)(HD44780* HD) = NULL;
void (*key_prev_fun)(HD44780* HD) = NULL;
void (*key_enter_fun)(HD44780* HD) = NULL;
void (*key_back_fun)(HD44780* HD) = NULL;
void (*key_on_fun)(HD44780* HD) = NULL;


void key_next_pressed(HD44780* HD, Joystick* J)
{
	if(J->pressed[2])
	{
		J->pressed[2] = false;
		if(key_next_fun) (*key_next_fun)(HD);
	}
}

void key_prev_pressed(HD44780* HD, Joystick* J)
{
	if(J->pressed[0])
	{
		J->pressed[0] = false;
		if(key_prev_fun) (*key_prev_fun)(HD);

	}
}

void key_enter_pressed(HD44780* HD, Joystick* J)
{
	if(J->pressed[1])
	{
		J->pressed[1] = false;
		if(key_enter_fun) (*key_enter_fun)(HD);

	}
}

void key_back_pressed(HD44780* HD, Joystick* J)
{
	if(J->pressed[3])
	{
		J->pressed[3] = false;
		if(key_back_fun) (*key_back_fun)(HD);

	}
}

void key_on_pressed(HD44780* HD, Joystick* J)
{
	if(J->pressed[4])
	{
		J->pressed[4] = false;
		if(key_on_fun) (*key_on_fun)(HD);


	}

}


