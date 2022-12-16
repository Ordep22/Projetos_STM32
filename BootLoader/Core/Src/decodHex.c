/*----------------------------------------------------------------------------
 *   Decodifica arquivos HEX - Lupa Tecnologia e Sistemas Ltda
 *----------------------------------------------------------------------------
 *   Nome:      decodHex.c
 *   Descrição: Implementa funções de decodificação do arquivo de firmware
 *              no padrão HEX
 *----------------------------------------------------------------------------
 *   Copyright (c) 2021 Lupa Tecnologia e Sistemas Ltda. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "decodHex.h"
#include "flash.h"

#define HEADER_LINHA 9
#define INICIO_LINHA ':'

STATS_FIRMWARE stats_firm;
//-----------------------------------------------------
STATS_FIRMWARE decodificarHEX(PROTOCOLO *Rx) {

	PROTOCOLO *dRX = Rx;
	STATS_FIRMWARE *stats = &stats_firm;


//{0x3A,0x03,0x00,0x30,0x00,0x02,0x33,0x7A,0x1E}
	linhaHEX linha;
	memset(&linha,0x0,sizeof(linhaHEX));

	int i = 0;
	uint8_t contBytes = 0;
	int respostaLinha = 0;
	uint8_t checksum = 0;
	uint8_t decod;
	uint8_t cs1[2];
	uint8_t addr;

	do
	{
		//-------- Acha o :
		//if(strcmp(dRX->pacote.dados[i],INICIO_LINHA) == 0 ){

		if(dRX->pacote.dados[i] == INICIO_LINHA)
		{
			memset(&linha,0x0,sizeof(linha));
			memset(&checksum,0x0,sizeof(checksum));
			memset(&contBytes,0x0,sizeof(contBytes));
			memset(&respostaLinha,0x0,sizeof(respostaLinha));

			linha.inicio = dRX->pacote.dados[i];

			//-------- Decodifica quantos bytes tem a linha
			linha.tamanho = ConverterBytes(dRX->pacote.dados[i+1], dRX->pacote.dados[i+2]);

			//------- Decodifica Address
			linha.address = (uint16_t) ConverterBytes(dRX->pacote.dados[i + 3], dRX->pacote.dados[i+4]);
			addr = ConverterBytes(dRX->pacote.dados[i+5], dRX->pacote.dados[i+6]);
			linha.address = linha.address << 8| (uint16_t) (addr & 0xff);

			//------- Decodifica Record Type
			linha.recordType = ConverterBytes( dRX->pacote.dados[i+7],  dRX->pacote.dados[i+8]);

			//------- Decodifica os Dados
			for (int w = 0; w < linha.tamanho * 2; w = w + 2) {
				decod = ConverterBytes(dRX->pacote.dados[i + HEADER_LINHA + w],	dRX->pacote.dados[i+ HEADER_LINHA + w + 1]);
				linha.data[w / 2] = decod;
			}

			//------- Decodifica CheckSum
			cs1[0] = dRX->pacote.dados[i + HEADER_LINHA + linha.tamanho*2];
			cs1[1] = dRX->pacote.dados[i + HEADER_LINHA + linha.tamanho*2 + 1];
			linha.checksum = ConverterBytes(cs1[0],	cs1[1]);

			//------- Verifica CheckSum da linha
			checksum+=linha.tamanho;
			checksum+= ConverterBytes(dRX->pacote.dados[i + 3], dRX->pacote.dados[i + 4]); //addr
			checksum+= ConverterBytes(dRX->pacote.dados[i + 5], dRX->pacote.dados[i + 6]);//addr
			checksum+=linha.recordType;

			for( int j = 0 ; j< linha.tamanho;j++)
			{
				checksum+=linha.data[j];
			}

			if ((uint8_t) ((~checksum + 1) & 0xff) == linha.checksum)
			{
			  //----- Copia os bytes para a estrutura
				contBytes = FIRM_OK;

				i += linha.tamanho * 2 + HEADER_LINHA + 2;

				//Identificou uma linha completa, salvar no SD

                printf("Linha Inicio: %02X \r\n", linha.inicio);
                printf("Linha Tamanho: %02d \r\n", linha.tamanho);
                printf("Linha Endereco: %04X \r\n", linha.address);
                printf("Linha Record Type: %02X \r\n", linha.recordType);

                for (int W = 0; W < linha.tamanho; W++){

                	printf("%02X", linha.data[W]);

                }

                printf("\n");

				//f_printf(fil, "%02X", linha.inicio);


				//Substituir pela versao de gravacao na flash externa
			    //f_putc(linha.inicio, fil);


               // f_printf(fil, "%02X", linha.tamanho);
			//	f_printf(fil, "%04X", linha.address);
			//	f_printf(fil, "%02X", linha.recordType);


			//	for (int W = 0; W < linha.tamanho; W++)
				//{

					//f_printf(fil, "%02X", linha.data[W]);
					//printf("%02X", linha.data[W]);

				//}



                // decodificarLINHA(&linha);

                // decodeLine(&linha);



				//f_printf(fil, "%02X", linha.checksum);

				//printf("%02X", linha.checksum);

				//f_printf(fil, "\n");


				//respostaLinha = FIRM_FIM;

				if (linha.recordType == 0x1)
				{
				  contBytes = FIRM_FIM;
        }
				stats->stats_firm.linhas_gravadas += 1;
			}
			else
			{
        contBytes = FIMM_CS_ERROR;
        break;
      }
		}
		else
		{
			i++; // i = percorre os indices

		}
		//------ Linha OK! Decodificar Instrucoes

		//Valor FAI  while (dRX->pacote.tamanho - 2 > i);

	} while (dRX->pacote.tamanho - 7 > i);


	// Implementar para salvar na flash externa

	//Dados_SalvarArquivo(fil);

	memcpy(&stats->stats_firm.status, &contBytes, sizeof(contBytes));
	return *stats;
}

//#if defined(__BOOTLOADER_LUPA__)

int primeiraIteracao;
//-----------------------------------------------------

/*

//uint8_t SalvarFirmware(FIL *fil)

uint8_t SalvarFirmware()
{
  printf("[Info] Gravando novo firmware na Flash... \r\n");

	uint8_t stats;
	char bufferSD[64] = {0};

	uint8_t decod;
	uint8_t cs1[2];
	uint8_t addr;
	uint32_t contaLinhas = 0;

	// Antes de iniciar grava��o apaga assinatura da Flash

  uint32_t setor = GetPagePageAdress(FLASH_USER_START_ADDR);

  printf("Limpando assinatura na Flash (setor 0x%08x)\r\n", (unsigned int)setor);

  //  SCB_CleanDCache();
  //SCB_CleanInvalidateDCache();

  int err = FlashErasePage(FLASH_USER_START_ADDR, 100);

  //int err = EraseFlashBlockByAddress(FLASH_USER_SIGN_ADDR);

  HAL_Delay(100);

  if (err != HAL_OK)
  {
    printf("[Erro] Erro ao limpar assinatura na Flash no endereco 0x%08x - Cod. Erro: %i\r\n", (unsigned int) FLASH_USER_START_ADDR, err);
    HAL_Delay(50);
    return FIRM_ERROR;
  }

  do
	{
		memset(&linha,0x0,sizeof(linhaHEX));
		memset(&bufferSD,0x0,sizeof(bufferSD));

   //f_gets(bufferSD, sizeof(bufferSD), fil); //--------------------------------> Le uma linha do arquivo

    linha.inicio = bufferSD[0];

    linha.tamanho = ConverterBytes(bufferSD[1], bufferSD[2]);

    linha.address = (uint16_t) ConverterBytes(bufferSD[3], bufferSD[4]);

    addr = ConverterBytes(bufferSD[5], bufferSD[6]);

    linha.address = linha.address << 8| (uint16_t) (addr & 0xff);

    cs1[0] = bufferSD[HEADER_LINHA + linha.tamanho*2];
    cs1[1] = bufferSD[HEADER_LINHA + linha.tamanho*2 + 1];
    linha.checksum = ConverterBytes(cs1[0], cs1[1]);

    linha.recordType = ConverterBytes(bufferSD[7], bufferSD[8]);

    for (int i = 0; i < linha.tamanho * 2; i = i + 2)
    {
      decod = ConverterBytes(bufferSD[HEADER_LINHA + i], bufferSD[HEADER_LINHA + i + 1]);
      linha.data[i / 2] = decod;
    }

		stats = decodificarLINHA(&linha); // salvar na flash

		if (++contaLinhas % 500 == 0)
		{
		  printf(". \r\n"); // Printa na serial para mostrar progresso
		}
	} while (stats != FIRM_FIM);


	// Ap�s finalizar, grava assinatura padr�o na Flash
	if (stats == FIRM_FIM)
	{
    printf("Gravando assinatura na Flash \r\n");
    HAL_Delay(50);

    extern const char dftSignature[TAMANHO_ASSINATURA];
    uint8_t data[32];
    memset(data,0xFF,sizeof(data));
    memcpy(data,dftSignature,TAMANHO_ASSINATURA);
    err = WriteFlashSignature(data); // Salva assinatura na mem�ria flash
    if (err != LFS_ERR_OK)
    {
      printf("[Erro] Erro ao gravar assinatura na Flash no endereco 0x%08x - Cod. Erro: %i\r\n", (unsigned int)FLASH_USER_SIGN_ADDR, err);
      HAL_Delay(50);
      return FIRM_ERROR;
    }

	}

	return stats;

}

*/

