/*
 * Flash.c
 *
 *  Created on: Oct 26, 2022
 *      Author: italo.francis
 */

#include "flash.h"


/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

uint32_t NbOPages = 0;
uint32_t PAGEError = 0;
uint32_t Index = 0;

/**
 * @brief Retorna Addr da página referente ao endereço passado como parametro
 *
 * @param Address
 * @return Endereço inicial referente a página
 *
 */

uint32_t GetPagePageAdress(uint32_t Address)
{

	for (int indx = 0; indx < FLASH_TOTAL_PAGES; indx++)
	{

		if ((Address < (0x08000000 + (FLASH_PAGE_SIZE * (indx + 1)))) && (Address >= (0x08000000 + FLASH_PAGE_SIZE * indx)))

		{

			return (0x08000000 + FLASH_PAGE_SIZE * indx);
		}
	}

	return 0;
}

/**
 * @brief
 *
 * @param Address
 * @return Retona
 */

uint16_t GetNumPage(uint32_t Address)
{

	for (int indx = 0; indx < (FLASH_TOTAL_PAGES / 2); indx++)
	{

		if ((Address < (0x08000000 + (FLASH_PAGE_SIZE * (indx + 1))))&& (Address >= (0x08000000 + FLASH_PAGE_SIZE * indx)))
		{
			return (FLASH_TOTAL_PAGES / 2) - indx;
		}
	}

	return 0;
}


/**
 * @brief
 *
 * @param PageAdress
 * @param NbPages
 * @return
 */
int FlashErasePage(uint32_t PageAdress, uint16_t NbPages)
{

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = GetPagePageAdress(PageAdress);
	EraseInitStruct.NbPages = NbPages;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{

		return 0;
	}

	HAL_FLASH_Lock();

	return 1;
}


int FlashEraseFromAddress (uint32_t PageAdress){

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = GetPagePageAdress(PageAdress);
	EraseInitStruct.NbPages = GetNumPage(EraseInitStruct.PageAddress);

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
		{
			return 0;
		}

		HAL_FLASH_Lock();

		return 1;
}



/**
 * @briefFuin
 *
 * @param StartPageAddress
 * @param Data
 * @param size
 * @return
 */

int FlashWriteData( uint32_t *Data, int32_t size, uint32_t Addrress)

{
	int index = 0;

		HAL_FLASH_Unlock();

		while (index < size)
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Addrress, Data[index]) == HAL_OK)
			{

				//Addrress += 2; //Half Word
			      Addrress += 4; //Word
				//Addrress += 8; //double word
				index++;
			}
			else
			{
				/* Error occurred while writing data in Flash memory*/
				return HAL_FLASH_GetError();
			}
		}
	/* Lock the Flash to disable the flash control register access */
	HAL_FLASH_Lock();

	return 0;
}



/**
 * @brief
 *
 * @param StartPageAddress
 * @param RxBuf
 * @param size
 */

void FlashReadData (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t size)
{
	while (1)
	{
		*RxBuf = *(__IO uint32_t *)StartPageAddress;
		StartPageAddress += 4;
		RxBuf++;
		if (!(size--)) break;
	}
}


void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {

      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {

    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;

   return float_variable;
}


void Convert_To_Str(uint32_t *Data, char *Buf)
{
	int numberofbytes = ((strlen((char*) Data) / 4) + ((strlen((char*) Data) % 4) != 0)) * 4;

	for (int i = 0; i < numberofbytes; i++)
	{
		Buf[i] = Data[i / 4] >> (8 * (i % 4));
	}
}





