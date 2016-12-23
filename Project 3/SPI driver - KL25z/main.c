/********************************************
*   File: main.c
*
*   Copyrights 2016 Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*
*   Author: Snehal Sanghvi and Shalin Shah
*   Date Edited: 11 Nov 2016
*
*   Description: Calls the Nordic and SPI functions
*
********************************************************/

#include "MKL25Z4.h"
#include "spi.h"
#include "nRF24L01.h"
#include <stdint.h>
#include "LED.h"
#include "log.h"


int main(void) {
	unsigned char c;
	uint8_t status=0;
	uint8_t fifo_status;
	UART0_init();
	SPI_init();

	// calling the tx configure function
	NORDIC_ConfigTX();

	// checking the value of status register
	status = NORDIC_Get_Status();
	if(status==0xe){
		LOG_0("\n\rStatus received is 0x0e",25);
	}
	else
		LOG_0("\n\rDifferent",11);

	//calling the rf setup function
	NORDIC_RF_Setup();

	fifo_status = NORDIC_Read(NORDIC_FIFO_STATUS);

	//performing sanity checks on FIFO STATUS register
	if(fifo_status && 0x11)
		LOG_0("\n\rFIFO status is 0b00010001 -> empty tx and rx buffer",53);
	else
		LOG_0("\n\rFaulty configuration",22);

	//setting tx address
	NORDIC_Set_TX_Address();

	while(1) {
	}
}