uint32_t EnderecoEnviado = 0;
uint16_t EnderecoSuperior = 0x0;
int err;
int horaDeSalvar = 0;
uint32_t setor, off;
linhaHEX LH_SAVE;
uint32_t ultimoEndereco = 0;

int decodeLine(linhaHEX * linha)
{


	int contBytes = 0;

	EnderecoEnviado = (uint32_t) EnderecoSuperior << 16| (uint32_t) ( linha->address & 0xffff); // Inicio da região de memória 0x0800 000

	uint16_t record =  linha->recordType;


	switch (record)

		{

	case 0x0: // Dados a serem salvos na memória flash

			// Verifica se o Setor está formatado
			if (horaDeSalvar == 0)
			{

				setor = GetPagePageAdress(EnderecoEnviado); // Setor
				off = EnderecoEnviado;

				if (setor != primeiraIteracao)

				{ // Verifica se é necessário apagar o setor antes de salvar

					if (setor != SETOR_INVALIDO)
					{

						printf("[Info] Apagando setor 0x%08x\r\n", (unsigned int) setor);

						//err = FlashEraseFromAddress(EnderecoEnviado);

						HAL_Delay(100);
					}

	#if defined(__PRINT_DEBUG_ERRO__)
		        else
		        {
		          printf("Setor Invalido - endereco 0x%08x\r\n", EnderecoEnviado);
		          HAL_Delay(50);
		        }
		#endif
				}
				primeiraIteracao = setor;
				horaDeSalvar++;
			}




	 	// Condição para alocar no buffe
	 		switch (LH_SAVE.tamanho)
	 		{

	 		case 0: // Primeira linha a ser verificada

	 			memcpy(&LH_SAVE.data,  linha->data,  linha->tamanho); // Copia os X bytes de informação referente ao primeiro registrador de X bytes

	 			LH_SAVE.address = EnderecoEnviado; //Primeiro Endereço enviado da sequência

	 			if ( linha->tamanho == 8 && ( linha->address == 0x0290))
	 			{
	 				//Preenche com 0xff os ultimos 8 bytes ( existe uma linha assim )
	 				memset(&LH_SAVE.data[8], 0xff, 8); // Copia os ultimos 16bytes que formam o registrador de 32 bytes
	 				LH_SAVE.tamanho += 16; // Atualiza o tamanho da variável
	 			}
	 			else
	 				LH_SAVE.tamanho =  linha->tamanho; //Salva o tamanho na variavel que irá para o registrador

	 			contBytes = FIRM_ESPERAR_LINHA;

	 			break;

	 		case 4: //4+16

	 			LH_SAVE.tamanho +=  linha->tamanho;

	 			memcpy(&LH_SAVE.data[4],  linha->data,  linha->tamanho); // Copia os ultimos 16bytes que formam o registrador de 32 bytes

	 			break;

	 		case 8: //Deslocou 8 Bytes

	 			memcpy(&LH_SAVE.data[8],  linha->data,  linha->tamanho); // Copia os ultimos 16bytes que formam o registrador de 32 bytes

	 			LH_SAVE.tamanho +=  linha->tamanho; // Atualiza o tamanho da variável
	 			break;

	 		case 12: //Deslocou 8 Bytes

	 			LH_SAVE.tamanho +=  linha->tamanho;
	 			memcpy(&LH_SAVE.data[12],  linha->data,  linha->tamanho); // Copia os ultimos 16bytes que formam o registrador de 32 bytes
	 			break;

	 		case 16:
	 			memcpy(&LH_SAVE.data[16],  linha->data,  linha->tamanho); // Copia os ultimos 16bytes que formam o registrador de 32 bytes

	 			if ( linha->tamanho == 8 && ( linha->address == 0x0290))
	 			{
	 				//Preenche com 0xff os ultimos 8 bytes ( existe uma linha assim )
	 				memset(&LH_SAVE.data[24], 0xff, 8); // Copia os ultimos 16bytes que formam o registrador de 32 bytes
	 				LH_SAVE.tamanho += 16; // Atualiza o tamanho da variável
	 			}
	 			else

	 				LH_SAVE.tamanho +=  linha->tamanho; // Atualiza o tamanho da variável

	 			break;

	 		case 20:

	 			LH_SAVE.tamanho +=  linha->tamanho; //20+16 = 36
	 			memcpy(&LH_SAVE.data[20],  linha->data, 12); // Copia os ultimos 16bytes que formam o registrador de 32 bytes

	 			break;
	 		case 24: //Deslocou 24 Bytes 16 + 8

	 			LH_SAVE.tamanho +=  linha->tamanho; //= 24+16 = 40
	 			memcpy(&LH_SAVE.data[24],  linha->data, 8); // Copia os ultimos 16bytes que formam o registrador de 32 bytes

	 			break;


	 		case 28:

	 			LH_SAVE.tamanho +=  linha->tamanho; //= 28+16 = 44
	 			//LH_SAVE.address =EnderecoEnviado - 0x1C; // Alinha o Endereço 0x1C = 28
	 			memcpy(&LH_SAVE.data[28],  linha->data, 4); // Copia os ultimos 16bytes que formam o registrador de 32 bytes
	 			break;

	 		default:
	 			//Caso o tamanho nao seja 8, 16 ou 24
	 			break;
	 		}




	 		// Condição para Salvar na Flash
	 		 int statusGravacao = LFS_ERR_OK;


	   		switch (LH_SAVE.tamanho)
		{

		case 16:

			//EnderecoEnviado = ultimoEndereco + 0x20; //0x10;

			printf("Address: [%08lx] - Registro: \r\n", EnderecoEnviado);
			printf("Gravando no endereco 0x%08lx \r\n", EnderecoEnviado);

			if (EnderecoEnviado <= FLASH_END_ADDR - 32) { // Verifica se é endereço válido para escrever na Flash


				/*Rotina de salvar na flash*/

				uint8_t data2[16];


                 memcpy(&data2, LH_SAVE.data, linha->tamanho);


				//statusGravacao = FlashWriteData((uint32_t*) data2, 4, EnderecoEnviado); // Salva a sequência na memória flash


				if (statusGravacao != LFS_ERR_OK) {

					printf("[Erro] Erro ao gravar na Flash no endereco 0x%08x - Cod. Erro: %i\r\n", (unsigned int) EnderecoEnviado, statusGravacao);

					HAL_Delay(50);
				}

				/*Fim da Rotina*/

				HAL_Delay(1);

			} else {

				printf("[Erro] Endereco invalido 0x%08x\r\n", EnderecoEnviado);

				HAL_Delay(50);
			}


			ultimoEndereco = EnderecoEnviado;


			memset(&LH_SAVE, 0x0, sizeof(LH_SAVE));


			//horaDeSalvar = 0; // Zera a variável de controle

			contBytes = FIRM_OK;

			break;


   		case 36:



		break;


		}

























	case 0x1: // End of File
				  // Fim da linha

			if ( linha->tamanho == 0x00)

				contBytes = FIRM_FIM;
			//else
				//contBytes = FIRM_ERROR;
			break;

		case 0x2:
			contBytes = FIRM_ERROR;

			break;

		case 0x3:
			contBytes = FIRM_ERROR;

			break;

		case 0x4: // Extenteded Linear Address Gives the high-order 16 bits of the target address for programming  " :020000040806EC<------- The upper 16 bits of the target address: 0x0806 "

			EnderecoSuperior = (uint16_t) ( linha->data[0] << 8) | (uint16_t) ( linha->data[1] & 0xff);
			EnderecoEnviado = (uint32_t) EnderecoSuperior << 16 | (uint32_t) ( linha->address & 0xffff);
			ultimoEndereco = EnderecoEnviado - 0x20;

			horaDeSalvar = 0; // Zera a variável de controle  //*** Todo: TESTAR

			memset(&LH_SAVE, 0x0, sizeof(LH_SAVE));

			contBytes = FIRM_OK;

			break;

		case 0x5: //The address field is 0000 (not used) and the byte count is always 04. The four data bytes represent
				  //a 32-bit address value (big-endian). In the case of 80386 and higher CPUs, this address is loaded into the EIP register.
				  //x -- EnderecoEnviado = ((uint32_t)(LH->data[0]<<24) | (uint32_t)(LH->data[1] << 16)) | ((uint32_t)(LH->data[2]<<8) | (uint32_t)(LH->data[3] & 0xff));
			if (LH_SAVE.tamanho != 0)
			{
				EnderecoEnviado = ultimoEndereco + 32;			//+ LH_SAVE.tamanho;
				memset(&LH_SAVE.data[LH_SAVE.tamanho], 0x0, 32 - LH_SAVE.tamanho);
				//WriteFlash(&LH_SAVE.data, 32, setor, EnderecoEnviado); // Salva a sequência na memória flash
				/*
				 printf("Address: [%08x] - Registro: ", EnderecoEnviado);
				 printf("Salto: [%02x]  ", EnderecoEnviado - ultimoEndereco);
				 for (int j = 0; j < 32; j++) { // 32bytes
				 printf("%02x", LH_SAVE.data[j]);
				 }*/
				//printf(".\r\n");
				printf("Terminou \r\n");

				HAL_Delay(50);

			}
			contBytes = FIRM_OK;
			break;

		default:

			break;

		}

	return 0;
}










