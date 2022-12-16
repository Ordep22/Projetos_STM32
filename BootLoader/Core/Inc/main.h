/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Bootloader.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define _V_MAIOR	0
#define _V_MENOR	0
#define _V_BUILD	0

#define SEPARADOR_MSG_DEBUG "-------------------------------------------------------------\r\n"
#define TEMPO_LED 2000



#define __PRINT_DEBUG_SERIAL__

#ifdef __PRINT_DEBUG_SERIAL__
//#define __PRINT_DEBUG_NAND__		// Importante: Não usar esse define de debug quando o FileSystem estiver em uso
//#define __PRINT_DEBUG_FS__
//#define __PRINT_DEBUG_FS_ERRO__
//#define __PRINT_DEBUG_ERRO__
#endif



typedef struct VERSAO
{
  uint8_t Major;
  uint8_t Minor;
  uint8_t Build;
} TVersao;


typedef struct PARAM_STATUS{

  uint8_t statusEquipamento;
  TVersao versaoFirmware;
  char assinatura[TAMANHO_ASSINATURA];
} TStatusParam;

typedef union STATUS_SISTEMA
{
  uint8_t dados[sizeof(TStatusParam)];
  TStatusParam parametros;
} TStatus;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_6
#define FLASH_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
