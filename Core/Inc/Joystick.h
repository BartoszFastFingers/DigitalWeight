/**
 * @file    Joystick_lib.h
 * @brief   Joystick handling library for STM32.
 *
 * Provides initialization, polling, and state-check functions
 * for a 5-way joystick (up, down, left, right, click) with debounce logic.
 *
 * Each direction and the click button are debounced to prevent
 * false triggers, and stable states are tracked.
 *
 * @author  Bartosz Terlecki
 * @date    2025-08-19
 */

#ifndef INC_JOYSTICK_LIB_H_
#define INC_JOYSTICK_LIB_H_

#include "Gen_libary.h"

/**
 * @brief Structure representing a 5-way joystick.
 */
typedef struct {
    pin_t up;          /**< Pin for the UP direction */
    pin_t down;        /**< Pin for the DOWN direction */
    pin_t right;       /**< Pin for the RIGHT direction */
    pin_t left;        /**< Pin for the LEFT direction */
    pin_t click;       /**< Pin for the CLICK button */

    uint32_t last_debounce[5]; /**< Timestamp of last state change for debounce logic */
    bool last_read[5];         /**< Previous raw readings for each input */
    bool pressed[5];           /**< Flags indicating if each input was pressed */
    bool stable[5];            /**< Debounced stable state of each input */
} Joystick;

/**
 * @brief Initializes the joystick with specified GPIO ports and pins.
 * @param J Pointer to Joystick structure.
 * @param up_port GPIO port for UP direction.
 * @param up_pin GPIO pin for UP direction.
 * @param right_port GPIO port for RIGHT direction.
 * @param right_pin GPIO pin for RIGHT direction.
 * @param down_port GPIO port for DOWN direction.
 * @param down_pin GPIO pin for DOWN direction.
 * @param left_port GPIO port for LEFT direction.
 * @param left_pin GPIO pin for LEFT direction.
 * @param click_port GPIO port for CLICK button.
 * @param click_pin GPIO pin for CLICK button.
 */
void JoystickInit(Joystick* J, GPIO_TypeDef* up_port, uint16_t up_pin,
                  GPIO_TypeDef* right_port, uint16_t right_pin,
                  GPIO_TypeDef* down_port, uint16_t down_pin,
                  GPIO_TypeDef* left_port, uint16_t left_pin,
                  GPIO_TypeDef* click_port, uint16_t click_pin);

/**
 * @brief Check if the UP button is pressed.
 * @param J Pointer to Joystick structure.
 * @return true if pressed, false otherwise.
 */
bool is_up_pressed(Joystick* J);

/**
 * @brief Check if the RIGHT button is pressed.
 * @param J Pointer to Joystick structure.
 * @return true if pressed, false otherwise.
 */
bool is_right_pressed(Joystick* J);

/**
 * @brief Check if the DOWN button is pressed.
 * @param J Pointer to Joystick structure.
 * @return true if pressed, false otherwise.
 */
bool is_down_pressed(Joystick* J);

/**
 * @brief Check if the LEFT button is pressed.
 * @param J Pointer to Joystick structure.
 * @return true if pressed, false otherwise.
 */
bool is_left_pressed(Joystick* J);

/**
 * @brief Check if the CLICK button is pressed.
 * @param J Pointer to Joystick structure.
 * @return true if pressed, false otherwise.
 */
bool is_click_pressed(Joystick* J);

/**
 * @brief Polls the joystick to update debounced and stable states.
 * @param J Pointer to Joystick structure.
 */
void pool_joystick(Joystick* J);

#endif /* INC_JOYSTICK_LIB_H_ */
