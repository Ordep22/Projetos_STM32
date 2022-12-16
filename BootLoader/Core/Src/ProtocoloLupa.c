/*
 * ProtocoloLupa.c
 *
 *  Created on: Oct 28, 2022
 *      Author: italo.francis
 */


#include "ProtocoloLupa.h"


//---------------------------------------------------------------------------
/**
 * @brief Monta pacote de comunica��o, preenchendo buffer com cabe�alho no formato do protocolo
 * @param buffer: Ponteiro para o buffer
 * @param funcao: C�digo do comando do protocolo
 * @param tamanho: Quantidade de bytes de dados do pacote
 * @retval Quantidade de bytes do pacote completo, incluindo o checksum
 */

uint16_t MontarPacote(uint8_t *buffer, uint8_t funcao, uint16_t tamanho) {

	uint16_t n = TAMANHO_HEADER + tamanho;

	/* 4 Bytes de String */
	buffer[0] = 'L';
	buffer[1] = 'U';
	buffer[2] = 'P';
	buffer[3] = 'A';
	/*2 Bytes de tamanho*/
#if defined(__LSB__)
	buffer[5] = (n) >> 8;
	buffer[4] = (n) & 0xff;
#else
	buffer[4] = (n) >> 8;
	buffer[5] = (n) & 0xff;
#endif
	buffer[6] = funcao;

	// Calcula o CRC-16
	uint16_t crc = crc16_calc(&buffer[0], n);

#if defined(__CHECKSUM_INT32__)
#if defined(__LSB__)
	buffer[n + 3] = 0;
	buffer[n + 2] = 0;
	buffer[n + 1] = (crc >> 8) & 0xff;
	buffer[n] = crc & 0xff;
#else
	buffer[n] = 0;
	buffer[n + 1] = 0;
	buffer[n + 2] = (crc >> 8) & 0xff;
	buffer[n + 3] = crc & 0xff;
#endif
#else
#if defined(__LSB__)
	buffer[n + 1] = crc >> 8;
	buffer[n] = crc & 0xff;
#else
	buffer[n] = crc >> 8;
	buffer[n + 1] = crc & 0xff;
#endif
#endif

	return n + TAMANHO_CHECKSUM;
}


//---------------------------------------------------------------------------
/**
 * @brief Verifica se pacote � v�lido quanto ao comando e ao checksum
 * @param buffer: Ponteiro para o buffer
 * @retval C�digo de erro
 */
int ValidarPacote_old(uint8_t *buffer) {

	uint8_t comando = buffer[6];
	uint32_t checksumRecebido;
	uint32_t checksumCalculado = 0;

//	PROTOCOLO *dadosComunicacao = buffer;
//	uint16_t bytes = dadosComunicacao->pacote.tamanho;

#if defined(__LSB__)
	uint16_t bytes = (buffer[5] << 8) + buffer[4];
#else
	uint16_t bytes = (buffer[4] << 8) + buffer[5];
#endif

	for (int i = 0; i < bytes; i++)
		checksumCalculado += buffer[i];

#if defined(__CHECKSUM_INT32__)
#if defined(__LSB__)
	checksumRecebido = (buffer[bytes + 3] << 24) + (buffer[bytes + 2] << 16) + (buffer[bytes + 1] << 8) + buffer[bytes];
#else
	checksumRecebido = (buffer[bytes] << 24) +
	(buffer[bytes + 1] << 16) +
	(buffer[bytes + 2] << 8) +
	buffer[bytes + 3];
#endif
#else
#if defined(__LSB__)
	checksumRecebido = (buffer[bytes + 1] << 8) + buffer[bytes];
#else
	checksumRecebido = (buffer[bytes] << 8) + buffer[bytes + 1];
#endif
#endif

	if (checksumRecebido != checksumCalculado)
		return E_CHECKSUM;

	if (comando > QTD_COMANDOS)
		return E_COMANDO_INVALIDO;

	return E_PACOTE_OK;

}

//---------------------------------------------------------------------------

int ValidarPacote(uint8_t *buffer) {

	uint8_t comando = buffer[6];
	uint32_t crcRecebido;
	uint16_t crcCalculado = 0;

//	PROTOCOLO *dadosComunicacao = buffer;
//	uint16_t bytes = dadosComunicacao->pacote.tamanho;

#if defined(__LSB__)
	uint16_t bytes = (buffer[5] << 8) + buffer[4];
#else
	uint16_t bytes = (buffer[4] << 8) + buffer[5];
#endif

	crcCalculado = crc16_calc(&buffer[0], bytes);


#if defined(__CHECKSUM_INT32__)
#if defined(__LSB__)
	crcRecebido = (buffer[bytes + 3] << 24) + (buffer[bytes + 2] << 16) + (buffer[bytes + 1] << 8) + buffer[bytes];
#else
	crcRecebido = (buffer[bytes] << 24) +
	(buffer[bytes + 1] << 16) +
	(buffer[bytes + 2] << 8) +
	buffer[bytes + 3];
#endif
#else
#if defined(__LSB__)
	crcRecebido = (buffer[bytes + 1] << 8) + buffer[bytes];
#else
	crcRecebido = (buffer[bytes] << 8) + buffer[bytes + 1];
#endif
#endif

	if (crcRecebido != crcCalculado)
		return E_CHECKSUM;

	if (comando > QTD_COMANDOS)
		return E_COMANDO_INVALIDO;

	return E_PACOTE_OK;

}


//---------------------------------------------------------------------------

uint16_t MontarPacoteExtenso(uint8_t *buffer, uint8_t funcao, uint16_t tamanho, uint16_t totalPacotes, uint16_t indicePacote) {

	uint16_t n = TAMANHO_HEADER_EXT + tamanho;

	/* 4 Bytes de String */
	buffer[0] = 'L';
	buffer[1] = 'U';
	buffer[2] = 'P';
	buffer[3] = 'A';
	/*2 Bytes de tamanho*/
#if defined(__LSB__)

	buffer[5] = (n) >> 8;
	buffer[4] = (n) & 0xff;

	buffer[8] = totalPacotes >> 8;
	buffer[7] = totalPacotes & 0xff;

	buffer[10] = indicePacote >> 8;
	buffer[9] = indicePacote & 0xff;

#else
	buffer[4] = (n) >> 8;
	buffer[5] = (n) & 0xff;

	buffer[7] = totalPacotes >> 8;
	buffer[8] = totalPacotes & 0xff;

	buffer[9] = indicePacote >> 8;
	buffer[10] = indicePacote & 0xff;
#endif
	buffer[6] = funcao;

	// Calcula o CRC-16
	uint16_t crc = crc16_calc(&buffer[0], n);

#if defined(__CHECKSUM_INT32__)
#if defined(__LSB__)
	buffer[n + 3] = 0;
	buffer[n + 2] = 0;
	buffer[n + 1] = (crc >> 8) & 0xff;
	buffer[n] = crc & 0xff;
#else
	buffer[n] = 0;
	buffer[n + 1] = 0;
	buffer[n + 2] = (crc >> 8) & 0xff;
	buffer[n + 3] = crc & 0xff;
#endif
#else
#if defined(__LSB__)
	buffer[n + 1] = crc >> 8;
	buffer[n] = crc & 0xff;
#else
	buffer[n] = crc >> 8;
	buffer[n + 1] = crc & 0xff;
#endif
#endif

	return n + TAMANHO_CHECKSUM;
}

//---------------------------------------------------------------------------







