/*
 * HD44780.c
 *
 *  Created on: Aug 8, 2025
 *      Author: barte
 */

#include "HD44780.h"
#include <stdlib.h>


static void HD44780_send_cmd4(HD44780* hd44780, uint8_t data)
{
	int i;

	HAL_GPIO_WritePin(hd44780->E.GPIO_PORT, hd44780->E.GPIO_PIN, GPIO_PIN_SET);
	for(i = 0; i < 4; i++)
		HAL_GPIO_WritePin(hd44780->data_pins[i].GPIO_PORT, hd44780->data_pins[i].GPIO_PIN, (1 & (data >> i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(hd44780->E.GPIO_PORT, hd44780->E.GPIO_PIN, GPIO_PIN_RESET);
}

static uint8_t HD44780_read_cmd4(HD44780* hd44780)
{
	int i;
	uint8_t result = 0;
	HAL_GPIO_WritePin(hd44780->E.GPIO_PORT, hd44780->E.GPIO_PIN, GPIO_PIN_SET);
	for(i = 0; i < 4; i++)
		result |= ((uint8_t)HAL_GPIO_ReadPin(hd44780->data_pins[i].GPIO_PORT, hd44780->data_pins[i].GPIO_PIN) << i);
	HAL_GPIO_WritePin(hd44780->E.GPIO_PORT, hd44780->E.GPIO_PIN, GPIO_PIN_RESET);
	return result;
}


static void HD44780_send_cmd8(HD44780* hd44780, uint8_t data)
{

	HD44780_send_cmd4(hd44780, data >> 4);
	HD44780_send_cmd4(hd44780, data & 0x0F);
}

static uint8_t HD44780_read_cmd8(HD44780* hd44780)
{
    uint8_t result = (HD44780_read_cmd4(hd44780) & 0x0F) << 4;
    result |= (HD44780_read_cmd4(hd44780) & 0x0F);
    return result;
}

void HD44780_init(HD44780* hd44780,
		GPIO_TypeDef* rs_gpio_port, uint16_t rs_gpio_pin,
		GPIO_TypeDef* rw_gpio_port, uint16_t rw_gpio_pin,
		GPIO_TypeDef* e_gpio_port, uint16_t e_gpio_pin,
		GPIO_TypeDef* d4_gpio_port, uint16_t d4_gpio_pin,
		GPIO_TypeDef* d5_gpio_port, uint16_t d5_gpio_pin,
		GPIO_TypeDef* d6_gpio_port, uint16_t d6_gpio_pin,
		GPIO_TypeDef* d7_gpio_port, uint16_t d7_gpio_pin)
{
	int i;
	hd44780->RS.GPIO_PORT = rs_gpio_port;
	hd44780->RS.GPIO_PIN = rs_gpio_pin;
	hd44780->RW.GPIO_PORT = rw_gpio_port;
	hd44780->RW.GPIO_PIN = rw_gpio_pin;
	hd44780->E.GPIO_PORT = e_gpio_port;
	hd44780->E.GPIO_PIN = e_gpio_pin;
	hd44780->data_pins[0].GPIO_PORT = d4_gpio_port;
	hd44780->data_pins[0].GPIO_PIN = d4_gpio_pin;
	hd44780->data_pins[1].GPIO_PORT = d5_gpio_port;
	hd44780->data_pins[1].GPIO_PIN = d5_gpio_pin;
	hd44780->data_pins[2].GPIO_PORT = d6_gpio_port;
	hd44780->data_pins[2].GPIO_PIN = d6_gpio_pin;
	hd44780->data_pins[3].GPIO_PORT = d7_gpio_port;
	hd44780->data_pins[3].GPIO_PIN = d7_gpio_pin;
	hd44780->io = 1;

	hd44780->buf.flag = 0;
	hd44780->buf.asyn_x = 0;
	hd44780->buf.asyn_y = 0;
	hd44780->buf.x = 0;
	hd44780->buf.y = 0;
	hd44780->buf.frame = malloc(HD44780_ROWS * sizeof(char*));
	hd44780->buf.frame_old = malloc(HD44780_ROWS * sizeof(char*));



	for(i = 0; i < HD44780_ROWS; i++)
	{
	    hd44780->buf.frame[i] = malloc(HD44780_COLS * sizeof(char));
	    hd44780->buf.frame_old[i] = malloc(HD44780_COLS * sizeof(char));
	}


	for(i = 0 ;i < 4; i++)
		CHANGE_IO_GPIO_PUSH_PULL(hd44780->data_pins[i], GPIO_MODE_OUTPUT_PP);

	HD44780_set_IR(hd44780);
	HD44780_set_write(hd44780);

	HD44780_send_cmd4(hd44780, 0x03);
	delay_us(42000);
	HD44780_send_cmd4(hd44780, 0x03);
	delay_us(1000);
	HD44780_send_cmd4(hd44780, 0x03);
	delay_us(1000);
	HD44780_send_cmd4(hd44780, 0x02);
	delay_us(1000);
	HD44780_send_cmd8(hd44780, HD44780_FUN_SET | HD44780_2LINES | HD44780_4BIT | HD44780_5x8_FONT);
	delay_us(2000);
		HD44780_send_cmd8(hd44780, HD44780_DISPLAY_ON_OFF | HD44780_DISPLAY_ON | HD44780_DISPLAY_CURSOR_DISP_OFF | HD44780_BLINK_OFF);
		delay_us(1000);
		HD44780_send_cmd8(hd44780, HD44780_CLEAR_DISPLAY);
		HAL_Delay(5);
		HD44780_send_cmd8(hd44780, HD44780_ENTRY_MODE | HD44780_CUR_RIGHT);



		for (int y = 0; y < HD44780_ROWS; y++) {
		    for (int x = 0; x < HD44780_COLS; x++) {
		        hd44780->buf.frame[y][x] = ' ';
		        hd44780->buf.frame_old[y][x] = 0xFF;
		    }
		}

}

void HD44780_init_brightness(HD44780* hd44780, TIM_HandleTypeDef* timer,
		volatile uint32_t *pwm_reg_ccr, volatile uint32_t *pwm_reg_arr, uint32_t chan)
{
	hd44780->control.brightness.htim = timer;
	hd44780->control.brightness.pwm_ccr = pwm_reg_ccr;
	hd44780->control.brightness.pwm_arr = pwm_reg_arr;
	hd44780->control.brightness.channel = chan;
	HAL_TIM_PWM_Start(timer, chan);
}

void HD44780_init_weight(HD44780* hd44780, HX711* hx711)
{
	hd44780->control.weight = hx711;
}
void HD44780_init_keypad(HD44780* hd44780, Keypad* K){
	hd44780->control.Keypad = malloc(sizeof(Keypad_control));
	hd44780->control.Keypad->K = K;
	hd44780->control.Keypad->K_offset = 0;

	hd44780->control.Keypad->K_str = malloc(5*sizeof(char));
	hd44780->control.Keypad->K_str[0] = '\0';
	hd44780->control.last_debounce = 0;
}

void HD44780_set_IR(HD44780* hd44780)
{

	HAL_GPIO_WritePin(hd44780->RS.GPIO_PORT, hd44780->RS.GPIO_PIN, GPIO_PIN_RESET);
}

void HD44780_set_DR(HD44780* hd44780)
{
	HAL_GPIO_WritePin(hd44780->RS.GPIO_PORT, hd44780->RS.GPIO_PIN, GPIO_PIN_SET);
}

void HD44780_set_write(HD44780* hd44780)
{
	HAL_GPIO_WritePin(hd44780->RW.GPIO_PORT, hd44780->RW.GPIO_PIN, GPIO_PIN_RESET);
}

void HD44780_set_read(HD44780* hd44780)
{
	HAL_GPIO_WritePin(hd44780->RW.GPIO_PORT, hd44780->RW.GPIO_PIN, GPIO_PIN_SET);
}

void HD44780_pulse_E(HD44780* hd44780)
{
	HAL_GPIO_WritePin(hd44780->E.GPIO_PORT, hd44780->E.GPIO_PIN, GPIO_PIN_SET);
	delay_us(500);
	HAL_GPIO_WritePin(hd44780->E.GPIO_PORT, hd44780->E.GPIO_PIN, GPIO_PIN_RESET);
}


void HD44780_write_cmd(HD44780* hd44780, uint8_t data)
{
	HD44780_while_busy(hd44780);
	int i;
	HD44780_set_IR(hd44780);
	HD44780_set_write(hd44780);
	if(hd44780->io!=1)
	{
		for(i=0;i<4;i++)
			CHANGE_IO_GPIO_PUSH_PULL(hd44780->data_pins[i], GPIO_MODE_OUTPUT_PP);
		hd44780->io = 1;
	}
	HD44780_send_cmd8(hd44780, data);
}


void HD44780_write_char(HD44780* hd44780, uint8_t data)
{
	HD44780_while_busy(hd44780);
	int i;
	HD44780_set_DR(hd44780);
	HD44780_set_write(hd44780);
	if(hd44780->io!=1)
	{
		for(i=0;i<4;i++)
			CHANGE_IO_GPIO_PUSH_PULL(hd44780->data_pins[i], GPIO_MODE_OUTPUT_PP);
		hd44780->io = 1;
	}
	HD44780_send_cmd8(hd44780, data);
}

void HD44780_set_location(HD44780* hd44780, uint8_t x, uint8_t y)
{

	switch(y)
	{
	case 0:
		HD44780_write_cmd(hd44780, HD44780_SET_DDRAM | (HD44780_LINE1 + x) );
		break;
	case 1:
		HD44780_write_cmd(hd44780, HD44780_SET_DDRAM | (HD44780_LINE2 + x) );
		break;
	}
}

void HD44780_str(HD44780* hd44780, char* txt)
{
	while(*txt)
		HD44780_write_char(hd44780, *txt++);
}

uint8_t HD44780_read_BF_ADD(HD44780* hd44780)
{
	int i;
	HD44780_set_IR(hd44780);
	HD44780_set_read(hd44780);
	if(hd44780->io!=0)
	{
		for(i = 0; i < 4; i++)
			CHANGE_IO_GPIO_PUSH_PULL(hd44780->data_pins[i], GPIO_MODE_INPUT);
		hd44780->io = 0;
	}
	return HD44780_read_cmd8(hd44780);
}

bool HD44780_is_busy(HD44780* hd44780)
{
	return (HD44780_read_BF_ADD(hd44780) & 0x80) != 0;
}


void HD44780_while_busy(HD44780* hd44780)
{
	while(HD44780_is_busy(hd44780)) {}
}











