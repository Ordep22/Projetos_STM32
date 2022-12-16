/*
 * Comunicacao.h
 *
 *  Created on: Oct 28, 2022
 *      Author: italo.francis
 */

#ifndef INC_COMUNICACAO_H_
#define INC_COMUNICACAO_H_

#include "main.h"
#include <stdio.h>
#include <string.h>


#define TIMEOUT_COMUNICACAO_RX		10000 //
#define TIMEOUT_COMUNICACAO_TX		10000
#define TIMEOUT_SERIAL 60000




//Define do protocolo que de comunicacao
#define __LUPA_PROT__



//Validacao com protocolo lupa dominique para teste e implementacao
#if defined (__LUPA_PROT__)

#include "ProtocoloLupa.h"

#endif


//Aguardando definicao para implementacao do LPB
#if defined (__LUPA_LPB__)



#endif



enum Serial_Bootloader_status{
	SERIAL_BL_INICIALIZAR ,
	SERIAL_BL_OCUPADO ,
	SERIAL_BL_PROCESSANDO,
	SERIAL_BL_ERRO,
	SERIAL_BL_FIM
};


enum {
	ESTADO_AGUARDANDO_PACOTE,
	ESTADO_RECEBENDO_HEADER,
	ESTADO_RECEBENDO_DADOS,
	ESTADO_PROCESSANDO_PACOTE,
	ESTADO_ENVIANDO_PACOTE_DADOS
};


extern uint16_t timeoutUART;
void ReceberSerial(uint8_t byte);
void ProcessarMaquinaEstados(uint8_t byte);
void ProcessarUART();


#endif /* INC_COMUNICACAO_H_ */
