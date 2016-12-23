/********************************************
*   File: nordic.c
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
*   Description: Source file for testing the NORDIC NRF24L01+ chip
*   			
*   		NORDIC_Write
*		NORDIC_Config
*		CE_High
*		CE_Low
*		NORDIC_Write_Multiple
*		NORDIC_Get_Status
*		NORDIC_RF_Setup
*		NORDIC_TX_Payload
*		NORDIC_TX_Flush
*		NORDIC_RX_Flush
*
********************************************************/
#include "MKL25Z4.h"
#include "spi.h"
#include "nRF24L01.h"
#include <stdint.h>
#include "LED.h"
#include "log.h"

uint8_t val=0;

#define RF_CHANNEL 2	// channel 2 is also the default channel
#define PAYLOAD_SIZE 1	// payload size
uint8_t flag=0;

uint8_t payload[PAYLOAD_SIZE];  // payload data
uint8_t TX_address[5] = {0XF0, 0XF0, 0XF0, 0XF0, 0XE1};
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// Function to write a byte to the register
void NORDIC_Write(uint8_t register1, uint8_t value){
	NORDIC_CS_ENABLE();
	SPI_write_read_byte(NORDIC_W_REGISTER|register1); //sending the write command
	SPI_write_read_byte(value);	    				  //sending the value
	NORDIC_CS_DISABLE();
}


//Function to read register
uint8_t NORDIC_Read(uint8_t register1){
	uint8_t a,b;
	NORDIC_CS_ENABLE();
	a = SPI_write_read_byte(NORDIC_R_REGISTER|register1);
	b = SPI_write_read_byte(NORDIC_NOP);	// to read the value, NOP command or 0xFF has to be sent
	NORDIC_CS_DISABLE();
	return b;
}


// Setting up RX payload
uint8_t NORDIC_ReadPayload(void *loc, uint8_t len){
	  uint8_t status;
	  uint8_t* current = (uint8_t*)(loc);

	  uint8_t data_len = (len < PAYLOAD_SIZE)?len:PAYLOAD_SIZE;	//take the minimum

	  status = SPI_write_read_byte( NORDIC_R_RX_PAYLOAD );
	  while ( data_len-- )
	    *current++ = SPI_write_read_byte(0xff);

	  return status;
}


// Configuring parameters like 1 CRC byte, enable CRC, power up and set to transmit mode
void NORDIC_ConfigTX(void){
	uint8_t status = 0;
	NORDIC_Write(NORDIC_CONFIG, NORDIC_CONFIG_EN_CRC(1) | NORDIC_CONFIG_CRCO_1 | NORDIC_CONFIG_POWER_UP | NORDIC_CONFIG_PRIM_TX);
	status = NORDIC_Read(NORDIC_CONFIG);
	if(status==NORDIC_CONFIG_EN_CRC(1) | NORDIC_CONFIG_CRCO_1 | NORDIC_CONFIG_POWER_UP | NORDIC_CONFIG_PRIM_TX)
		LOG_0("\n\rNordic successfully configured in TX mode!",44);
	else
		LOG_0("\n\rNordic configuration unsuccessful in TX mode!",47);
}


void NORDIC_ConfigRX(void){
	uint8_t status = 0;
	NORDIC_Write(NORDIC_CONFIG, NORDIC_CONFIG_EN_CRC(1) | NORDIC_CONFIG_CRCO_1 | NORDIC_CONFIG_POWER_UP | NORDIC_CONFIG_PRIM_RX);
	status = NORDIC_Read(NORDIC_CONFIG);
	if(status==NORDIC_CONFIG_EN_CRC(1) | NORDIC_CONFIG_CRCO_1 | NORDIC_CONFIG_POWER_UP | NORDIC_CONFIG_PRIM_RX)
		LOG_0("\n\rNordic successfully configured in RX mode!",44);
	else
		LOG_0("\n\rNordic configuration unsuccessful in RX mode!",47);
}


// setting the clock enable (CE) pin high to transmit data
void CE_High(void){
	//enabling port C
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	//Select Alternate function 1 (or gpio pin) for pin 3
	PORTC_PCR3 |= PORT_PCR_MUX(1);
	//setting direction as output
	PTC->PDDR |= 0x08;
	//making the CE pin high (active high)
	PTC->PSOR = 0x08;
}


// setting the clock enable (CE) pin low
void CE_Low(void){
	//enabling port C
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	//Select Alternate function 1 (or gpio pin) for pin 3
	PORTC_PCR3 |= PORT_PCR_MUX(1);
	//setting direction as output
	PTC->PDDR |= 0x08;
	//making the CE pin low (active high)
	PTC->PCOR = 0x08;
}


