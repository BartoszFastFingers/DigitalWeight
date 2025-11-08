/*
 * HD44780_callbacks.c
 *
 *  Created on: Aug 20, 2025
 *      Author: barte
 */

#include <stdio.h>
#include <stdlib.h>

#include "HD44780_callbacks.h"
#include "Keypad_control.h"
#include "HD44780_menu.h"
#include "HD44780_buf.h"
#include "HX711_pro.h"
#include "Keypad.h"
#include "stm32f1xx_it.h"

#define DO_NOTHING 0
#define BACK_TO_MENU 1
#define DISP_WEIGHT_CALLBACK 2
#define DISP_WEIGHT_P_CALLBACK 3
#define DISP_BRIGHTNESS_CALLBACK 4
#define CUSTOM_OFFSET_CALLBACK 5
#define DISP_WEIGHT_P			6

#define PERCENT_MIN 1
#define PERCENT_MAX 100

#define DISPLAY_DELAY 100
#define HD44780_COLS  16


static uint32_t last_debounce = 0;

void disp_weight_callback(uint8_t* state){
	*state = DISP_WEIGHT_CALLBACK;
}

void disp_weight_p_callback(uint8_t* state){
	*state = DISP_WEIGHT_P_CALLBACK;
}
void disp_set_custom_callback(uint8_t* state)
{
	*state = CUSTOM_OFFSET_CALLBACK;
}

void disp_brightness_callback(uint8_t* state){
	*state = DISP_BRIGHTNESS_CALLBACK;
}


static void clear_buffer(HD44780* HD)
{
	HD->control.Keypad->K_str[0] = '\0';
	HD->control.Keypad->K_offset = 0;
}

static void set_offset_by_keypad(HD44780* HD)
{
	HD->control.weight->offset_val -= strtol(HD->control.Keypad->K_str, NULL, 10);
	clear_buffer(HD);
}

static void set_read_estimation_by_keypad(HD44780* HD)
{
	HD44780_buf_clear(HD);
	HD44780_buf_set_location(HD, 0, 1);
	HD44780_buf_str(HD, "Waiting . . .");
	HD44780_refresh_sync(HD);
	HD->control.probes = atoi(HD->control.Keypad->K_str);
	HD->control.weight->last_estimation_read = 0;
	HX711_estimation_weight_read(HD->control.weight, HD->control.probes);
	HD->control.menu_state = DISP_WEIGHT_P;


}

static void set_automatic_offset(HD44780* HD)
{
	clear_buffer(HD);

	    snprintf(HD->control.Keypad->K_str,
	             5,
	             "%.0f",
	            -HD->control.weight->last_read);

	    HD->control.Keypad->K_offset = strlen(HD->control.Keypad->K_str);
}


void disp_brightness_back(HD44780* HD){
	int32_t bright_value = (int32_t)((*HD->control.brightness.pwm_ccr) - (*HD->control.brightness.pwm_arr)*10/100);
	if(PERCENT_MIN > bright_value) bright_value = PERCENT_MIN;
	*HD->control.brightness.pwm_ccr = bright_value;
	disp_brightness_refresh(HD);
}

void disp_brightness_enter(HD44780* HD){
	uint32_t bright_value = (*HD->control.brightness.pwm_ccr) + (*HD->control.brightness.pwm_arr)*10/100;
	if(bright_value > PERCENT_MAX) bright_value = PERCENT_MAX;
	*HD->control.brightness.pwm_ccr = bright_value;

	disp_brightness_refresh(HD);
}

void disp_brightness_on(HD44780* HD){
	HD->control.menu_state = BACK_TO_MENU;
}


void disp_brightness_refresh(HD44780* HD){

	uint32_t percent = ((*HD->control.brightness.pwm_ccr)*100)/ (*HD->control.brightness.pwm_arr);
	if(percent > PERCENT_MAX) percent = PERCENT_MAX;
	if(percent < PERCENT_MIN) percent = PERCENT_MIN;

	HD44780_buf_clear(HD);
	HD44780_buf_set_location(HD, 0, 0);
	HD44780_buf_str(HD, "-- BRIGHTNESS --");



	memset(&HD->buf.frame[1][3], '-', 10);
	memset(&HD->buf.frame[1][3], 0xff, percent/10);
	HD44780_refresh_sync(HD);
}




