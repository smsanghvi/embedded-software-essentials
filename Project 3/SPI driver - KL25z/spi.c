/********************************************
*   File: spi.c
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
*   Date Edited: 9 Nov 2016
*
*   Description: Source file for SPI drivers
*   		
*		SPI_init
*		SPI_write_read_byte
*		SPI_status
*		SPI_flush
*
********************************************************/
#include "MKL25Z4.h"
#include "spi.h"
#include <stdint.h>


//initializing the spi module
void SPI_init(void){

	//Enabling the clock gate for SPI0
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

	//selecting port C for SPI0 and configuring its GPIO pins
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR4 |= PORT_PCR_MUX(1); 			//Select Alternate function 1 (or gpio pin) for SPI0_PCS0 (or SPI SS)
	PORTC_PCR5 |= PORT_PCR_MUX(2);			//Select Alternate function 2 for SPI0_SCK
	PORTC_PCR6 |= PORT_PCR_MUX(2); 			//Select Alternate function 2 for SPI0_MOSI
	PORTC_PCR7 |= PORT_PCR_MUX(2); 			//Select Alternate function 2 for SPI0_MISO
	PORTC_PCR3 |= PORT_PCR_MUX(1);			//For CE pin

	//configuring the ss pin
	PTC->PDDR |= 0x10;                      //selecting the ss pin as output (---1 ----)
	PTC->PSOR = 0x10;						//making the ss pin high (or idle, since its active low)

	//Selecting master mode for the SPI on MCU
	SPI0->C1 |= SPI_C1_MSTR_MASK;
	SPI0->C1 &= ~SPI_C1_CPHA_MASK;

	SPI0->C2 = 0x00;

	//setting a baud rate divisor of 4
	SPI0->BR = SPI_BR_SPR(1);

	//enabling the SPI module
	SPI0->C1 |= SPI_C1_SPE_MASK;
}


//function to return status of SPI
uint8_t SPI_status() {
	return SPI0->S;
}


//function to re-initialize the spi module
void SPI_flush()
{
	//disable the SPI module by making SPE=0
	SPI0_C1 &= 0xBF;
	//Call the initialization function
	SPI_init();
}


//transmitting and receiving a byte
uint8_t SPI_write_read_byte(unsigned char byte){
	unsigned char temp = 0;

	//waiting for transmit buffer to become empty
	while(!(SPI_status() & 0x20));
	SPI0->D = byte; //writing the character to data register

	//waiting for data to become available in the receive buffer
	while(!(SPI_status() & 0x80));
	temp = SPI0_D;   //clears SPRF

	return temp;
}
