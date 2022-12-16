/*
 * ProtocoloLupa.h
 *
 *  Created on: Oct 28, 2022
 *      Author: italo.francis
 */

#ifndef INC_PROTOCOLOLUPA_H_
#define INC_PROTOCOLOLUPA_H_

#include <stdio.h>
#include <string.h>
#include "crc16.h"
#include "main.h"
#include "flash.h"

#include "Bootloader.h"

//--------------------------------------------------------------------
// Comentar diretiva abaixo para considerar o padr�o MSB (Most Significant Byte First)
#define __LSB__ 			// Utiliza protocolo considerando o padr�o LSB (Least Significant Byte First)

// Comentar diretiva abaixo para utilizar checksum em 16 bits (2 bytes)
//#define __CHECKSUM_INT32__	// Checksum calculado em 32 bits (4 bytes)

//--------------------------------------------------------------------

#define TAMANHO_BUFFER		260 //Original 4096
#define TAMANHO_HEADER		7
#define TAMANHO_HEADER_EXT	(TAMANHO_HEADER + 4)

#if defined(__CHECKSUM_INT32__)

	#define TAMANHO_CHECKSUM	4
#else
#define TAMANHO_CHECKSUM	2
#endif

#define TAMANHO_MIN_PACOTE	(TAMANHO_HEADER + TAMANHO_CHECKSUM)

#define TAMANHO_MAX_DADOS	(TAMANHO_BUFFER - TAMANHO_HEADER_EXT - TAMANHO_CHECKSUM)

#define MAX_TENTATIVAS		5

typedef union Protocolo
{
	uint8_t buffer[TAMANHO_BUFFER];
	struct
	{
		uint8_t start[4];
		uint16_t tamanho;
		uint8_t comando;
		uint8_t dados[TAMANHO_BUFFER - TAMANHO_HEADER]; //considerar checksum nos dados
//		uint16_t checksum;
	} pacote;
} PROTOCOLO;

typedef struct ControleTransferenciaDados
{
	uint8_t estado;
	uint8_t comando;
	uint8_t flagRetorno;
	uint8_t tentativas;
	uint16_t tamanhoDadosPacote;
	uint32_t totalBytes;
	uint16_t totalPacotes;
	uint16_t indexPacote;
	uint8_t parametros[8];
	uint8_t *pDados;

} CONTROLE_TRANSFERENCIA_DADOS;


typedef union Procolo_Valores
{
	uint8_t buffer[TAMANHO_BUFFER];
	struct
	{
		uint8_t start[4];
		uint16_t tamanho;
		uint8_t comando;
		float dados[TAMANHO_BUFFER - TAMANHO_HEADER];
	} pacote_valores;

} PROTOCOLO_VALORES;

// Comandos do protocolo Lupa para o FAI (adaptar para o MPA23
enum comandos
{
	CMD_STATUS, CMD_ERRO_CHK, CMD_INVALIDO, CMD_BUSY, CMD_PING,


	// Comandos Bootloader

	CMD_ABRIR_BOOTLOADER,
	CMD_ENVIANDO_FIRMWARE,
	CMD_BOOT_GRAVAR_HEX,
	CMD_BOOT_ISP,
	CMD_BOOT_DEFAULT,
	CMD_SYSTEM_RESET,
	CMD_COMPARAR_FIRMWARE,
	CMD_ACK,
	CMD_NACK,
	CMD_CLOSE_FILES,

	//CMD_ACK_OSCILOGRAFIA,

	//------------------
	// Comandos Detector FAI
//	CMD_GET_IH_CUR_DATA_CMD,
//	CMD_LER_CONFIG,
//	CMD_GRAVAR_CONFIG,
//	CMD_RESET_IH,
//
//	CMD_LER_ARQUIVO,
//	CMD_LER_DIRETORIO,

//------------------
	QTD_COMANDOS
};

enum
{
	E_PACOTE_INVALIDO = -4,
	E_BUSY = -3,
	E_CHECKSUM = -2,
	E_COMANDO_INVALIDO = -1,
	E_PACOTE_OK = 0
};

enum
{
	RET_OK, RET_ERRO_GRAVACAO, RET_ERRO_LEITURA, RET_ERRO_DADOS, RET_CANCELAR
};



//--------------------------------------------------------------------



int ValidarPacote(uint8_t *buffer);
uint16_t MontarPacote(uint8_t *buffer, uint8_t funcao, uint16_t tamanho);
uint16_t MontarPacoteExtenso(uint8_t *buffer, uint8_t funcao, uint16_t tamanho, uint16_t totalPacotes, uint16_t indicePacote);

//--------------------------------------------------------------------




#endif /* INC_PROTOCOLOLUPA_H_ */
