/**
 * @file    HX711_pro.h
 * @author  Bartosz Terlecki
 * @date    2025-08-05
 * @brief   HX711 load cell amplifier driver for STM32.
 *
 * Provides initialization, raw read, filtered read and tare functionality.
 */

#ifndef INC_HX711_PRO_H_
#define INC_HX711_PRO_H_

#include <stdbool.h>

#include "Gen_libary.h"

#define TOLERATION 250


/**
 * @brief Structure representing an HX711 load cell amplifier.
 *
 * This structure holds the GPIO configuration for the data and clock pins,
 * as well as calibration and last read values for the HX711 module.
 */
typedef struct{
	GPIO_TypeDef *CLK_GPIO_PORT;	/**< GPIO port for clock pin */
	uint16_t CLK_GPIO_PIN;			/**< GPIO pin number for clock */
	GPIO_TypeDef *DATA_GPIO_PORT;	/**< GPIO port for data pin */
	uint16_t DATA_GPIO_PIN;			/**< GPIO pin number for data */


	uint8_t gain;					/**< Gain factor (128, 64, or 32) */
	float offset_val;				/**< Offset value for tare calibration */
	float last_estimation_read;		/**< Last filtered value */
	float last_read;				/**< Last raw value */
}HX711;

#endif /* INC_HX711_PRO_H_ */




/**
 * @brief Initializes HX711 with default settings.
 *
 * Sets gain = 128 (highest sensitivity) and offset = 0.
 * This function provides a quick way to start using HX711
 * without advanced configuration.
 *
 * @param hx711     Pointer to HX711 handle structure.
 * @param clk_port  GPIO port for CLK pin.
 * @param clk_pin   GPIO pin for CLK pin.
 * @param data_port GPIO port for DATA pin.
 * @param data_pin  GPIO pin for DATA pin.
 */
void HX711_default_init(HX711* hx711, GPIO_TypeDef* clk_port, uint16_t clk_pin,
		GPIO_TypeDef* data_port, uint16_t data_pin);

/**
 * @brief Initializes HX711 with custom offset and gain.
 *
 * Unlike HX711_default_init(), this function allows you to specify
 * both the initial offset value and the gain setting.
 * Useful when calibration values are already known or when more
 * control over initialization is needed.
 *
 * @param hx711           Pointer to HX711 handle structure.
 * @param clk_port        GPIO port for CLK pin.
 * @param clk_pin         GPIO pin for CLK pin.
 * @param data_port       GPIO port for DATA pin.
 * @param data_pin        GPIO pin for DATA pin.
 * @param init_offset_val Initial offset value (tare calibration).
 * @param init_gain       Gain factor (valid values: 128, 64, 32).
 */
void HX711_full_init(HX711* hx711, GPIO_TypeDef* clk_port, uint16_t clk_pin,
		GPIO_TypeDef* data_port, uint16_t data_pin, long init_offset_val, uint32_t init_gain);


/**
 * @brief Reads a single raw measurement from the HX711 ADC.
 *
 * This function performs a blocking read of the HX711 data.
 * It first checks if the HX711 is ready using `isReady()`. If
 * the device is not ready, it returns the special value `IS_NOT_READY`.
 *
 * The function reads 3 bytes from the HX711, applies the configured
 * gain, and converts the 24-bit two's complement value into a signed long.
 *
 * @param hx711 Pointer to HX711 handle structure.
 * @return Signed 32-bit value representing the raw ADC measurement.
 *         Returns `IS_NOT_READY` if HX711 is not ready.
 */
long HX711_read(HX711* hx711);

/* More advanced read based on multiple samples (Estimation_samples).
   Provides noise filtering and is used by HX711_tar().
   Note: more samples = longer processing time. */

/**
 * @brief Performs a filtered read of the HX711 using multiple samples.
 *
 * This function reads the HX711 ADC multiple times (specified by `Estimation_samples`),
 * sorts the results, and computes a filtered average by excluding outliers beyond a
 * defined tolerance (`TOLERATION`). It is useful for reducing noise in weight measurements.
 *
 * Steps performed by the function:
 * 1. Reads `Estimation_samples` raw values using `HX711_read()`.
 *    - If the device is not ready (`IS_NOT_READY`), the sample is retried.
 * 2. Sorts the array of samples using `BubbleSort()`.
 * 3. Computes the median of the samples.
 * 4. Sums only the values that are within `TOLERATION` of the median.
 * 5. Calculates the average of the filtered samples and subtracts the HX711 offset.
 * 6. Stores the filtered result in `hx711->last_estimation_read` and returns it.
 *
 * @param hx711            Pointer to HX711 handle structure.
 * @param Estimation_samples Number of samples to take for estimation/filtering.
 * @return Filtered measurement as a long integer. If no sample is within tolerance,
 *         the function returns the median of all samples.
 */
long HX711_estimation_read(HX711* hx711,uint8_t  Estimation_samples);

/**
 * @brief Sets the current reading as the zero offset (tare) for the HX711.
 *
 * This function updates the HX711 handle's `offset_val` by adding the last filtered
 * measurement (`last_estimation_read`). After calling this function, subsequent
 * readings will be zeroed relative to the current weight on the load cell.
 *
 * @param hx711 Pointer to HX711 handle structure.
 */
void HX711_tar(HX711* hx711);

/**
 * @brief Sets the gain factor for the HX711 ADC.
 *
 * This function updates the `gain` field in the HX711 handle structure.
 * Valid gain values are 128 (highest sensitivity), 64, or 32 (lowest sensitivity).
 * If an invalid value is provided, the gain defaults to 128.
 *
 * @param hx711 Pointer to HX711 handle structure.
 * @param gain  Desired gain factor (valid values: 128, 64, 32).
 */
void HX711_set_gain(HX711* hx711, uint32_t gain);

/**
 * @brief Reads the weight from HX711 using multiple samples and filtering.
 *
 * This function performs a filtered read using `HX711_estimation_read()` with
 * the specified number of samples. The raw value is then converted to a weight
 * approximation by dividing by 981 and subtracting the current offset.
 *
 * @param hx711   Pointer to HX711 handle structure.
 * @param samples Number of samples to take for estimation/filtering.
 */
void HX711_estimation_weight_read(HX711* hx711, uint8_t samples);

/**
 * @brief Reads the weight from HX711 with a single raw measurement (fast read).
 *
 * This function reads a single raw value from the HX711 ADC using `HX711_read()`,
 * converts it to an approximate weight by dividing by 981, and subtracts the offset.
 * It does not perform any filtering, so it is faster but may be noisier.
 *
 * @param hx711 Pointer to HX711 handle structure.
 */
void HX711_weight_read_fast(HX711* hx711);
