/**
 * @file HD44780_callbacks.h
 * @brief Callback functions for HD44780 LCD display and menu control.
 *
 * Provides a set of callback functions for:
 * - Displaying weight and probe count
 * - Adjusting display brightness
 * - Setting custom offset values
 * - Handling menu navigation and state machine
 *
 * These functions interact with the HD44780 buffer, keypad input,
 * and HX711 weight sensor to manage real-time display updates
 * and menu interactions on STM32-based projects.
 *
 * @author barte
 * @date Aug 20, 2025
 */

#ifndef INC_HD44780_CALLBACKS_H_
#define INC_HD44780_CALLBACKS_H_

#include "Joystick_menu.h"
#define NULL 0

/**
 * @brief Callback to display current weight.
 * @param state Pointer to the menu state variable.
 */
void disp_weight_callback(uint8_t* state);

/**
 * @brief Callback to prompt for number of weight probes.
 * @param state Pointer to the menu state variable.
 */
void disp_weight_p_callback(uint8_t* state);

/**
 * @brief Callback to set a custom offset value via keypad.
 * @param state Pointer to the menu state variable.
 */
void disp_set_custom_callback(uint8_t* state);

/**
 * @brief Callback to enter brightness adjustment menu.
 * @param state Pointer to the menu state variable.
 */
void disp_brightness_callback(uint8_t* state);

/**
 * @brief Refreshes the LCD to show current brightness value as a bar graph.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_brightness_refresh(HD44780* HD);

/**
 * @brief Increases brightness by 10%.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_brightness_enter(HD44780* HD);

/**
 * @brief Decreases brightness by 10%.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_brightness_back(HD44780* HD);

/**
 * @brief Activates the brightness menu selection and returns to main menu.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_brightness_on(HD44780* HD);

/**
 * @brief Refreshes the LCD to display current weight.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_weight_refresh(HD44780* HD);

/**
 * @brief Refreshes the LCD to prompt for probe count input.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_weight_p_refresh(HD44780* HD);

/**
 * @brief Displays weight after probe measurements.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_weight_p_ok(HD44780* HD);

/**
 * @brief Refreshes the custom offset menu and reads input from keypad.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_set_custom_refresh(HD44780* HD);

/**
 * @brief Main state machine handling menu callbacks.
 *
 * Updates LCD and executes appropriate functions depending on the current menu state.
 * @param HD Pointer to the HD44780 structure.
 */
void disp_menu_fun_callbacks_state_machine(HD44780* HD);

#endif /* INC_HD44780_CALLBACKS_H_ */
