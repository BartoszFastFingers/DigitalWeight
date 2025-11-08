/**
 * @file    Gen_libary.h
 * @brief   General-purpose STM32 library for GPIO, timers, PWM, and menu handling.
 *
 * Provides utility structures and functions for:
 * - Timer initialization and microsecond delays
 * - GPIO configuration and state changes
 * - PWM channel management
 * - Menu system with hierarchical navigation
 *
 * This library centralizes common hardware and UI functionalities
 * for STM32-based projects.
 *
 * @author  barte
 * @date    2025-08-08
 */

#ifndef INC_GEN_LIBARY_H_
#define INC_GEN_LIBARY_H_

#include <string.h>
#include <stdbool.h>
#include "tim.h"
#include "main.h"

/** Pointer to general-purpose timer used across the library */
extern TIM_HandleTypeDef* General_timer;

/**
 * @brief Structure representing a GPIO pin.
 */
typedef struct{
    GPIO_TypeDef* GPIO_PORT; /**< Pointer to the GPIO port (e.g., GPIOA, GPIOB) */
    uint16_t GPIO_PIN;       /**< GPIO pin number (e.g., GPIO_PIN_0, GPIO_PIN_1) */
} pin_t;

/**
 * @brief Buffer structure for HD44780 or other frame-based displays.
 */
typedef struct{
    uint8_t x, y;            /**< Current cursor coordinates */
    uint8_t asyn_x, asyn_y;  /**< Asynchronous or temporary coordinates for async updates */
    bool flag;               /**< Update flag indicating if buffer has changed */
    char **frame;            /**< Pointer to the current frame content */
    char **frame_old;        /**< Pointer to the previous frame content for comparison */
} buf_t;

/** Forward declaration of menu structure */
typedef struct menu_struct menu_t;

/**
 * @brief Structure representing a single menu item.
 */
struct menu_struct{
    const char* name;                 /**< Name of the menu item */
    menu_t* next;                     /**< Pointer to the next sibling menu item */
    menu_t* prev;                     /**< Pointer to the previous sibling menu item */
    menu_t* child;                    /**< Pointer to the first child menu item (sub-menu) */
    menu_t* parent;                   /**< Pointer to the parent menu item */
    void (*menu_function)(uint8_t* state); /**< Function to execute when the menu is selected */
};

/**
 * @brief Structure tracking the menu display state.
 */
typedef struct{
    menu_t *current;            /**< Pointer to the currently selected menu item */
    uint8_t index;              /**< Index of the current item within its level */
    uint8_t row_pos;            /**< Current row position on the display */
    uint8_t* row_pos_lvl;       /**< Array tracking row positions for each menu level */
    uint8_t header_size;        /**< Number of rows reserved for the menu header */
} menu_disp;

/**
 * @brief Structure representing a PWM output channel.
 */
typedef struct{
    volatile uint32_t *pwm_ccr; /**< Pointer to the timer's CCR register for duty cycle control */
    volatile uint32_t *pwm_arr; /**< Pointer to the timer's ARR register for period control */
    TIM_HandleTypeDef *htim;    /**< Handle to the STM32 timer peripheral */
    uint32_t channel;           /**< Timer channel number (e.g., TIM_CHANNEL_1) */
} pwm_channel_t;

/** Example menu items (extern declarations) */
extern menu_t menu_1, menu_2, menu_3;
extern menu_t sub_menu_1_1, sub_menu_1_2, sub_menu_2_1, sub_menu_3_1, sub_menu_3_2;

/** @brief Initialize general-purpose timer */
void Gen_libary_Timer_init(TIM_HandleTypeDef *htim);

/** @brief Delay execution for specified microseconds */
void delay_us(uint32_t us);

/**
 * @brief Change GPIO mode for push-pull output.
 * @param gpio Pin structure.
 * @param GPIO_INIT_MODE Mode for initialization (e.g., GPIO_MODE_OUTPUT_PP).
 */
void CHANGE_IO_GPIO_PUSH_PULL(pin_t gpio, uint32_t GPIO_INIT_MODE);

/**
 * @brief General GPIO configuration with additional parameters.
 * @param gpio Pin structure.
 * @param GPIO_INIT_MODE Mode for initialization (e.g., GPIO_MODE_AF_PP).
 * @param Pull Pull-up/Pull-down configuration (e.g., GPIO_NOPULL).
 * @param Speed GPIO speed (e.g., GPIO_SPEED_FREQ_HIGH).
 * @param Alternate Alternate function number.
 */
void CHANGE_GENERAL_IO_GPIO_PUSH_PULL(pin_t gpio, uint32_t GPIO_INIT_MODE,
                                      uint32_t Pull, uint32_t Speed, uint32_t Alternate);

#endif /* INC_GEN_LIBARY_H_ */
