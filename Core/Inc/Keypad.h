/**
 * @file    Keypad.h
 * @brief   Keypad driver library for STM32.
 *
 * Provides functions and structures to initialize and scan
 * a matrix keypad with debounce handling. Supports retrieval
 * of pressed key values and stable key states.
 *
 * @author  Bartosz Terlecki
 * @date    2025-08-24
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include <stdbool.h>
#include "Gen_libary.h"

/**
 * @brief Structure representing a matrix keypad.
 */
typedef struct {
    char* key_map;         /**< Mapping of keys to characters */

    bool* read;            /**< Current raw key states */
    bool* stable;          /**< Debounced key states */
    bool* last_read;       /**< Previous raw key states */
    bool* pressed;         /**< Flags indicating which keys were pressed */

    uint32_t* last_debounce; /**< Timestamp of last state change for debounce logic */

    pin_t* R;              /**< Array of row pins */
    pin_t* C;              /**< Array of column pins */

    uint8_t rows;          /**< Number of rows in the keypad */
    uint8_t columns;       /**< Number of columns in the keypad */
} Keypad;

/**
 * @brief Initializes the keypad structure with row/column pins.
 */
void Keypad_init(Keypad* keypad, pin_t* R_pins, pin_t* C_pins, uint8_t rows, uint8_t columns, char* keymap);

/**
 * @brief Scans the keypad and updates key states.
 */
void Keypad_scan(Keypad* keypad);

/**
 * @brief Updates pressed flags based on stable key states.
 */
void Keypad_get_pressed(Keypad* keypad);

/**
 * @brief Returns the character of the currently pressed key.
 * @return Pressed key character.
 */
char Keypad_return_val(Keypad* keypad);

#endif /* INC_KEYPAD_H_ */
