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
#include "adc.h"
#include "dma.h"
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
#include "sensor.h"
#include "buzzer.h"
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
void system_init();
void test_LedDebug();
void test_Buzzer();
void Adc();
void timer_count();
void test_7seg();
void int2time(int n);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_FSMC_Init();
	MX_I2C1_Init();
	MX_TIM13_Init();
	MX_TIM2_Init();
	MX_ADC1_Init();
	/* USER CODE BEGIN 2 */
	system_init();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	lcd_Clear(BLACK);
	while (1) {
		while (!flag_timer2)
			;
		flag_timer2 = 0;
		button_Scan();
//		test_LedDebug();
		Adc();
//		test_Buzzer();
//		test_7seg();
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void system_init() {
	timer_init();
	button_init();
	lcd_init();
	sensor_init();
	buzzer_init();
	led7_init();
	setTimer2(50);
}

uint8_t count_led_debug = 0;
int counter = 0;

void test_LedDebug() {
	count_led_debug = (count_led_debug + 1) % 20;
	if (count_led_debug == 0) {
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

uint8_t isButtonUp() {
	if (button_count[3] == 1)
		return 1;
	else
		return 0;
}

uint8_t isButtonDown() {
	if (button_count[7] == 1)
		return 1;
	else
		return 0;
}

uint8_t isButtonRight() {
	if (button_count[11] == 1)
		return 1;
	else
		return 0;
}

uint8_t count_adc = 0;

void Adc() {
	count_adc = (count_adc + 1) % 20;
	if (count_adc == 0) {
		sensor_Read();
		lcd_ShowStr(10, 100, "Voltage:", RED, BLACK, 16, 0);
		lcd_ShowFloatNum(130, 100, sensor_GetVoltage(), 4, RED, BLACK, 16);
		lcd_ShowStr(170, 100, "(mV)", RED, BLACK, 16, 0);
//		lcd_ShowStr(10, 120, "Current:", RED, BLACK, 16, 0);
//		lcd_ShowFloatNum(130, 120, sensor_GetCurrent(), 4, RED, BLACK, 16);
		lcd_ShowStr(10, 120, "Light:", RED, BLACK, 16, 0);
		lcd_ShowIntNum(130, 120, sensor_GetLight(), 4, RED, BLACK, 16);
		if (sensor_GetLight() < 2048)
			lcd_ShowStr(170, 120, "Weak", RED, BLACK, 16, 0);
		else
			lcd_ShowStr(170, 120, "Strong", RED, BLACK, 16, 0);
//		lcd_ShowStr(10, 160, "Potentiometer:", RED, BLACK, 16, 0);
//		lcd_ShowIntNum(130, 160, sensor_GetPotentiometer(), 4, RED, BLACK, 16);
		lcd_ShowStr(10, 140, "Temperature:", RED, BLACK, 16, 0);
		lcd_ShowFloatNum(130, 140, sensor_GetTemperature(), 4, RED, BLACK, 16);
		lcd_ShowStr(170, 140, "(oC)", RED, BLACK, 16, 0);
		lcd_ShowStr(10, 160, "Humidity:", RED, BLACK, 16, 0);
		float temp = sensor_GetPotentiometer() * 0.024;
		lcd_ShowFloatNum(130, 160, temp, 4, RED, BLACK, 16);
		lcd_ShowStr(170, 160, "%", RED, BLACK, 16, 0);

		if (temp > 70) {
			buzzer_SetVolume(50);
			lcd_StrCenter(0, 200, "Alert!", RED, BLACK, 32, 0);
		} else {
			lcd_StrCenter(0, 200, "Alert!", BLACK, BLACK, 32, 0);
			buzzer_SetVolume(0);
		}
		counter = counter + 1;
		int2time(counter);
	}
}


void test_Buzzer() {
	if (isButtonUp()) {
		buzzer_SetVolume(50);
	}

	if (isButtonDown()) {
		buzzer_SetVolume(0);
	}

	if (isButtonRight()) {
		buzzer_SetVolume(25);
	}

}

void test_7seg() {
	//write number1 at led index 0 (not show dot)
	led7_SetDigit(1, 0, 0);
	led7_SetDigit(5, 1, 0);
	led7_SetDigit(4, 2, 0);
	led7_SetDigit(7, 3, 0);
}

void int2time(int n) {
	led7_SetDigit(n / 1000, 0, 0);
	n = n % 1000;
	led7_SetDigit(n / 100, 1, 0);
	n = n % 100;
	led7_SetDigit(n / 10, 2, 0);
	n = n % 10;
	led7_SetDigit(n, 3, 0);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
