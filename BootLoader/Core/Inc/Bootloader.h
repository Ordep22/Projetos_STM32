/*
 * BootLoader.h
 *
 *  Created on: Oct 26, 2022
 *      Author: italo.francis
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

#include <stdio.h>
#include <stdbool.h>



#define __BOOTLOADER_LUPA__ // Habilita funçõees específicas do bootloader


#define TAMANHO_ASSINATURA  8

//int sbl_user_code_signature_check(void);
void sbl_set_signature(void);
void sbl_set_blt_signature(void);
void sbl_set_grv_signature(void);
void sbl_set_ISP_signature(void);



//unsigned char eIAP_user_code_present(void);
//void eIAP_execute_code(uint32_t Address);

void Bootloader_LerAssinatura(char *buffer, uint8_t printDebug);
uint8_t Bootloader_AtualizarFirmware();
void Bootloader_InicializarSistema();






#endif /* INC_BOOTLOADER_H_ */