// Function to write multiple bytes to the bus - needed if transmit address is to be sent
void NORDIC_Write_Multiple(uint8_t register1, uint8_t *loc, uint8_t size) {
  uint8_t i;
  NORDIC_CS_ENABLE();    //clearing the ss bit to drive it low
  SPI_write_read_byte(NORDIC_W_REGISTER|register1);

  for(i=0; i<size; i++) {
    SPI_write_read_byte(loc[i]);
  }
  NORDIC_CS_DISABLE();	//setting the ss bit high
}


// This function reads multiple bytes
void NORDIC_Read_Multiple(uint8_t register1, uint8_t size){
	uint8_t i=0;
	uint8_t val=0;
	NORDIC_CS_ENABLE();

	SPI_write_read_byte(NORDIC_R_REGISTER|register1);

	val = SPI_write_read_byte(NORDIC_NOP);
	LOG_1("\n\rByte 0 is",11,val,8);

	val = SPI_write_read_byte(NORDIC_NOP);
	LOG_1("\n\rByte 1 is",11,val,8);

	val = SPI_write_read_byte(NORDIC_NOP);
	LOG_1("\n\rByte 2 is",11,val,8);

	val = SPI_write_read_byte(NORDIC_NOP);
	LOG_1("\n\rByte 3 is",11,val,8);

	val = SPI_write_read_byte(NORDIC_NOP);
	LOG_1("\n\rByte 4 is",11,val,8);

	NORDIC_CS_DISABLE();	//setting the ss bit high
}


// Reading the value of the STATUS register by sending NOP command
uint8_t NORDIC_Get_Status(void){
	uint8_t read_value;
	NORDIC_CS_ENABLE();
	read_value = SPI_write_read_byte(NORDIC_NOP);
	NORDIC_CS_DISABLE();
	return read_value;
}


// function to set the RF parameters required for transmission.
void NORDIC_RF_Setup(void){
	uint8_t received_val;

	//Setting a data rate of 1 Mbps, transmit power of 0dBm and the low power gain
	NORDIC_Write(NORDIC_RF_SETUP, NORDIC_RF_DR_ONE | NORDIC_RF_PWR_ZERO | NORDIC_LNA_HCURR(1));

	//performing sanity check!
	received_val =  NORDIC_Read(NORDIC_RF_SETUP);
	if((received_val && NORDIC_RF_PWR_ZERO)!=0)
		LOG_0("\n\rTransmit power is 0dBm.",27);
	else
		LOG_0("\n\rError in setting up RF parameters",35);

	//setting the RF channel no. 2
	NORDIC_Write(NORDIC_RF_CH, RF_CHANNEL);

	//setting 1 byte as payload size for the receive pipe 0
	NORDIC_Write(NORDIC_RX_PW_P0, PAYLOAD_SIZE);

	// setting the number of address bytes
	NORDIC_Write(NORDIC_RX_ADDR_P0, 0x05);

}


// sending payload to transmit buffer to send it
void NORDIC_TX_Payload(uint8_t *payload, uint8_t Payload_size) {
	uint16_t count = 0;

	//flushes the old data
	NORDIC_TX_Flush();

	// writing the payload data
	NORDIC_Write_Multiple(NORDIC_W_TX_PAYLOAD, payload, Payload_size);

	//setting CE pin to start transmission
	CE_High();

	//setting some delay for which signal is high - 15ms
	delay_100us(150);

	//stop transmission
	CE_Low();
}



// Reset the given mask of status bits
void NORDIC_ResetStatusIRQ(uint8_t flags){
  delay_100us(10);
  PTC->PCOR = 0x10;
  delay_100us(10);
  NORDIC_Write(NORDIC_STATUS, flags); /* reset all IRQ in status register */
  delay_100us(10);
  PTC->PSOR = 0x10;
  delay_100us(10);
}


// Function to flush TX
void NORDIC_TX_Flush(void){
	SPI_write_read_byte(NORDIC_FLUSH_TX);
}


// Function to flush RX
void NORDIC_RX_Flush(void){
	SPI_write_read_byte(NORDIC_FLUSH_RX);
}


//setting some delay
void delay_100us(volatile unsigned char n)
{
unsigned char i;
while(n--)
for(i=0;i<32;i++);
}


