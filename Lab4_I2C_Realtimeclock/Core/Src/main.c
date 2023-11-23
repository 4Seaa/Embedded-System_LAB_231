/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
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
uint8_t count_led_debug = 0;
// 0: time mode; 1: edit mode; 2: alarm mode
uint8_t count_sys_mode = 0;
// 0: hr; 1: min; 2: sec
// 3: day; 4: date; 5: month; 6: year
uint8_t count_edit_var = 0;

uint8_t edit_temp_hour = 0;
uint8_t edit_temp_min = 0;
uint8_t edit_temp_sec = 0;
uint8_t edit_temp_day = 2;
uint8_t edit_temp_date = 1;
uint8_t edit_temp_month = 1;
uint8_t edit_temp_year = 0;

uint8_t alarm_hour = 0;
uint8_t alarm_min = 0;
uint8_t alarm_sec = 0;
uint8_t alarm_day = 2;
uint8_t alarm_date = 1;
uint8_t alarm_month = 1;
uint8_t alarm_year = 0;
//
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init();
void test_LedDebug();
void displayTime();
void updateTime();

void time_mode();
void edit_mode();
void alarm_mode();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lcd_Clear(BLACK);
  updateTime();
  while (1)
  {
    while (!flag_timer2)
      ;
    flag_timer2 = 0;
    button_Scan();
    switch (count_sys_mode)
    {
    case 0: // Mode 1: Time Mode
      time_mode();
      break;
    case 1: // Mode 2: Edit Mode
      edit_mode();
      break;
    case 2: // Mode 3: Alarm Mode
      alarm_mode();
      break;
    default: // Handle unexpected mode
      break;
    }

    // ds3231_ReadTime();
    // displayTime();

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

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void system_init()
{
  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
  timer_init();
  led7_init();
  button_init();
  lcd_init();
  ds3231_init();
  setTimer2(50);
}

void test_LedDebug()
{
  count_led_debug = (count_led_debug + 1) % 20;
  if (count_led_debug == 0)
  {
    HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
  }
}

void test_7seg()
{
  led7_SetDigit(0, 0, 0);
  led7_SetDigit(5, 1, 0);
  led7_SetDigit(4, 2, 0);
  led7_SetDigit(7, 3, 0);
}
void test_button()
{
  for (int i = 0; i < 16; i++)
  {
    if (button_count[i] == 1)
    {
      led7_SetDigit(i / 10, 2, 0);
      led7_SetDigit(i % 10, 3, 0);
    }
  }
}

void updateTime()
{
  ds3231_Write(ADDRESS_YEAR, 23);
  ds3231_Write(ADDRESS_MONTH, 10);
  ds3231_Write(ADDRESS_DATE, 20);
  ds3231_Write(ADDRESS_DAY, 6);
  ds3231_Write(ADDRESS_HOUR, 20);
  ds3231_Write(ADDRESS_MIN, 11);
  ds3231_Write(ADDRESS_SEC, 23);
}

uint8_t isButtonUp()
{
  if (button_count[3] == 1)
    return 1;
  else
    return 0;
}
uint8_t isButtonDown()
{
  if (button_count[7] == 1)
    return 1;
  else
    return 0;
}
void displayTime()
{
  lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
  lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
  lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
  lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
  lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
  lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
  lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void check_button_change_mode()
{
  // Check button change mode
  if (button_count[0] == 1)
  {
    count_sys_mode = (count_sys_mode == 2) ? 0 : count_sys_mode + 1;
  }
}

void check_button_change_edit_var()
{
  // Check button change edit var
  if (button_count[1] == 1)
  {
    count_edit_var = (count_edit_var == 6) ? 0 : count_edit_var + 1;
  }
}

void check_button_save_edit()
{
  // Check button save edit
  if (button_count[2] == 1)
  {
    ds3231_hours = edit_temp_hour;
    ds3231_min = edit_temp_min;
    ds3231_sec = edit_temp_sec;
    ds3231_day = edit_temp_day;
    ds3231_date = edit_temp_date;
    ds3231_month = edit_temp_month;
    ds3231_year = edit_temp_year;
  }
}

void check_button_increase_edit_var(uint8_t *edit_var_value, uint8_t edit_var_index)
{
  // Check button change edit var
  if (button_count[3] == 1)
  {
    switch (edit_var_index)
    {
    case 0: // Var 1: Hour
      (*edit_var_value) = ((*edit_var_value) == 23) ? 0 : (*edit_var_value) + 1;
      break;
    case 1: // Var 2: Min
      (*edit_var_value) = ((*edit_var_value) == 59) ? 0 : (*edit_var_value) + 1;
      break;
    case 2: // Var 3: Sec
      (*edit_var_value) = ((*edit_var_value) == 59) ? 0 : (*edit_var_value) + 1;
      break;
    case 3: // Var 4: Day
      (*edit_var_value) = ((*edit_var_value) == 8) ? 2 : (*edit_var_value) + 1;
      break;
    case 4: // Var 5: Date
      (*edit_var_value) = ((*edit_var_value) == 31) ? 1 : (*edit_var_value) + 1;
      break;
    case 5: // Var 6: Month
      (*edit_var_value) = ((*edit_var_value) == 12) ? 1 : (*edit_var_value) + 1;
      break;
    case 6: // Var 7: Year
      (*edit_var_value) = ((*edit_var_value) == 99) ? 0 : (*edit_var_value) + 1;
      break;
    default: // Handle unexpected mode
      break;
    }
  }
}

void time_mode()
{
  // Read current time
  ds3231_ReadTime();
  // Display current time on LCD
  lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
  lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
  lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
  lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
  lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
  lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
  lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
  // Check button change mode
  check_button_change_mode();
  // Check if alarm condition true then execute
  check_execute_alarm();
}

void edit_mode()
{
  int *edit_vars[] = {&edit_temp_hour, &edit_temp_min, &edit_temp_sec,
                      &edit_temp_day, &edit_temp_date, &edit_temp_month, &edit_temp_year};

  check_button_increase_edit_var(edit_vars[count_edit_var], count_edit_var);
  check_button_change_mode();
  check_button_change_edit_var();
  check_button_save_edit();
}

void check_button_save_alarm()
{
  // Check button save edit
  if (button_count[2] == 1)
  {
    alarm_hour = edit_temp_hour;
    alarm_min = edit_temp_min;
    alarm_sec = edit_temp_sec;
    alarm_day = edit_temp_day;
    alarm_date = edit_temp_date;
    alarm_month = edit_temp_month;
    alarm_year = edit_temp_year;
  }
}

void alarm_mode()
{
  int *edit_vars[] = {&edit_temp_hour, &edit_temp_min, &edit_temp_sec,
                      &edit_temp_day, &edit_temp_date, &edit_temp_month, &edit_temp_year};

  check_button_increase_edit_var(edit_vars[count_edit_var], count_edit_var);
  check_button_change_mode();
  check_button_change_edit_var();
  check_button_save_alarm();
}

void check_execute_alarm()
{
  if ((ds3231_hours == alarm_hour) && (ds3231_min == alarm_min) && (ds3231_sec == alarm_sec) && (ds3231_day == alarm_day) && (ds3231_date == alarm_date) && (ds3231_month == alarm_month) && (ds3231_year == alarm_year))
  {
    lcd_Clear(RED);
  }
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
