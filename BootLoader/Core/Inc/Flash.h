/*
 * Flash.h
 *
 *  Created on: Oct 26, 2022
 *      Author: italo.francis
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"
#include <string.h>




#define FLASH_BASE_ADDR  (uint32_t) ADDR_FLASH_SECTOR_0

#define FLASH_END_ADDR   ((uint32_t) 0x0801FFFF)

#define FLASH_TOTAL_PAGES 128


//Definicao inicial até a finalizacao
#define FLASH_USER_START_ADDR ((uint32_t) 0x0800500)

#define FLASH_USER_END_ADDR   ((uint32_t) 0x0801FFFF)




//Memória flash  STM32F072

#define ADDR_FLASH_SECTOR_0   ((uint32_t)0x08000000)

#define ADDR_FLASH_SECTOR_1   ((uint32_t)0x08001000)

#define ADDR_FLASH_SECTOR_2   ((uint32_t)0x08002000)

#define ADDR_FLASH_SECTOR_3   ((uint32_t)0x08003000)

#define ADDR_FLASH_SECTOR_4   ((uint32_t)0x08004000)

#define ADDR_FLASH_SECTOR_5   ((uint32_t)0x08005000)

#define ADDR_FLASH_SECTOR_6   ((uint32_t)0x08006000)

#define ADDR_FLASH_SECTOR_7   ((uint32_t)0x08007000)

#define ADDR_FLASH_SECTOR_8   ((uint32_t)0x08008000)

#define ADDR_FLASH_SECTOR_9   ((uint32_t)0x08009000)

#define ADDR_FLASH_SECTOR_10   ((uint32_t)0x0800A000)

#define ADDR_FLASH_SECTOR_11   ((uint32_t)0x0800B000)

#define ADDR_FLASH_SECTOR_12   ((uint32_t)0x0800C000)

#define ADDR_FLASH_SECTOR_13   ((uint32_t)0x0800D000)

#define ADDR_FLASH_SECTOR_14   ((uint32_t)0x0800F000)

#define ADDR_FLASH_SECTOR_15   ((uint32_t)0x08010000)

#define ADDR_FLASH_SECTOR_16   ((uint32_t)0x08011000)

#define ADDR_FLASH_SECTOR_17   ((uint32_t)0x08012000)

#define ADDR_FLASH_SECTOR_18   ((uint32_t)0x08013000)

#define ADDR_FLASH_SECTOR_19   ((uint32_t)0x08014000)

#define ADDR_FLASH_SECTOR_20   ((uint32_t)0x08015000)

#define ADDR_FLASH_SECTOR_21   ((uint32_t)0x08016000)

#define ADDR_FLASH_SECTOR_22   ((uint32_t)0x08017000)

#define ADDR_FLASH_SECTOR_23   ((uint32_t)0x08018000)

#define ADDR_FLASH_SECTOR_24   ((uint32_t)0x08019000)

#define ADDR_FLASH_SECTOR_25   ((uint32_t)0x0801A000)

#define ADDR_FLASH_SECTOR_26   ((uint32_t)0x0801B000)

#define ADDR_FLASH_SECTOR_27   ((uint32_t)0x0801C000)

#define ADDR_FLASH_SECTOR_28   ((uint32_t)0x0801D000)

#define ADDR_FLASH_SECTOR_29   ((uint32_t)0x0801E000)

#define ADDR_FLASH_SECTOR_30   ((uint32_t)0x0801F000)

#define SETOR_INVALIDO  0xFFFFFFFF






enum Boot_Status{

	BT_BOOTLOADER = 0,
	BT_RESET = -1,
	BT_DEFAULT = 1,
};




 // Possible error codes, these are negative to allow
 // valid positive return values
 enum lfs_error {
     LFS_ERR_OK          = 0,    // No error
     LFS_ERR_IO          = -5,   // Error during device operation
     LFS_ERR_CORRUPT     = -84,  // Corrupted
     LFS_ERR_NOENT       = -2,   // No directory entry
     LFS_ERR_EXIST       = -17,  // Entry already exists
     LFS_ERR_NOTDIR      = -20,  // Entry is not a dir
     LFS_ERR_ISDIR       = -21,  // Entry is a dir
     LFS_ERR_NOTEMPTY    = -39,  // Dir is not empty
     LFS_ERR_BADF        = -9,   // Bad file number
     LFS_ERR_FBIG        = -27,  // File too large
     LFS_ERR_INVAL       = -22,  // Invalid parameter
     LFS_ERR_NOSPC       = -28,  // No space left on device
     LFS_ERR_NOMEM       = -12,  // No more memory available
     LFS_ERR_NOATTR      = -61,  // No data/attr available
     LFS_ERR_NAMETOOLONG = -36,  // File name too long
 };









uint32_t GetPagePageAdress (uint32_t Address);


int FlashWriteData( uint32_t *Data, int32_t size, uint32_t StartPageAddress);


void FlashReadData (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords);

int FlashEraseFromAddress (uint32_t PageAdress);

int FlashErasePage(uint32_t PageAdress, uint16_t NbPages);


typedef union Versao {

	uint8_t buffer[3+sizeof(float)];
	struct {
		uint8_t start[3]; // FAI
		float versao;//00.00
	} dados;
} VERSAO_FIRM;



#endif /* INC_FLASH_H_ */