// Function to set the transmit address
void NORDIC_Set_TX_Address(){
	uint8_t i;
	uint8_t read_val;
	uint8_t val_val;

	// Setting the device address
	const uint8_t TX_ADDR[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

	// enable auto-ack for pipe 0
	NORDIC_Write(NORDIC_EN_AA, 0x00 | NORDIC_ENAA_P0(1));

	// enable data pipe 0
	NORDIC_Write(NORDIC_EN_RXADDR, 0x00 | NORDIC_ERX_P0(1));

	//setting the tx address width of 5 bytes in the Setup_AW (setup address width register)
	NORDIC_Write(NORDIC_SETUP_AW, NORDIC_AW_FIVE);

	val_val = NORDIC_Read(NORDIC_SETUP_AW);
	if(val_val==3)
		LOG_0("Address width of 5 validated",28);

	//writing the transmit 5 byte address
	NORDIC_Write_Multiple(NORDIC_TX_ADDR, (uint8_t*)TX_ADDR, sizeof(TX_ADDR));

	// Reading the same bytes to verify that the address was sent correctly
	NORDIC_Read_Multiple(NORDIC_TX_ADDR,5);

}


//creating test function for trying the extra credit part!
void test_function(){

	// Keeping the device in TX mode
	NORDIC_Write(NORDIC_CONFIG, NORDIC_CONFIG_PRIM_TX|NORDIC_CONFIG_POWER_UP);
	// enable auto-ack for pipe 0
	NORDIC_Write(NORDIC_EN_AA, 0x00 | NORDIC_ENAA_P0(1));
	// enable data pipe 1
	NORDIC_Write(NORDIC_EN_RXADDR, 0x00 | NORDIC_ERX_P1(1));
	//setting the RF channel no. 4c
	NORDIC_Write(NORDIC_RF_CH, RF_CHANNEL);
	//setting 1 byte as payload size for the receive pipe 0
	NORDIC_Write(NORDIC_RX_PW_P1, PAYLOAD_SIZE);
	//setting the tx address width of 5 bytes in the Setup_AW (setup address width register)
	NORDIC_Write(NORDIC_SETUP_AW, NORDIC_AW_FIVE);
	//Clocking in the TX address
	NORDIC_Write_Multiple(NORDIC_TX_ADDR, (uint8_t *)TX_ADDR, 5);

	//Send the payload data
	NORDIC_CS_ENABLE();
	SPI_write_read_byte(NORDIC_W_TX_PAYLOAD); //sending the write command
	SPI_write_read_byte(0x01);	    		  //sending the payload , in this case 1 byte -> 0x01
	NORDIC_CS_DISABLE();

	//setting rx address to receive ack, this should be for data pipe 0, and same as tx addr
	NORDIC_Write_Multiple(NORDIC_RX_ADDR_P0, (uint8_t *)TX_ADDR, 5);
	//setting the RF parameters - data rate of 1mbps and power of 0dBm
	NORDIC_Write(NORDIC_RF_SETUP, NORDIC_RF_DR_ONE | NORDIC_RF_PWR_ZERO);

	//High pulse on CE
	CE_High();
	//Setting a delay of atleast 15us
	delay(20);
	//transmission complete
	CE_Low();

}


void test_func_2(void){
	delay(50);
	CE_Low();
	NORDIC_CS_DISABLE();
	NORDIC_Write(NORDIC_RF_SETUP, NORDIC_RF_DR_ONE | NORDIC_RF_PWR_ZERO);
	NORDIC_Write(NORDIC_RX_PW_P0, 1); //CHECK
	NORDIC_Write(NORDIC_RF_CH, RF_CHANNEL);

	NORDIC_Write_Multiple(NORDIC_RX_ADDR_P0, (uint8_t *)TX_ADDR, 5);
	NORDIC_Write_Multiple(NORDIC_TX_ADDR, (uint8_t *)TX_ADDR, 5);

	NORDIC_Write(NORDIC_EN_RXADDR, 0x00 | NORDIC_ERX_P0(1));

	NORDIC_ResetStatusIRQ(NORDIC_STATUS_RX_DR(1)|NORDIC_STATUS_TX_DS(1)|NORDIC_STATUS_MAX_RT(1));

	NORDIC_Write(NORDIC_EN_AA, 0x00 | NORDIC_ENAA_P0(1));

	NORDIC_Write(NORDIC_SETUP_RETR, NORDIC_ARD_750|NORDIC_ARC_15);

	NORDIC_Write(NORDIC_CONFIG, NORDIC_CONFIG_PRIM_TX|NORDIC_CONFIG_POWER_UP|NORDIC_CONFIG_EN_CRC(1)|NORDIC_CONFIG_CRCO_2);

	CE_Low();

	LOG_0("\n\rFilling payload",18);

	NORDIC_TX_Flush();
	NORDIC_CS_ENABLE();
	SPI_write_read_byte(NORDIC_W_TX_PAYLOAD); //sending the write command
	SPI_write_read_byte(0x01);	    		  //sending the payload , in this case 1 byte -> 0x01
	NORDIC_CS_DISABLE();

	CE_High();
	delay(50);
	CE_Low();

}
