/*
 * HD44780_menu.c
 *
 *  Created on: Aug 20, 2025
 *      Author: barte
 */

#include <stdlib.h>
#include "HD44780_buf.h"
#include "HD44780_menu.h"
#include <stddef.h>

#define NULL 0
#define BACK_TO_MENU 1

void HD44780_init_menu(HD44780* hd44780, menu_t* root)
{
	hd44780->menu.row_pos_lvl = malloc(2*sizeof(uint8_t));
	hd44780->menu_root = root;
	hd44780->menu.current = root;
	hd44780->menu.index = HD44780_get_index_m(hd44780);
	hd44780->menu.row_pos = 0;
	hd44780->menu.header_size = 2;
	hd44780->control.menu_state = 0;
	HD44780_refresh_m(hd44780);
}


void HD44780_next_m(HD44780* hd44780){
	if(hd44780->menu.current->next)
	{
		hd44780->menu.current = hd44780->menu.current->next;
		hd44780->menu.index++;
		if(++hd44780->menu.row_pos > HD44780_ROWS - hd44780->menu.header_size)
			hd44780->menu.row_pos = HD44780_ROWS - hd44780->menu.header_size;
	}
	else
	{
		hd44780->menu.index = 0;
		hd44780->menu.row_pos = 0;

		if(hd44780->menu.current->parent) hd44780->menu.current = (hd44780->menu.current->parent)->child;
		else  hd44780->menu.current = hd44780->menu_root;
	}


	HD44780_refresh_m(hd44780);
}

void HD44780_prev_m(HD44780* hd44780){
	if(hd44780->menu.current->prev)
		hd44780->menu.current = hd44780->menu.current->prev;

	if(hd44780->menu.index){
		hd44780->menu.index--;
		if(hd44780->menu.row_pos > 0) hd44780->menu.row_pos--;
	}
	else{
		hd44780->menu.index = HD44780_get_index_m(hd44780);

		if(hd44780->menu.index >= HD44780_ROWS - hd44780->menu.header_size) hd44780->menu.row_pos = HD44780_ROWS - hd44780->menu.header_size;
		else hd44780->menu.row_pos = hd44780->menu.index;
	}

	HD44780_refresh_m(hd44780);
}



void HD44780_enter_m(HD44780* hd44780){


	if(hd44780->menu.current->menu_function)
					hd44780->menu.current->menu_function(&hd44780->control.menu_state);

	if(hd44780->menu.current->child)
	{
		hd44780->menu.row_pos_lvl[HD44780_get_lvl_m(hd44780)] = hd44780->menu.row_pos;

		hd44780->menu.index = 0;
		hd44780->menu.row_pos = 0;

		hd44780->menu.current = hd44780->menu.current->child;

		HD44780_refresh_m(hd44780);

	}

}

void HD44780_get_back_m(HD44780* hd44780){

	hd44780->control.menu_state = BACK_TO_MENU;
	if(hd44780->menu.current->parent)
	{
		hd44780->menu.current = hd44780->menu.current->parent;
		hd44780->menu.row_pos = hd44780->menu.row_pos_lvl[HD44780_get_lvl_m(hd44780)];
		hd44780->control.Keypad->K_str[0] = '\0';
		hd44780->control.Keypad->K_offset = 0;


		hd44780->menu.index = HD44780_get_index_m(hd44780);
		HD44780_refresh_m(hd44780);
	}

}
void HD44780_on_m(HD44780* hd44780)
{
	if(hd44780->menu.current->menu_function)
				hd44780->menu.current->menu_function(&hd44780->control.menu_state);
	HD44780_refresh_m(hd44780);
}



uint8_t HD44780_get_index_m(HD44780* hd44780){
	menu_t* temp = hd44780->menu.current;
	uint8_t i = 0;

	if(hd44780->menu.current->parent)
		temp = (hd44780->menu.current->parent)->child;
	else
		temp = hd44780->menu_root;

	while(temp != hd44780->menu.current){
		temp = temp->next;
		i++;
	}
	return i;
}

uint8_t HD44780_get_lvl_m(HD44780* hd44780)
{
	menu_t* temp = hd44780->menu.current;
	uint8_t i;

	if(!hd44780->menu.current->parent) return 0;

	while(temp->parent != 0)
	{
		temp = temp->parent;
		i++;
	}
	return i;
}


void HD44780_refresh_m(HD44780* hd44780)
{
	menu_t* temp;
	uint8_t i = 0;




	memset(hd44780->buf.frame[0], '-', HD44780_COLS);

	if(hd44780->menu.current && hd44780->menu.current->parent){
		temp = (hd44780->menu.current->parent)->child;
		hd44780->menu.header_size = 1;
	}
	else if(hd44780->menu.index != 0)
	{
		temp = hd44780->menu_root;
		hd44780->menu.header_size = 1;
	}
	else
	{
		temp = hd44780->menu_root;
		hd44780->menu.header_size = 2;
		HD44780_buf_set_location(hd44780, 0, 0);

		HD44780_buf_str(hd44780, "----- MENU -----");
	}

	for(i = 0; i != hd44780->menu.index - hd44780->menu.row_pos; i++)
	{

			temp = temp->next;
	}
	if(hd44780->menu.current && (hd44780->menu.current->parent || hd44780->menu.index != 0)){
		HD44780_buf_clear(hd44780);
		for(i = 0; i < HD44780_ROWS; i++)
			{
				HD44780_buf_set_location(hd44780, 0, i);

				if(temp == hd44780->menu.current) HD44780_buf_char(hd44780, 62);
				else HD44780_buf_char(hd44780, ' ');

				HD44780_buf_set_location(hd44780, 2, i);
				if(temp && temp->name)
					HD44780_buf_str(hd44780, temp->name);

				temp = temp->next;
				if(!temp) break;
			}
	}
	else
	{
		HD44780_buf_clear_m(hd44780);
		for(i = 1; i < HD44780_ROWS; i++)
			{
				HD44780_buf_set_location(hd44780, 0, i);

				if(temp == hd44780->menu.current) HD44780_buf_char(hd44780, 62);
				else HD44780_buf_char(hd44780, ' ');

				HD44780_buf_set_location(hd44780, 2, i);
				if(temp && temp->name)
					HD44780_buf_str(hd44780, temp->name);

				temp = temp->next;
				if(!temp) break;
			}
	}



	HD44780_refresh_sync(hd44780);
}
