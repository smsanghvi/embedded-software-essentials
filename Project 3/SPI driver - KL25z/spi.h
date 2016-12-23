/********************************************
*   File: spi.h
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
*   Description: Header file for SPI drivers
*   		
*		SPI_init
*		SPI_write_read_byte
*		SPI_status
*		SPI_flush
*
********************************************************/

#ifndef INCLUDES_SPI_H_
#define INCLUDES_SPI_H_


//setup routine to initialize the SPI module
void SPI_init(void);

//routine to transmit byte
uint8_t SPI_write_read_byte(unsigned char byte);

//routine to return status of SPI
uint8_t SPI_status(void);

//function to re-initialize the spi module
void SPI_flush(void);


#endif /* INCLUDES_SPI_H_ */