void disp_weight_refresh(HD44780* HD)
{

	char read_str[16];
	HD44780_buf_clear(HD);
	HD44780_buf_set_location(HD, 0, 0);
	HD44780_buf_str(HD, "---- WEIGHT ----");

	float read = HD->control.weight->last_read;
	snprintf(read_str, 16,"%.0f g", read);
	HD44780_buf_set_location(HD, 0, 1);
	HD44780_buf_str(HD,read_str);
	HD44780_refresh_sync(HD);
}

void disp_weight_p_refresh(HD44780* HD)
{


	HD44780_buf_clear(HD);
	HD44780_buf_set_location(HD, 0, 0);
	HD44780_buf_str(HD, "How many probes?");
	HD44780_buf_set_location(HD, 0, 1);
	Keypad_digit_control(HD, HD->control.Keypad, '+', 3, set_read_estimation_by_keypad);
	HD44780_buf_str(HD, "> ");
	HD44780_buf_str(HD, HD->control.Keypad->K_str);
	HD44780_refresh_sync(HD);
}

void disp_weight_p_ok(HD44780* HD)
{
	char read_str[16];
	HD44780_buf_clear(HD);
	if(Keypad_return_val(HD->control.Keypad->K) == '#') HD->control.menu_state = DISP_WEIGHT_P_CALLBACK;
	HD44780_buf_set_location(HD, 0, 0);
	HD44780_buf_str(HD, "---- WEIGHT ----");
	float read = HD->control.weight->last_estimation_read;
	snprintf(read_str,16,"%.0f g",read);
	HD44780_buf_set_location(HD, 0, 1);
	HD44780_buf_str(HD, read_str);
	HD44780_refresh_sync(HD);
}

void disp_set_custom_refresh(HD44780* HD)
{


	if((SysTicks - last_debounce) > 100){
		last_debounce = SysTicks;
	char read_str[16];
	HD44780_buf_clear(HD);
	HD44780_buf_set_location(HD, 0, 0);


	HD44780_buf_str(HD, "CUR. W: ");

	snprintf(read_str,16,"%.0f g", HD->control.weight->last_read);
	HD44780_buf_str(HD, read_str);
	}


	HD44780_buf_clear_m(HD);
	Keypad_digit_control(HD, HD->control.Keypad, '-', 5, set_offset_by_keypad);

	HD44780_buf_set_location(HD, 0, 1);
	HD44780_buf_str(HD, "OFFSET: ");
	HD44780_buf_str(HD, HD->control.Keypad->K_str);
	if(HD->control.Keypad->K_str[0] != '\0' && !(
		HD->control.Keypad->K_str[0] == '-' && HD->control.Keypad->K_str[1] == '\0'))
		HD44780_buf_str(HD, " g");
	HD44780_refresh_sync(HD);
}


void disp_menu_fun_callbacks_state_machine(HD44780* HD)
{
	switch((unsigned int)HD->control.menu_state)
	{
	case DO_NOTHING:
		break;
	default:
		break;
	case BACK_TO_MENU:
		key_next_fun = HD44780_next_m;
		key_prev_fun = HD44780_prev_m;
		key_back_fun = HD44780_get_back_m;
		key_enter_fun = HD44780_enter_m;
		key_on_fun = HD44780_on_m;
		HD44780_refresh_m(HD);
		HD->control.menu_state = DO_NOTHING;
		break;
	case DISP_WEIGHT_CALLBACK:
		key_next_fun = NULL;
		key_prev_fun = NULL;
		key_enter_fun = NULL;
		key_on_fun = NULL;
		key_back_fun = HD44780_get_back_m;

		disp_weight_refresh(HD);
		break;
	case DISP_WEIGHT_P_CALLBACK:
		key_next_fun = NULL;
		key_prev_fun = NULL;
		key_enter_fun = NULL;
		key_on_fun = NULL;
		disp_weight_p_refresh(HD);
		break;
	case DISP_BRIGHTNESS_CALLBACK:
		key_next_fun = NULL;
		key_prev_fun = NULL;
		key_back_fun = disp_brightness_back;
		key_enter_fun = disp_brightness_enter;
		key_on_fun = disp_brightness_on;
		disp_brightness_refresh(HD);
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HD->control.menu_state = DO_NOTHING;
		break;
	case CUSTOM_OFFSET_CALLBACK:
		key_next_fun = NULL;
		key_prev_fun = NULL;
		key_on_fun = set_automatic_offset;
		disp_set_custom_refresh(HD);
		break;
	case DISP_WEIGHT_P:
		disp_weight_p_ok(HD);
		break;
	}
}



