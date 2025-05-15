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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sgp30.h"
#include "easy_rgb_lcd.h"
#include "buzzer.h"
#include "sen54.h"
#include "sen5x_i2c.h"
#include "string.h"
#include "stdio.h"
#include "buzzer_music.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //start communication with sensor

  HAL_Delay(10);
  //Display initialization
  LCD_begin(16,2,0);
  LCD_setColorWhite();
  //first I2C initialization
  sgp30_iaq_init();
  HAL_Delay(1000);
  uint8_t sensor_data[60];  // Buffer large enough for full data

   uint8_t buffer[48];
   sen5x_get_product_name(buffer, 48);
   int16_t error = sen5x_device_reset();
   error = sen5x_start_measurement();

   uint16_t mass_concentration_pm1p0;
   uint16_t mass_concentration_pm2p5;
   uint16_t mass_concentration_pm4p0;
   uint16_t mass_concentration_pm10p0;
   int16_t ambient_humidity;
   int16_t ambient_temperature;
   int16_t voc_index;
   int16_t nox_index;
   uint8_t cur_screen = 0;
   uint32_t last_switch = 0;



   HAL_Delay(1000);

   error = sen5x_read_measured_values(
       &mass_concentration_pm1p0, &mass_concentration_pm2p5,
       &mass_concentration_pm4p0, &mass_concentration_pm10p0,
       &ambient_humidity, &ambient_temperature, &voc_index, &nox_index);

   if (sen54_init(&hi2c1) == HAL_OK)
   {
       printf("Sensor initialized.\r\n");
   } else {
       printf("Sensor init failed.\r\n");
   }


   if (sen54_read_measurements(&hi2c1, sensor_data, sizeof(sensor_data)) == HAL_OK) {
       /*printf("Measurement received:\r\n");*/

       for (int i = 0; i < sizeof(sensor_data); i++) {
           printf("%02X ", sensor_data[i]);
       }
       printf("\r\n");
   }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //measurement of values
	  uint16_t co2, tvoc, pm25;
	  float temp = 0.0f, hum= 0.0f;

	  char buffer[17];



	  //reading VOC and CO2 values
	  sgp30_measure_iaq_blocking_read(&tvoc, &co2);

	  /*
	  //reading SEn54
	  if (sen54_read_measurements(&hi2c1, sensor_data, sizeof(sensor_data)) == HAL_OK) {
	        printf("Measurement received:\r\n");
	        for (int i = 0; i < sizeof(sensor_data); i++) {
	            printf("%02X ", sensor_data[i]);
	        }
	        printf("\r\n");
	  }
*/
	  if (sen54_read_measurements(&hi2c1, sensor_data, sizeof(sensor_data)) == HAL_OK) {

	  	      printf("\r\n");
	  	      uint16_t raw_mc_1 = ((uint16_t)sensor_data[0] << 8) | sensor_data[1];
	  	      mass_concentration_pm1p0 = raw_mc_1/10.0f;
	  	      int mc_1_int = (int)(mass_concentration_pm1p0 + 0.5f);
	  	      printf("\r\nMass Concentration PM1.0: %.d °C\r\n", mc_1_int);

	  	      printf("\r\n");
	  	      uint16_t raw_mc_25 = ((uint16_t)sensor_data[3] << 8) | sensor_data[4];
	  	      mass_concentration_pm2p5 = raw_mc_25/10.0f;
	  	      int mc_25_int = (int)(mass_concentration_pm2p5 + 0.5f);
	  	      printf("\r\nMass Concentration PM2.5: %.d °C\r\n", mc_25_int);

	  	      printf("\r\n");
	  	      uint16_t raw_mc_4 = ((uint16_t)sensor_data[6] << 8) | sensor_data[7];
	  	      mass_concentration_pm4p0 = raw_mc_4/10.0f;
	  	      int mc_4_int = (int)(mass_concentration_pm4p0 + 0.5f);
	  	      printf("\r\nMass Concentration PM4.0: %.d °C\r\n", mc_4_int);

	  	      printf("\r\n");
	  	      uint16_t raw_mc_10 = ((uint16_t)sensor_data[9] << 8) | sensor_data[10];
	  	      mass_concentration_pm10p0 = raw_mc_10/10.0f;
	  	      int mc_10_int = (int)(mass_concentration_pm10p0 + 0.5f);
	  	      printf("\r\nMass Concentration PM10.0: %.d °C\r\n", mc_10_int);

	  	      printf("\r\n");
	  	      uint16_t raw_hum = ((uint16_t)sensor_data[12] << 8) | sensor_data[13];
	  	      ambient_humidity = raw_hum/100.0f;
	  	      int hum_int = (int)(ambient_humidity + 0.5f);
	  	      printf("\r\nAmbient Humidity: %.d %\r\n", hum_int);

	  	      uint16_t raw_temp = ((uint16_t)sensor_data[15] << 8) | sensor_data[16];
	  	      float room_temperature = raw_temp / 200.0f;
	  	      int temp_int = (int)(room_temperature + 0.5f);
	  	      printf("\r\nAmbient Temperature: %.d °C\r\n", temp_int);


	  	  }


	  HAL_Delay(1000); //read every second

	  //cycling through the data on screen
	  if (HAL_GetTick() - last_switch >= 3000) {
	          last_switch = HAL_GetTick();
	          cur_screen = (cur_screen + 1) % 3;

	          //clear and update LCD
	          LCD_clear();
			  switch(cur_screen){
				  case 0:
					  LCD_setCursor(0, 0);
					  snprintf(buffer, sizeof(buffer), "CO2: %u ppm", co2);
					  LCD_print(buffer, strlen(buffer));

					  LCD_setCursor(0, 1);
					  snprintf(buffer, sizeof(buffer), "TVOC: %u ppb", tvoc);
					  LCD_print(buffer, strlen(buffer));
					  break;
				  case 1:
					  LCD_setCursor(0, 0);
					  uint16_t raw_temp = ((uint16_t)sensor_data[15] << 8) | sensor_data[16];
					  float room_temperature = raw_temp / 200.0f;
					  int temp_int = (int)(room_temperature + 0.5f);
					  snprintf(buffer, sizeof(buffer), "T: %d C", temp_int);
					  LCD_print(buffer, strlen(buffer));

					  LCD_setCursor(0, 1);
					  //int hum_int = (int)(hum + 0.5f);
					  uint16_t raw_hum = ((uint16_t)sensor_data[12] << 8) | sensor_data[13];
					  ambient_humidity = raw_hum/100.0f;
					  int hum_int = (int)(ambient_humidity + 0.5f);
					  snprintf(buffer, sizeof(buffer), "RH: %d %%", hum_int);
					  LCD_print(buffer, strlen(buffer));
					  break;
				  case 2:
					  LCD_setCursor(0, 0);
					  uint16_t raw_mc_25 = ((uint16_t)sensor_data[3] << 8) | sensor_data[4];
					  mass_concentration_pm2p5 = raw_mc_25/10.0f;
					  int mc_25_int = (int)(mass_concentration_pm2p5 + 0.5f);
					  snprintf(buffer, sizeof(buffer), "PM2.5: %u ug/m3", mc_25_int);
					  LCD_print(buffer, strlen(buffer));
					  break;
			  }


	  }

	  uint16_t raw_temp = ((uint16_t)sensor_data[15] << 8) | sensor_data[16];
	 					  float room_temperature = raw_temp / 200.0f;
	 					  int temp_int = (int)(room_temperature + 0.5f);
	  uint16_t raw_hum = ((uint16_t)sensor_data[12] << 8) | sensor_data[13];
	  ambient_humidity = raw_hum/100.0f;
	  int hum_int = (int)(ambient_humidity + 0.5f);
	  uint16_t raw_mc_25 = ((uint16_t)sensor_data[3] << 8) | sensor_data[4];
	  mass_concentration_pm2p5 = raw_mc_25/10.0f;
	  int mc_25_int = (int)(mass_concentration_pm2p5 + 0.5f);

	  //Check Lüften conditions
	  if (co2>1000|| tvoc>300||mass_concentration_pm2p5>35 || room_temperature>30.0f || ambient_humidity > 60.0f){

		 //Twinkle Twinkle little star

		  int length = 15;
		  char notes[] = "ccggaagffeeddc ";
		  int beats[]  = {1,1,1,1,1,1,2,1,1,1,1,1,1,2,4};
		  int tempo = 300;
		  char buffer[17];
/*
		    == for TOM THE TANK ENGINE
		    	  int length = 8;
		    	  char notes[] = "gahCDEG "
		    	  int beats[] = {1,1,1,2,1,1,4};
		    	  int tempo = 300;
	*/


		for (int i = 0; i < length; i++) {
		  LCD_clear();
		  LCD_setCursor(0, 0);
		  snprintf(buffer, sizeof(buffer), "LUEFTEN!");
		  LCD_print(buffer, strlen(buffer));
		  sgp30_measure_iaq_blocking_read(&tvoc, &co2);

		  LCD_setCursor(0, 1);
		  if (co2 > 1000) {
		          snprintf(buffer, sizeof(buffer), "CO2: %u ppm", co2);
		      }
		  else if (tvoc > 300) {
		          snprintf(buffer, sizeof(buffer), "TVOC: %u ppb", tvoc);
		      }
		  else if (room_temperature > 50.0f || room_temperature < 18.0f) {
			  uint16_t raw_temp = ((uint16_t)sensor_data[15] << 8) | sensor_data[16];
			  float room_temperature = raw_temp / 200.0f;
			  int temp_int = (int)(room_temperature + 0.5f);
			  snprintf(buffer, sizeof(buffer), "T: %d C", temp_int);
		      }
		  else if (ambient_humidity  > 60.0f) {
			  uint16_t raw_hum = ((uint16_t)sensor_data[12] << 8) | sensor_data[13];
			  ambient_humidity = raw_hum/100.0f;
			  int hum_int = (int)(ambient_humidity + 0.5f);
			  snprintf(buffer, sizeof(buffer), "RH: %d %%", hum_int);
			  LCD_print(buffer, strlen(buffer));
		  		      }
		  else if (mass_concentration_pm2p5 > 35) {
			  uint16_t raw_mc_25 = ((uint16_t)sensor_data[3] << 8) | sensor_data[4];
			  mass_concentration_pm2p5 = raw_mc_25/10.0f;
			  int mc_25_int = (int)(mass_concentration_pm2p5 + 0.5f);
			  snprintf(buffer, sizeof(buffer), "PM2.5: %u ug/m3", mc_25_int);
		  }
		  LCD_print(buffer, strlen(buffer));

		  if (notes[i] == ' ') {
		  					HAL_Delay(beats[i] * tempo);
		  				}
		  else {
		  					play_note(notes[i], beats[i] * tempo);
		  				}
		  				HAL_Delay(tempo / 2);
		}
		  HAL_Delay(3000);
		  LCD_clear();
		  HAL_Delay(3000);
	  }


	  //printf("CO2: %d ppm, TVOC: %d ppb\r\n", co2, tvoc);
	  int16_t ret = sgp30_measure_iaq_blocking_read(&tvoc, &co2);
	  if (ret != 0) {
	      printf("SGP30 I2C read error: %d\r\n", ret);
	  }


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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10D19CE4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Buzzer_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
