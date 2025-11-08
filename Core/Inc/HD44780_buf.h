/**
 * @file    HD44780_buf.h
 * @brief   Buffered interface for HD44780 LCD.
 *
 * This file provides function prototypes for writing to an HD44780 LCD
 * using an internal buffer. Functions include cursor control, character
 * and string writing, partial clearing, and synchronous/asynchronous refresh.
 *
 * @author  Bartosz Terlecki
 * @date    2025-08-20
 */

#ifndef INC_HD44780_BUF_H_
#define INC_HD44780_BUF_H_


#include "HD44780.h"

/**
 * @brief Sets the cursor position in the LCD buffer.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param x Column position (0..HD44780_COLS-1).
 * @param y Row position (0..HD44780_ROWS-1).
 */
void HD44780_buf_set_location(HD44780* hd44780, uint8_t x, uint8_t y);
/**
 * @brief Writes a single character to the LCD buffer at the current cursor position.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param c Character to write.
 */
void HD44780_buf_char(HD44780* hd44780, char c);

/**
 * @brief Clears the entire LCD buffer.
 *
 * This function sets all buffer contents to blank characters.
 *
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_buf_clear(HD44780* hd44780);
/**
 * @brief Clears the main content area of the LCD buffer (excluding headers or menu).
 *
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_buf_clear_m(HD44780* hd44780);
/**
 * @brief Clears only the current row in the LCD buffer.
 *
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_buf_clear_r(HD44780* hd44780);
/**
 * @brief Writes a string to the LCD buffer at the current cursor position.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param txt Null-terminated string to write.
 */
void HD44780_buf_str(HD44780* hd44780, const char *txt);
/**
 * @brief Synchronously refreshes the physical LCD from the buffer.
 *
 * This function writes the entire buffer to the LCD and waits
 * until all operations are complete.
 *
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_refresh_sync(HD44780* hd44780);

/**
 * @brief Asynchronously refreshes the physical LCD from the buffer.
 *
 * This function starts updating the LCD from the buffer but returns immediately,
 * allowing other code to run while the display is updated in the background.
 *
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_refresh_async(HD44780* hd44780);

#endif /* INC_HD44780_BUF_H_ */
