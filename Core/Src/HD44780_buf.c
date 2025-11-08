/*
 * HD44780_buf.c
 *
 *  Created on: Aug 20, 2025
 *      Author: barte
 */

#ifndef SRC_HD44780_BUF_C_
#define SRC_HD44780_BUF_C_


#include "HD44780_buf.h"
#include <stddef.h>

void HD44780_buf_set_location(HD44780* hd44780, uint8_t x, uint8_t y)
{
	hd44780->buf.x = x;
	hd44780->buf.y = y;
}

void HD44780_buf_char(HD44780* hd44780, char c)
{
	if(hd44780->buf.x < HD44780_COLS && hd44780->buf.y < HD44780_ROWS)
	{
	    hd44780->buf.frame[hd44780->buf.y][hd44780->buf.x] = c;
	    hd44780->buf.x++;
	    if(hd44780->buf.x == HD44780_COLS){
	        hd44780->buf.x = 0;
	        hd44780->buf.y++;
	        if(hd44780->buf.y == HD44780_ROWS)
	            hd44780->buf.y = 0;
	    }
	}
}

void HD44780_buf_clear(HD44780* hd44780)
{
	int x,y;
	for(y = 0; y < HD44780_ROWS; y++)
	{
	    for(x = 0; x < HD44780_COLS; x++)
	        hd44780->buf.frame[y][x] = ' ';
	}

	hd44780->buf.x = 0;
	hd44780->buf.y = 0;
}
void HD44780_buf_clear_m(HD44780* hd44780)
{
	int x,y;
	for(y = 1; y < HD44780_ROWS; y++)
	{
	    for(x = 0; x < HD44780_COLS; x++)
	        hd44780->buf.frame[y][x] = ' ';
	}

	hd44780->buf.x = 0;
	hd44780->buf.y = 0;
}

void HD44780_buf_clear_r(HD44780* hd44780)
{
	int x,y;
		for(y = 0; y < HD44780_ROWS - 1; y++)
		{
		    for(x = 0; x < HD44780_COLS; x++)
		        hd44780->buf.frame[y][x] = ' ';
		}

		hd44780->buf.x = 0;
		hd44780->buf.y = 0;
}

void HD44780_buf_str(HD44780* hd44780, const char *txt)
{
	if (txt == 0) return;
	while(*txt)
		HD44780_buf_char(hd44780, *txt++);
}

void HD44780_refresh_async(HD44780* hd44780)
{

	if(HD44780_is_busy(hd44780) == 0)
	{
	    if(hd44780->buf.asyn_x == HD44780_COLS) {
	    	hd44780->buf.asyn_x = 0;
	    	hd44780->buf.asyn_y++;
	        if(hd44780->buf.asyn_y == HD44780_ROWS)
	        	hd44780->buf.asyn_y = 0;

	        HD44780_set_location(hd44780, hd44780->buf.asyn_x, hd44780->buf.asyn_y);
	        return;
	    }

	    if(hd44780->buf.frame[hd44780->buf.asyn_y][hd44780->buf.asyn_x] !=
	    		hd44780->buf.frame_old[hd44780->buf.asyn_y][hd44780->buf.asyn_x]) {
	        if(!hd44780->buf.flag) {
	            HD44780_set_location(hd44780, hd44780->buf.asyn_x, hd44780->buf.asyn_y);
	            hd44780->buf.flag = 1;
	            return;
	        }
            HD44780_write_char(hd44780, hd44780->buf.frame[hd44780->buf.asyn_y][hd44780->buf.asyn_x]);
            hd44780->buf.frame_old[hd44780->buf.asyn_y][hd44780->buf.asyn_x] =
            		hd44780->buf.frame[hd44780->buf.asyn_y][hd44780->buf.asyn_x];

	    } else {
	    	hd44780->buf.flag = 0;
	    }

	    hd44780->buf.asyn_x++;
	}

}

void HD44780_refresh_sync(HD44780* hd44780)
{

	for(uint8_t y=0; y<HD44780_ROWS; y++)
	{
		HD44780_set_location(hd44780, 0, y);

		for(uint8_t x=0; x<HD44780_COLS; x++)
		{

			if( hd44780->buf.frame[y][x] != hd44780->buf.frame_old[y][x] )
			{
				if( !hd44780->buf.flag )
					HD44780_set_location(hd44780, x, y);
				HD44780_write_char(hd44780, hd44780->buf.frame[y][x]);
				hd44780->buf.frame_old[y][x] = hd44780->buf.frame[y][x];
				hd44780->buf.flag = 1;
			}
			else
				hd44780->buf.flag = 0;

		}

	}
}


#endif /* SRC_HD44780_BUF_C_ */
