/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>


#include <stddef.h>
#include "Gen_libary.h"
#include "HX711_pro.h"
#include "HD44780.h"
#include "HD44780_menu.h"
#include "Joystick.h"
#include "Joystick_menu.h"
#include "HD44780_callbacks.h"
#include "stm32f1xx_it.h"
#include "Keypad.h"


#define NULL 0

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
  if (ch == '\n') {
    __io_putchar('\r');
  }
  HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
  return 1;
}



menu_t menu_1 = {"MEASURE", &menu_2, &menu_3 , &sub_menu_1_1, NULL, NULL};
	menu_t sub_menu_1_1 = {"Realtime", &sub_menu_1_2, &sub_menu_1_2 , NULL, &menu_1, disp_weight_callback};
	menu_t sub_menu_1_2 = {"Probes", NULL, &sub_menu_1_1 , NULL, &menu_1, disp_weight_p_callback};

menu_t menu_2 = {"DISPLAY", &menu_3, &menu_1, &sub_menu_2_1, NULL, NULL};
	menu_t sub_menu_2_1 = {"Brightness", NULL, &sub_menu_2_1, NULL, &menu_2, disp_brightness_callback};

menu_t menu_3 = {"SET OFFSET", NULL, &menu_2, &sub_menu_3_1, NULL, NULL};
	menu_t sub_menu_3_1 = {"Set custom", NULL, &sub_menu_3_1, NULL, &menu_3, disp_set_custom_callback};



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  char tmp[] = {
  	    '1','2','3',
  	    '4','5','6',
  	    '7','8','9',
  	    '*','0','#'
  	};


  HX711 hx711_weight;
  HD44780 hd44780_lcd1602;
  Joystick J;
  Keypad K;

  pin_t K_r_pins[] = {
      {KEYPAD_R1_GPIO_Port, KEYPAD_R1_Pin},
      {KEYPAD_R2_GPIO_Port, KEYPAD_R2_Pin},
      {KEYPAD_R3_GPIO_Port, KEYPAD_R3_Pin},
      {KEYPAD_R4_GPIO_Port, KEYPAD_R4_Pin}
  };


  pin_t K_c_pins[] = {
      {KEYPAD_C1_GPIO_Port, KEYPAD_C1_Pin},
      {KEYPAD_C2_GPIO_Port, KEYPAD_C2_Pin},
      {KEYPAD_C3_GPIO_Port, KEYPAD_C3_Pin}
  };

  HAL_TIM_Base_Start(&htim2);
  Gen_libary_Timer_init(&htim1);

  Keypad_init(&K, K_r_pins, K_c_pins, 4, 3, tmp);
  HX711_default_init(&hx711_weight, HX711_CLK_GPIO_Port, HX711_CLK_Pin, HX711_DATA_GPIO_Port, HX711_DATA_Pin);
  HX711_set_gain(&hx711_weight, 128);


  HD44780_init(&hd44780_lcd1602, LCD1602_RS_GPIO_Port, LCD1602_RS_Pin, LCD1602_RW_GPIO_Port, LCD1602_RW_Pin,
		  LCD1602_E_GPIO_Port, LCD1602_E_Pin, LCD1602_D4_GPIO_Port,  LCD1602_D4_Pin, LCD1602_D5_GPIO_Port,
		  LCD1602_D5_Pin, LCD1602_D6_GPIO_Port, LCD1602_D6_Pin, LCD1602_D7_GPIO_Port, LCD1602_D7_Pin);
  HD44780_init_brightness(&hd44780_lcd1602, &htim2, &TIM2->CCR1, &TIM2->ARR, TIM_CHANNEL_1);
  HD44780_init_keypad(&hd44780_lcd1602, &K);
  HD44780_init_menu(&hd44780_lcd1602, &menu_1);
  HD44780_init_weight(&hd44780_lcd1602, &hx711_weight);
  JoystickInit(&J, J_UP_GPIO_Port, J_UP_Pin,
		  J_RIGHT_GPIO_Port, J_RIGHT_Pin,
		  J_DOWN_GPIO_Port, J_DOWN_Pin, J_LEFT_GPIO_Port,
		  J_LEFT_Pin, J_CLICK_GPIO_Port, J_CLICK_Pin);



   key_next_fun = HD44780_next_m;
   key_prev_fun = HD44780_prev_m;
   key_enter_fun = HD44780_enter_m;
   key_back_fun = HD44780_get_back_m;
   key_on_fun = HD44780_on_m;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  disp_menu_fun_callbacks_state_machine(&hd44780_lcd1602);

	 HX711_weight_read_fast(&hx711_weight);

	 pool_joystick(&J);
	 Keypad_get_pressed(&K);
	 key_next_pressed(&hd44780_lcd1602, &J);
	 key_prev_pressed(&hd44780_lcd1602, &J);
	 key_enter_pressed(&hd44780_lcd1602, &J);
	 key_back_pressed(&hd44780_lcd1602, &J);
	 key_on_pressed(&hd44780_lcd1602, &J);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
