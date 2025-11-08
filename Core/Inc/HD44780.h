/* USER CODE BEGIN Header */
/**
 * @file    HD44780.h
 * @brief   Function prototypes and structures for controlling HD44780 LCD.
 *
 * This file contains all definitions, macros, and function prototypes
 * required to initialize and control an HD44780-compatible LCD in 4-bit mode,
 * including support for brightness PWM, keypad input, weight measurement
 * via HX711, and hierarchical menu display.
 */
/* USER CODE END Header */

#ifndef INC_HD44780_H_
#define INC_HD44780_H_


#include "Gen_libary.h"
#include "HD44780_control.h"

#define HD44780_ROWS  2
#define HD44780_COLS  16


/**
 * @brief Structure representing an HD44780 LCD display instance.
 *
 * This structure holds all the necessary pins, buffers, and control
 * structures to interface with an HD44780-based LCD in 4-bit mode.
 * It also contains menu display state and control flags for brightness,
 * weight sensor (HX711), and keypad input.
 */
typedef struct {
	pin_t RS;								/**< Register Select pin */
	pin_t RW;								/**< Read/Write pin */
	pin_t E;								/**< Enable pin */
	pin_t data_pins[4];						/**< Data pins D4-D7 for 4-bit communication */

	bool io; 								/**< I/O direction: input = 0, output = 1 */
	buf_t buf;								/**< Internal buffer for LCD content */

	menu_disp menu;							/**< Current menu display structure */
	menu_t* menu_root;						/**< Pointer to the root menu */
	lcd_control control;					/**< LCD control flags and settings */
}HD44780;


#define HD44780_CLEAR_DISPLAY 0x01

#define HD44780_RETURN_HOME 0X02

#define HD44780_ENTRY_MODE 0X04
	#define HD44780_SHIFT_CURSOR 0x00
	#define HD44780_SHIFT_DISP 0x01
	#define HD44780_CUR_LEFT 0x00
	#define HD44780_CUR_RIGHT 0x02

#define HD44780_DISPLAY_ON_OFF 0x08
	#define HD44780_DISPLAY_ON 0x04
	#define HD44780_DISPLAY_OFF 0x00
	#define HD44780_DISPLAY_CURSOR_DISP_ON 0x02
	#define HD44780_DISPLAY_CURSOR_DISP_OFF 0x00
	#define HD44780_BLINK_ON 0x01
	#define HD44780_BLINK_OFF 0x00

#define HD44780_SHIFT 0x10
	#define HD44780_DISPLAY_SHIFT 0x08
	#define HD44780_CURSOR_SHIFT 0x00
	#define HD44780_RIGHT_SHIFT 0x04
	#define HD44780_LEFT_SHIFT 0x00

#define HD44780_FUN_SET 0x20
	#define HD44780_8BIT 0x10
	#define HD44780_4BIT 0x00
	#define HD44780_2LINES 0x08
	#define HD44780_1LINE 0x00
	#define HD44780_5x10_FONT 0x04
	#define HD44780_5x8_FONT 0x00

#define HD44780_SET_CGRAM 0x40
#define HD44780_SET_DDRAM 0x80

#define HD44780_LINE1 0x00
#define HD44780_LINE2 0x40


/**
 * @brief Initializes the HD44780 LCD structure with GPIO pins.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param rs_gpio_port GPIO port for RS pin.
 * @param rs_gpio_pin GPIO pin for RS.
 * @param rw_gpio_port GPIO port for RW pin.
 * @param rw_gpio_pin GPIO pin for RW.
 * @param e_gpio_port GPIO port for E pin.
 * @param e_gpio_pin GPIO pin for E.
 * @param d4_gpio_port GPIO port for D4 pin.
 * @param d4_gpio_pin GPIO pin for D4.
 * @param d5_gpio_port GPIO port for D5 pin.
 * @param d5_gpio_pin GPIO pin for D5.
 * @param d6_gpio_port GPIO port for D6 pin.
 * @param d6_gpio_pin GPIO pin for D6.
 * @param d7_gpio_port GPIO port for D7 pin.
 * @param d7_gpio_pin GPIO pin for D7.
 */
void HD44780_init(HD44780* hd44780,
		GPIO_TypeDef* rs_gpio_port, uint16_t rs_gpio_pin,
		GPIO_TypeDef* rw_gpio_port, uint16_t rw_gpio_pin,
		GPIO_TypeDef* e_gpio_port, uint16_t e_gpio_pin,
		GPIO_TypeDef* d4_gpio_port, uint16_t d4_gpio_pin,
		GPIO_TypeDef* d5_gpio_port, uint16_t d5_gpio_pin,
		GPIO_TypeDef* d6_gpio_port, uint16_t d6_gpio_pin,
		GPIO_TypeDef* d7_gpio_port, uint16_t d7_gpio_pin);


/**
 * @brief Initializes PWM for LCD backlight brightness.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param timer Pointer to STM32 timer handle.
 * @param pwm_reg_ccr Pointer to CCR register of the timer.
 * @param pwm_reg_arr Pointer to ARR register of the timer.
 * @param chan Timer channel number.
 */
void HD44780_init_brightness(HD44780* hd44780, TIM_HandleTypeDef* timer,
		volatile uint32_t *pwm_reg_ccr,volatile uint32_t *pwm_reg_arr, uint32_t chan);


/**
 * @brief Links a HX711 weight sensor to the LCD structure.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param hx711 Pointer to HX711 handle.
 */
void HD44780_init_weight(HD44780* hd44780, HX711* hx711);
/**
 * @brief Links a keypad to the LCD structure for menu input.
 *
 * @param hd44780 Pointer to HD44780 structure.
 * @param K Pointer to Keypad structure.
 */
void HD44780_init_keypad(HD44780* hd44780, Keypad* K);

void HD44780_set_IR(HD44780* hd44780);	/**< Set LCD to instruction register mode */
void HD44780_set_DR(HD44780* hd44780);	/**< Set LCD to data register mode */
void HD44780_set_write(HD44780* hd44780);	/**< Set LCD to write mode */
void HD44780_set_read(HD44780* hd44780);	/**< Set LCD to read mode */
void HD44780_pulse_E(HD44780* hd44780);		/**< Generate enable pulse for LCD */

void HD44780_write_cmd(HD44780* hd44780, uint8_t data);	/**< Write command byte to LCD */
void HD44780_write_char(HD44780* hd44780, uint8_t data);	/**< Write character to LCD */



void HD44780_set_location(HD44780* hd44780, uint8_t x, uint8_t y); /**< Set cursor position */
void HD44780_str(HD44780* hd44780, char* txt); /**< Print string on LCD */
uint8_t HD44780_read_BF_ADD(HD44780* hd44780); /**< Read busy flag and address counter */
bool HD44780_is_busy(HD44780* hd44780);       /**< Check if LCD is busy */
void HD44780_while_busy(HD44780* hd44780);    /**< Wait until LCD is not busy */



#endif /* INC_HD44780_H_ */
