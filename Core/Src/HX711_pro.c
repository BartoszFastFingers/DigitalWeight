/*
 * HX711_pro.c
 *
 *  Created on: Aug 5, 2025
 *      Author: Bartosz Terlecki
 */


#include <math.h>
#include  <stdlib.h>
#include <stdio.h>
#include "HX711_pro.h"
#include "tim.h"

/// Time betweeen
#define T1_MIN  (1 )
#define T2_MAX  1
#define T3_MIN  (2)
#define T3_TYP 	(10 )
#define T3_MAX  (500 )
#define T4_MIN  (2 )
#define T4_TYP 	(10 )

#define IS_NOT_READY -2147483645




void HX711_default_init(HX711* hx711, GPIO_TypeDef* clk_port, uint16_t clk_pin,
		GPIO_TypeDef* data_port, uint16_t data_pin)
{
	hx711->CLK_GPIO_PORT = clk_port;
	hx711->CLK_GPIO_PIN = clk_pin;
	hx711->DATA_GPIO_PORT = data_port;
	hx711->DATA_GPIO_PIN = data_pin;
	hx711->gain = 128;
	hx711->offset_val = 0;

	HAL_GPIO_WritePin(hx711->CLK_GPIO_PORT, hx711->CLK_GPIO_PIN, GPIO_PIN_RESET);
}
void HX711_full_init(HX711* hx711, GPIO_TypeDef* clk_port, uint16_t clk_pin,
		GPIO_TypeDef* data_port, uint16_t data_pin, long init_offset_val, uint32_t init_gain)
{
	HX711_default_init(hx711, clk_port, clk_pin, data_port, data_pin);
	hx711->offset_val = init_offset_val;
	HX711_set_gain(hx711, init_gain);
}

bool isReady(HX711* hx711)
{
	if(HAL_GPIO_ReadPin(hx711->DATA_GPIO_PORT, hx711->DATA_GPIO_PIN) == GPIO_PIN_RESET) return true;

	return false;
}

void waitForHX711(HX711* hx711)
{
	while(!isReady(hx711)) {}
}

static uint8_t read_shift(HX711* hx711)
{
	uint8_t value = 0;
	uint8_t i;
	delay_us(T1_MIN);

	for(i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(hx711->CLK_GPIO_PORT, hx711->CLK_GPIO_PIN, GPIO_PIN_SET);
		delay_us(T2_MAX);
		if(HAL_GPIO_ReadPin(hx711->DATA_GPIO_PORT, hx711->DATA_GPIO_PIN) == GPIO_PIN_SET)
			value = (value << 1) | 1;
		else
			value = (value << 1);

		delay_us(T3_TYP);
		HAL_GPIO_WritePin(hx711->CLK_GPIO_PORT, hx711->CLK_GPIO_PIN, GPIO_PIN_RESET);
		delay_us(T4_TYP);

	}
	return value;

}

void HX711_set_gain(HX711* hx711, uint32_t gain)
{
	if (gain == 128 || gain == 64 || gain == 32) {
	        hx711->gain = gain;
	    } else {
	        hx711->gain = 128;
	    }
}

static void HX711_apply_gain(HX711* hx711)
{
	uint8_t pulses = hx711->gain == 128 ? 1 :
			hx711->gain == 64 ? 2 :
			3;
    for (uint8_t i = 0; i < pulses; i++)
    {
        HAL_GPIO_WritePin(hx711->CLK_GPIO_PORT, hx711->CLK_GPIO_PIN, GPIO_PIN_SET);
        delay_us(10);
        HAL_GPIO_WritePin(hx711->CLK_GPIO_PORT, hx711->CLK_GPIO_PIN, GPIO_PIN_RESET);
        delay_us(10);
    }
}

long HX711_read(HX711* hx711)
{

	if(!isReady(hx711)) {
		return IS_NOT_READY;
	}

	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	data[2] = read_shift(hx711);
	data[1] = read_shift(hx711);
	data[0] = read_shift(hx711);

	HX711_apply_gain(hx711);

	if (data[2] & 0x80) {
			filler = 0xFF;
		} else {
			filler = 0x00;
		}

	value = ( (unsigned long)(filler) << 24
				| (unsigned long)(data[2]) << 16
				| (unsigned long)(data[1]) << 8
				| (unsigned long)(data[0]) );

		return (long)(value);
}

void BubbleSort(long *arr, int n)
{
	int i, j;
	for(i = 0; i < n - 1; i++)
	{
		for(j = i + 1; j < n; j++)
		{
			if(arr[j] < arr[i]){
			long temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			}
		}
	}
}


long HX711_estimation_read(HX711* hx711,uint8_t  Estimation_samples)
{
	int i, j = 0;
	long temp[Estimation_samples];
	long val;
	for(i = 0; i < Estimation_samples; i++)
	{
		val = HX711_read(hx711);
		if(val == IS_NOT_READY) {
			i--;
		}
		else {
		temp[i] = val;
		}

	}
	BubbleSort(temp, Estimation_samples);
	long medium = Estimation_samples % 2 == 0 ? (temp[Estimation_samples/2 - 1] + temp[Estimation_samples/2])/2 :
			temp[Estimation_samples/2];


	long sum = 0;
	for(i = 0; i < Estimation_samples; i++) {
		if(abs(temp[i] - medium) <= TOLERATION)
		{
			sum += temp[i];
			j++;
		}
	}

	if(j == 0) return medium;


	val = sum/j - hx711->offset_val;
	hx711->last_estimation_read = val;

	return val;
}

void HX711_tar(HX711* hx711){

	hx711->offset_val += hx711->last_estimation_read;
}



void HX711_estimation_weight_read(HX711* hx711, uint8_t samples){
	long val = HX711_estimation_read(hx711, samples);
	if(val == IS_NOT_READY) return;

	hx711->last_estimation_read  = (float)val/981 - hx711->offset_val;

}

void HX711_weight_read_fast(HX711* hx711){

	long val = HX711_read(hx711);
	if(val == IS_NOT_READY) return;

	hx711->last_read = (float)val /981 - hx711->offset_val;

}
