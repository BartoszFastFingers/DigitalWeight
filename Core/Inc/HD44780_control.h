/**
 * @file HD44780_control.h
 * @brief Structures for controlling HD44780 LCD display, keypad input, and brightness.
 *
 * Defines key data structures for managing:
 * - Keypad input handling
 * - LCD display state and menu navigation
 * - PWM-based backlight brightness control
 * - Integration with HX711 weight sensor for measurements
 *
 * This header provides the core control interfaces for the HD44780 module
 * in STM32-based projects.
 *
 * @author barte
 * @date Aug 21, 2025
 */
#ifndef INC_HD44780_CONTROL_H_
#define INC_HD44780_CONTROL_H_

#include "HX711_pro.h"
#include "Gen_libary.h"
#include "Keypad.h"


/**
 * @brief Structure for managing keypad input and string buffer.
 *
 * This structure holds a pointer to the associated keypad,
 * an offset to track the current input position, and a string
 * buffer to store the entered keys.
 */
typedef struct {
    Keypad* K;       /**< Pointer to the associated keypad */
    uint8_t K_offset; /**< Offset for key processing */
    char* K_str;     /**< Buffer to store key input as a string */
} Keypad_control;



/**
 * @brief Structure for LCD control and menu interaction.
 *
 * This structure combines menu state, PWM backlight control,
 * weight sensor reference, keypad input, and probe/sample
 * configuration to manage the HD44780 display in STM32 projects.
 */
typedef struct {
    uint8_t menu_state;        /**< Current menu state/index */
    pwm_channel_t brightness;  /**< PWM channel for controlling backlight brightness */
    HX711* weight;             /**< Pointer to HX711 handle for weight measurement */
    uint8_t probes;            /**< Number of measurement samples to average */
    Keypad_control* Keypad;    /**< Pointer to keypad control structure */
    uint32_t last_debounce;    /**< Timestamp of last keypad input for debounce */
} lcd_control;

#endif /* INC_HD44780_CONTROL_H_ */
