/*
 * Dados.h
 *
 *  Created on: Nov 4, 2022
 *      Author: italo.francis
 */

#ifndef INC_DADOS_H_
#define INC_DADOS_H_


#define MAX_TENTATIVAS		5

typedef enum
{
  Status_Mem_OK,
  Status_Mem_Cheia,
  Status_Mem_Erro,
 // Status_Mem_Erro_FileSystem
} TStatusMemoria;

typedef union INFO_BOOT {
	uint8_t buffer[13];
	struct {
		char Assinatura[10]; //
		char Versao[3];
	} infoboot;
} INFO_BOOT;



typedef union Config {
	uint8_t dados[sizeof(INFO_BOOT)];
	INFO_BOOT parametros;
} TConfig;


typedef struct FLAGS_FLASH
{
	uint8_t gravacaoFlashHabilitada;
	uint8_t reservado1;
	uint8_t reservado2;
	uint8_t reservado3;
} TFlagsControleDados;

extern TFlagsControleDados f_controleDados;

extern TConfig Boot_Config;












#endif /* INC_DADOS_H_ */
