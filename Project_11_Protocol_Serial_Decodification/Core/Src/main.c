/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdlib.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define comand_high  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define comand_low HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
#define delay_00 HAL_Delay(750)
#define delay_01 HAL_Delay(450)
#define ESTAGIO_PREPROSSEAMENTO 0
#define ESTAGIO_01 1
#define ESTAGIO_02 2
#define ESTAGIO_03 3
#define ESTAGIO_04 4
#define ESTAGIO_05 5
#define CABECALHO 6
#define TAMANHO 7
#define COMANDO 8
#define MENSAGEM 9
#define CHECKSUM 10



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

volatile char V_Estagio; //Variável de cintrole do switch case
volatile char V_Estagio_Protocolo;

uint8_t ErroCabecalho[] = "Elemento de cabecalho incorreto\n";
uint8_t ErroCheckSum[] = "Check Sum incorreto\n";
uint8_t CorrectCheckSum[] = "Check Sum correto\n";
uint8_t MensageVazia[] = "-";



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */


int PiscaLed(int NumPiscadas , int TempoDelay)
{

	for(int i = 0; i < NumPiscadas; i++)
	{

		HAL_GPIO_WritePin(GPIOA,LED02_Pin,GPIO_PIN_SET);
		HAL_Delay(TempoDelay);
		HAL_GPIO_WritePin(GPIOA,LED02_Pin,GPIO_PIN_RESET);
		HAL_Delay(TempoDelay);

	}

	return 0;

}


int checkSum(uint8_t *Byte, int tamanho)
{
	//Adicionar uma vriável chamda tamanho para controlar até onde o for irá

	uint8_t dados= 0;

	uint8_t CS_Incorreto = 0;

	uint8_t CS_Correto = 1;



	for (int i = 0; i < 6; i++)
	{

			HAL_Delay(100);
			dados += Byte[i];
	}



	if (dados == Byte[7])
	{

		return CS_Correto;

	}

	else
	{

		return CS_Incorreto;

	}



}


void Protocolo(uint8_t *a,int Tamanho_string)
{
//Quan a função é declarada e logo abaixo é chamda não é necessário montar um projeto

	  fflush(stdin);

	  uint8_t Verificacao;

	  short int  contador = 0;

	  switch(V_Estagio_Protocolo)
	  {
	  	//Nesta estapa são verificados os elementos do cabecalho


		case(CHECKSUM):
		{

			Verificacao = checkSum(a, Tamanho_string);

			if (Verificacao == 1)
			{
				V_Estagio_Protocolo =  ESTAGIO_PREPROSSEAMENTO;

			}

			else
			{

				break;

			}


		}

		case ESTAGIO_PREPROSSEAMENTO:
		{

		  if((a[0] == 'L')){

			 V_Estagio_Protocolo  = ESTAGIO_01;
			 contador +=1;

		  }

		  if ((a[0] != 'L' ) && (a[0] != '\0') )
		  {


			  break;

		  }

		 }

		  case ESTAGIO_01:
		  {

			  if((a[1] == 'U'))
			  {

				  V_Estagio_Protocolo  = ESTAGIO_02;
				  contador +=1;

			  }

			  if ((a[1] != 'U') && (a[1] != '\0'))
			  {

				  break;
			  }
		  }
		  case ESTAGIO_02:
		  {
			  if((a[2] == 'P'))
			  {

				  V_Estagio_Protocolo  = ESTAGIO_03;
				  contador +=1;
			  }

			  if ((a[2] != 'P') && (a[2] != '\0'))

			  {

				  break;
			  }



		  }
		  case ESTAGIO_03:
		  {
			  if((a[3] == 'A'))

			  {
				  V_Estagio_Protocolo = COMANDO; //Provavelmente vai sair desse ponto
				  contador +=1;

			  }

			  if ((a[3] != 'A') && (a[3] != '\0'))

			  {

				  break;

			  }

		  }

		  case(COMANDO):
		  {

			  //Acende LED02
			  if (a[5] == 0)
			  {
				  V_Estagio_Protocolo =  CHECKSUM;
				  comand_high;
				  contador +=1;
			  }

			  //Apaga LED02
			  if (a[5] == 1)
			  {
				  V_Estagio_Protocolo =  CHECKSUM;
				  comand_low;
				  contador +=1;
			  }

			  //Pisca LED02 N vezes
			  if (a[5] == 2)
			  {

				  V_Estagio_Protocolo =  CHECKSUM;
				  PiscaLed(4, 500);
				  contador +=1;

			  }
		  }
	  }

  HAL_UART_Transmit(&huart2,a,12,100);

}






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

	int Tamanho_string;

	HAL_StatusTypeDef rx_status;

  	//V_Estagio  = CHECKSUM;
	V_Estagio_Protocolo = CHECKSUM;




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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  HAL_Delay(500);

	  uint8_t rx_dados[12] = {0};

	  HAL_Delay(500);

	  rx_status = HAL_UART_Receive_IT(&huart2,rx_dados,10);

	  Tamanho_string = sizeof(rx_dados);


	  if (rx_status !=  HAL_ERROR && rx_dados[0] == 'L')
	  {

		  Protocolo(rx_dados,Tamanho_string);

	  }

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED02_GPIO_Port, LED02_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED02_Pin */
  GPIO_InitStruct.Pin = LED02_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED02_GPIO_Port, &GPIO_InitStruct);

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
