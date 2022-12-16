/*----------------------------------------------------------------------------
 *   Decodifica arquivos HEX - Lupa Tecnologia e Sistemas Ltda
 *----------------------------------------------------------------------------
 *   Nome:      decodHex.h
 *   Descri��o: Implementa fun��es de decodifica��o do arquivo de firmware
 *              no padrão HEX
 *----------------------------------------------------------------------------
 *   Copyright (c) 2021 Lupa Tecnologia e Sistemas Ltda. All rights reserved.
 *---------------------------------------------------------------------------*/

#ifndef DECODHEX_H_
#define DECODHEX_H_


#include "ProtocoloLupa.h"

typedef struct{
	uint8_t inicio;
	uint8_t tamanho;
	uint16_t address;
	uint8_t recordType;
	uint8_t data[32];
	uint8_t checksum;
} linhaHEX;




typedef union{
	uint8_t buffer[sizeof(linhaHEX)];
	linhaHEX parametros;
} linhaHEX_union;
enum{
	FIRM_FIM = 1,
	FIRM_OK = 0,
	FIRM_ERROR = -1,
	FIMM_CS_ERROR = -2,
	FIRM_ESPERAR_LINHA = 2,
};



typedef union ATT_FIRMWARE_STATUS {
	uint8_t buffer[3];
	struct {
		uint8_t status;
		uint16_t linhas_gravadas;
	} stats_firm;
} STATS_FIRMWARE;

STATS_FIRMWARE decodificarHEX(PROTOCOLO *Rx);

int FlashWrite(const void *buffer, uint32_t size, uint32_t addr);

int decodeLine(linhaHEX *LH);

int decodificarLINHA(linhaHEX *LH);
uint8_t SalvarFirmware();
uint8_t ConverterBytes(uint8_t MaiorHex, uint8_t MenorHex);
#endif /* DECODHEX_H_ */
