/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdio.h>
#include <math.h>
#include "mag3110.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MOTION_THRESHOLD 3.5//2.0f   // µT, 2.0 było za mało
#define DIRECTION_THRESHOLD 3.5f
#define STILL_LIMIT 30          // ile próbek bez ruchu kończy kalibrację

#define ALPHA 0.2f
#define THRESHOLD 1.8f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
MAG3110_t mag;

#define N 10

float x,y,z;

float xmin =  10000;
float xmax = -10000;

float ymin =  10000;
float ymax = -10000;

float zmin =  10000;
float zmax = -10000;

float x_offset = 0;
float y_offset = 0;
float z_offset = 0;

float x_prev = 0;
float y_prev = 0;
float z_prev = 0;

float x_prev_corr = 0;
float y_prev_corr = 0;
float z_prev_corr = 0;

float B_buf[N];
int idx=0;

uint8_t calibration_done = 0;
uint32_t still_counter = 0;
uint8_t sensor_error = 0;

int MovementCounter = 0;

float x_filt = 0, y_filt = 0, z_filt = 0;
float x_prev_filt = 0, y_prev_filt = 0, z_prev_filt = 0;

static int motion_counter = 0;
uint32_t timer_start = 0;

uint8_t movement_active = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return 1;
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
  MX_USART2_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  MAG3110_Init(&mag, &hi2c1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  for(uint8_t addr=1; addr<128; addr++)
  {
      if(HAL_I2C_IsDeviceReady(&hi2c1, addr<<1, 2, 10) == HAL_OK)
    	  printf("Sensor found. Address: 0x%02X\r\n", addr);

  }

  if(MAG3110_Init(&mag, &hi2c1) != HAL_OK)
    {
  	  sensor_error = 1;
    }

  while (1)
  {
	  if(sensor_error == 1){
		  printf("MAG3110 init ERROR\r\n");
		  HAL_GPIO_TogglePin(LR_GPIO_Port, LR_Pin);
		  HAL_Delay(1000);
	  }else{
		  	  MAG3110_Read_uT(&mag, &x,&y,&z);
	//Dla offsetu
		  	float dx_raw = fabsf(x - x_prev);
			float dy_raw = fabsf(y - y_prev);
			float dz_raw = fabsf(z - z_prev);

			uint8_t motion_detected = (dx_raw > MOTION_THRESHOLD) ||
									  (dy_raw > MOTION_THRESHOLD) ||
									  (dz_raw > MOTION_THRESHOLD);
		  if(!calibration_done)
		  {
			  printf("Calibration in progress\r\n");
			  HAL_GPIO_WritePin(LR_GPIO_Port, LR_Pin, GPIO_PIN_SET);
			  /* aktualizacja min/max */
			  if(x < xmin) xmin = x;
			  if(x > xmax) xmax = x;

			  if(y < ymin) ymin = y;
			  if(y > ymax) ymax = y;

			  if(z < zmin) zmin = z;
			  if(z > zmax) zmax = z;

			  /* wykrywanie braku ruchu */
			  if(motion_detected)
				  still_counter = 0;
			  else
				  still_counter++;

			  /* zakończenie kalibracji */
			  if(still_counter > STILL_LIMIT)
			  {
				  x_offset = (xmax + xmin)/2.0f;
				  y_offset = (ymax + ymin)/2.0f;
				  z_offset = (zmax + zmin)/2.0f;

				  calibration_done = 1;

				  timer_start = HAL_GetTick();

				  HAL_GPIO_WritePin(LR_GPIO_Port, LR_Pin, GPIO_PIN_RESET);
				  printf("Calibration DONE\r\n");
				  printf("Offsets: x_offset=%.2f y_offset=%.2f z_offset=%.2f\r\n", x_offset,y_offset,z_offset);
				  HAL_GPIO_WritePin(LG_GPIO_Port, LG_Pin, GPIO_PIN_SET);
			  }
		  }else
			 {
			  //Bez filtra
				 float x_corr = x - x_offset;
				 float y_corr = y - y_offset;
				 float z_corr = z - z_offset;

//				 printf("Corrected values: x=%.2f y=%.2f z=%.2f\r\n", x_corr,y_corr,z_corr);

				  // FILTR IIR
				  x_filt = ALPHA * x_corr + (1 - ALPHA) * x_prev_filt;
				  y_filt = ALPHA * y_corr + (1 - ALPHA) * y_prev_filt;
				  z_filt = ALPHA * z_corr + (1 - ALPHA) * z_prev_filt;

				 /* zmiana pola magnetycznego */
				 // FILTR IIR
				 float dx = x_filt - x_prev_filt;
				 float dy = y_filt - y_prev_filt;
				 float dz = z_filt - z_prev_filt;

				 float dB = sqrtf(dx*dx + dy*dy + dz*dz);

				 uint32_t current_time = HAL_GetTick() - timer_start;
				 printf("Field values: x=%.2f, y=%.2f, z=%.2f, time=%lu ms, dB=%.2f, movement counter: %d\r\n", x_filt,y_filt,z_filt,current_time,dB,MovementCounter);

				 /* wykrywanie ruchu */
				 if(dB > THRESHOLD){
					 motion_counter++;
				 }else{
					 motion_counter = 0;
				 }

				 if(motion_counter >= 3)
				 {

					 float adx = fabsf(dx);
					 float ady = fabsf(dy);
					 float adz = fabsf(dz);

					 /* kierunek ruchu */

					 if(adx > ady && adx > adz)
					 {
						 if(dx > 0){
							 //printf("Direction: +X\r\n");
							 //printf("Field change value: %.2f \n",dx);
							 MovementCounter++;
							 HAL_GPIO_WritePin(LY1_GPIO_Port, LY1_Pin, GPIO_PIN_SET);
						 }else{
							 //printf("Direction: -X\r\n");
							 //printf("Field change value: %.2f\r\n",dx);
							 MovementCounter++;
							 HAL_GPIO_WritePin(LY2_GPIO_Port, LY2_Pin, GPIO_PIN_SET);
						 }

					 }else if(ady > adx && ady > adz)
					 {
						 if(dy > 0){
							 //printf("Direction: +Y\r\n");
							 //printf("Field change value: %.2f\r\n",dy);
							 MovementCounter++;
							 HAL_GPIO_WritePin(LY1_GPIO_Port, LY1_Pin, GPIO_PIN_SET);
						 }else{
							 //printf("Direction: -Y\r\n");
							 //printf("Field change value: %.2f\r\n",dy);
							 MovementCounter++;
							 HAL_GPIO_WritePin(LY2_GPIO_Port, LY2_Pin, GPIO_PIN_SET);
						 }

					 }else{
						 if(dz > 0){
							 //printf("Direction: +Z\r\n");
							 //printf("Field change value: %.2f\r\n",dz);
							 MovementCounter++;
							 HAL_GPIO_WritePin(LY1_GPIO_Port, LY1_Pin, GPIO_PIN_SET);
						 }else{
							 //printf("Direction: -Z\r\n");
							 //printf("Field change value: %.2f\r\n",dz);
							 MovementCounter++;
							 HAL_GPIO_WritePin(LY2_GPIO_Port, LY2_Pin, GPIO_PIN_SET);
						 }
					 }
				 }
				 else{
					 HAL_GPIO_WritePin(LY1_GPIO_Port, LY1_Pin, GPIO_PIN_RESET);
					 HAL_GPIO_WritePin(LY2_GPIO_Port, LY2_Pin, GPIO_PIN_RESET);
				 }

				 x_prev_filt = x_filt;
				 y_prev_filt = y_filt;
				 z_prev_filt = z_filt;
			 }

			 x_prev = x;
			 y_prev = y;
			 z_prev = z;

			 /* Diody przygotowane do obsługi katalogu ruchów */
	//	     HAL_GPIO_WritePin(Palec1Skorcz_GPIO_Port, Palec1Skorcz_Pin, GPIO_PIN_SET);
	//		 HAL_GPIO_WritePin(Palec1Roskorcz_GPIO_Port, Palec1Roskorcz_Pin, GPIO_PIN_RESET);
	//		 HAL_GPIO_WritePin(Palec2Skorcz_GPIO_Port, Palec2Skorcz_Pin, GPIO_PIN_SET);
	//		 HAL_GPIO_WritePin(Palec2Roskorcz_GPIO_Port, Palec2Roskorcz_Pin, GPIO_PIN_RESET);
	//		 HAL_GPIO_WritePin(Palec3Skorcz_GPIO_Port, Palec3Skorcz_Pin, GPIO_PIN_SET);
	//		 HAL_GPIO_WritePin(Palec3Roskorcz_GPIO_Port, Palec3Roskorcz_Pin, GPIO_PIN_RESET);
	//		 HAL_GPIO_WritePin(Palec4Skorcz_GPIO_Port, Palec4Skorcz_Pin, GPIO_PIN_SET);
	//		 HAL_GPIO_WritePin(Palec4Roskorcz_GPIO_Port, Palec4Roskorcz_Pin, GPIO_PIN_RESET);
	//		 HAL_GPIO_WritePin(Palec5Skorcz_GPIO_Port, Palec5Skorcz_Pin, GPIO_PIN_SET);
	//		 HAL_GPIO_WritePin(Palec5Roskorcz_GPIO_Port, Palec5Roskorcz_Pin, GPIO_PIN_RESET);
	//		 HAL_Delay(1000);
	//
	//	     HAL_GPIO_WritePin(Palec1Skorcz_GPIO_Port, Palec1Skorcz_Pin, GPIO_PIN_RESET);
	//	     HAL_GPIO_WritePin(Palec1Roskorcz_GPIO_Port, Palec1Roskorcz_Pin, GPIO_PIN_SET);
	//	     HAL_GPIO_WritePin(Palec2Skorcz_GPIO_Port, Palec2Skorcz_Pin, GPIO_PIN_RESET);
	//	     HAL_GPIO_WritePin(Palec2Roskorcz_GPIO_Port, Palec2Roskorcz_Pin, GPIO_PIN_SET);
	//	     HAL_GPIO_WritePin(Palec3Skorcz_GPIO_Port, Palec3Skorcz_Pin, GPIO_PIN_RESET);
	//	     HAL_GPIO_WritePin(Palec3Roskorcz_GPIO_Port, Palec3Roskorcz_Pin, GPIO_PIN_SET);
	//	     HAL_GPIO_WritePin(Palec4Skorcz_GPIO_Port, Palec4Skorcz_Pin, GPIO_PIN_RESET);
	//	     HAL_GPIO_WritePin(Palec4Roskorcz_GPIO_Port, Palec4Roskorcz_Pin, GPIO_PIN_SET);
	//	     HAL_GPIO_WritePin(Palec5Skorcz_GPIO_Port, Palec5Skorcz_Pin, GPIO_PIN_RESET);
	//	     HAL_GPIO_WritePin(Palec5Roskorcz_GPIO_Port, Palec5Roskorcz_Pin, GPIO_PIN_SET);
	//
	//	     HAL_Delay(1000);
	//
			 HAL_Delay(50);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LY2_Pin|LY1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LG_Pin|LR_Pin|Palec1Roskorcz_Pin|Palec1Skorcz_Pin
                          |Palec2Roskorcz_Pin|Palec3Skorcz_Pin|Palec3Roskorcz_Pin|Palec4Skorcz_Pin
                          |Palec4Roskorcz_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Palec2Skorcz_Pin|Palec5Skorcz_Pin|Palec5Roskorcz_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LY2_Pin LY1_Pin */
  GPIO_InitStruct.Pin = LY2_Pin|LY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LG_Pin LR_Pin Palec1Roskorcz_Pin Palec1Skorcz_Pin
                           Palec2Roskorcz_Pin Palec3Skorcz_Pin Palec3Roskorcz_Pin Palec4Skorcz_Pin
                           Palec4Roskorcz_Pin */
  GPIO_InitStruct.Pin = LG_Pin|LR_Pin|Palec1Roskorcz_Pin|Palec1Skorcz_Pin
                          |Palec2Roskorcz_Pin|Palec3Skorcz_Pin|Palec3Roskorcz_Pin|Palec4Skorcz_Pin
                          |Palec4Roskorcz_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Palec2Skorcz_Pin Palec5Skorcz_Pin Palec5Roskorcz_Pin */
  GPIO_InitStruct.Pin = Palec2Skorcz_Pin|Palec5Skorcz_Pin|Palec5Roskorcz_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
