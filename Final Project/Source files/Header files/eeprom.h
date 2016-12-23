/*
 * eeprom.h
 *
 *  Created on: 06-Dec-2016
 *      Author: Snehal Sanghvi and Shalin Shah
 */

#ifndef INCLUDES_EEPROM_H_
#define INCLUDES_EEPROM_H_

#define slave_address 0x50	//A2,A1,A0 are connected to ground

//Initializes the eeprom module
void eeprom_init(void);

// I2C write operation
void I2C_Write(uint8_t slave_addr,uint8_t eeprom_addr,uint8_t data);

//I2C read operation
uint8_t I2C_Read(uint8_t slave_addr,uint8_t mem_addr);

//setting a delay
void i2c_delay(int number);

#endif /* INCLUDES_EEPROM_H_ */