uint8_t ConverterBytes(uint8_t MaiorHex, uint8_t MenorHex)
{
uint8_t byte;

	if (MaiorHex - 0x30 <= 0x09)
	{
		byte = (MaiorHex- 0x30) << 4;
	}
	else {
		switch (MaiorHex) {
		case 'a':
		case 'A':
			byte = 0x0a << 4;
			break;
		case 'b':
		case 'B':
			byte = 0x0b << 4;
			break;
		case 'c':
		case 'C':
			byte = 0x0c << 4;
			break;
		case 'd':
		case 'D':
			byte = 0x0d << 4;
			break;
		case 'e':
		case 'E':
			byte = 0x0e << 4;
			break;
		case 'f':
		case 'F':
			byte = 0x0f << 4;
			break;
		default:
			break;
		}
	}
	if (MenorHex - 0x30 <= 0x09){
		byte |= (MenorHex- 0x30) & 0xff ;
	}
	else {
		switch (MenorHex) {
		case 'a':
		case 'A':
			byte |= 0x0a & 0xff;
			break;
		case 'b':
		case 'B':
			byte |= 0x0b & 0xff;
			break;
		case 'c':
		case 'C':
			byte |= 0x0c & 0xff;
			break;
		case 'd':
		case 'D':
			byte |= 0x0d & 0xff;
			break;
		case 'e':
		case 'E':
			byte |= 0x0e & 0xff;
			break;
		case 'f':
		case 'F':
			byte |= 0x0f & 0xff;
			break;
		default:
			break;
		}
	}
return byte;
}
