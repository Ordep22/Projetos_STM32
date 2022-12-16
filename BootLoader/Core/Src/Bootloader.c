/*
 * Bootloader.c
 *
 *  Created on: 27 de out de 2022
 *      Author: italo.francis
 */

#include "flash.h"
#include "Bootloader.h"


enum CHECK_ASSINATURA {
  ASSINATURA_INVALIDA = 0,
  ASSINATURA_OK,
  ASSINATURA_BOOTLOADER,
  ASSINATURA_GRAVANDO
  //ASSINATURA_ISP
};




typedef struct
{
  //unsigned char CrcErrOnStartup;
  unsigned char RunUserCode;
  unsigned char UserCodePresent;
} TSignatureParams;


//const char dftSignature[TAMANHO_ASSINATURA] = "LUPALUPA";
//const char btlSignature[TAMANHO_ASSINATURA] = "BOOTLOAD";
//const char grvSignature[TAMANHO_ASSINATURA] = "GRAVANDO";
//const char ispSignature[TAMANHO_ASSINATURA] = "ENTERISP";















