/**
 * @file    HD44780_menu.h
 * @brief   Menu system functions for HD44780 LCD display.
 *
 * Provides functions to initialize, navigate, and refresh
 * hierarchical menus on the HD44780 LCD module.
 * Supports next/previous item navigation, enter/back actions,
 * and retrieving current menu level and index.
 *
 * This header defines the interface for menu management
 * in STM32-based projects with HD44780 displays.
 *
 * @author  Bartosz Terlecki
 * @date    2025-08-20
 */

#ifndef INC_HD44780_MENU_H_
#define INC_HD44780_MENU_H_

#include "HD44780.h"

/**
 * @brief Initializes the menu system.
 * @param hd44780 Pointer to HD44780 structure.
 * @param root Pointer to the root menu.
 */
void HD44780_init_menu(HD44780* hd44780, menu_t* root);

/**
 * @brief Navigate to the next menu item.
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_next_m(HD44780* hd44780);

/**
 * @brief Navigate to the previous menu item.
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_prev_m(HD44780* hd44780);

/**
 * @brief Enter the selected menu item (child menu).
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_enter_m(HD44780* hd44780);

/**
 * @brief Go back to the parent menu.
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_get_back_m(HD44780* hd44780);

/**
 * @brief Activate the selected menu item.
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_on_m(HD44780* hd44780);

/**
 * @brief Refresh the current menu display.
 * @param hd44780 Pointer to HD44780 structure.
 */
void HD44780_refresh_m(HD44780* hd44780);

/**
 * @brief Get the current menu level (depth).
 * @param hd44780 Pointer to HD44780 structure.
 * @return Current menu level.
 */
uint8_t HD44780_get_lvl_m(HD44780* hd44780);

/**
 * @brief Get the index of the current menu item.
 * @param hd44780 Pointer to HD44780 structure.
 * @return Index of the current menu item.
 */
uint8_t HD44780_get_index_m(HD44780* hd44780);

#endif /* INC_HD44780_MENU_H_ */
