/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MKL25Z4.h"
#include "eeprom.h"

int data=0;

//Initializes the eeprom module
void eeprom_init(void){

	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;	//Setting clock for portB
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;	//set clock for I2c module

	PORTB_PCR0 |= PORT_PCR_MUX(2);		//Select alternate function for portB pin 0 - SCL
	PORTB_PCR1 |= PORT_PCR_MUX(2);		//select alternate function for portB pin 1 - SDA

	I2C0_C1 = 0;						//Disable I2C
	I2C0_S |= I2C_S_IICIF_MASK;			//clear interrupt flag
	I2C0_F |= I2C_F_ICR(0x22);			//Divide bus clock of 21.972Mhz with 224 to get 100Khz
	I2C0_C1 |= I2C_C1_IICEN_MASK;		//enable I2C
}


// I2C write operation
void I2C_Write(uint8_t slave_addr,uint8_t eeprom_addr,uint8_t data){

	I2C0_C1 |= I2C_C1_TX_MASK;		//Set the direction of tranfer
	I2C0_C1 |= I2C_C1_MST_MASK;		//start I2C in master mode

	I2C0_D = (slave_addr << 1)|0;		//Start I2C data transfer for address
	while((I2C0_S & I2C_S_IICIF_MASK)==0); 	//wait for transfer to complete
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag
  
  I2C0_D = 0x00;		//Send higher byte of address
	while((I2C0_S & I2C_S_IICIF_MASK)==0);	 //wait for transfer to complete
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_D = eeprom_addr;		//Start I2C data transfer for address
	while((I2C0_S & I2C_S_IICIF_MASK)==0);	 //wait for transfer to complete
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_D = data;		//Start I2C data transfer for address
	while((I2C0_S & I2C_S_IICIF_MASK)==0); 	//wait for transfer to complete
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_C1 &= ~I2C_C1_MST_MASK;	//stop the I2C transfer

	i2c_delay(50);
}


//I2C read operation
uint8_t I2C_Read(uint8_t slave_addr,uint8_t mem_addr){
	unsigned char temp;
	uint8_t data;

	I2C0_C1 |= I2C_C1_TX_MASK;		//Set the direction of transfer
	I2C0_C1 |= I2C_C1_MST_MASK;		//start I2C in master mode

	I2C0_D = (slave_addr << 1)|0;		//Start I2C data transfer for address
	while((I2C0_S & I2C_S_IICIF_MASK)==0);
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

  I2C0_D = 0x00;		//Send higher byte of address
	while((I2C0_S & I2C_S_IICIF_MASK)==0);	 //wait for transfer to complete
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_D = mem_addr;		//send memory address
	while((I2C0_S & I2C_S_IICIF_MASK)==0);
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_C1 |= I2C_C1_RSTA_MASK;	//send repeat start

	I2C0_D = (slave_addr<<1)|1;		//Start I2C data transfer for address in read mode
	while((I2C0_S & I2C_S_IICIF_MASK)==0);
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_C1 &= ~(I2C_C1_TX_MASK);	  //enter receiver mode
	I2C0_C1 |= I2C_C1_TXAK_MASK;		//disable ack

	temp = I2C0_D;			//read temp byte to initiate transfer
	while((I2C0_S & I2C_S_IICIF_MASK)==0);
	I2C0_S |= I2C_S_IICIF_MASK;		//clear interrupt flag

	I2C0_C1 &= ~I2C_C1_MST_MASK;  //i2c stop condition

	data = I2C0_D;				//read the data
	i2c_delay(50);
	return data;
}


//setting a delay
void i2c_delay(int number)
{
	int i;
	for(i=0; i<number; i++)
	{
		__asm("nop");
	};
}
