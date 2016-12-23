/******************************************************
*   File: messaging.h
*
*   Copyrights 2016  Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*
*   The information contained herein is property of the Authors. 
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*
*   Authors: Shalin Shah and Snehal Sanghvi
*   Date Edited: 9 Nov 2016
*
*   Description: Header file for messaging using Circular buffer
*               -decode_message
*               -ack_nack
*               -checksum_validate
*
********************************************************/
#ifndef _MESSAGING_H
#define _MESSAGING_H

#include "stdint.h"
#include "LED.h"
#include "uart.h"
#include "Circular_buffer.h"
#include "log.h"


#define MAX_DATA_LENGTH 20

/*Enumerating commmands which can be sent or received*/
typedef enum CMDS_t{
    LED_TOGGLE = 1,
    BRIGHTNESS = 2,
    ACK = 3,
    NACK = 4
}CMDS;


/*Struct of the command to be sent or received*/
typedef struct Message_t{
    CMDS Command;
    uint8_t length;
    uint8_t data[MAX_DATA_LENGTH];
    uint16_t checksum;
}Message;

Message message;

/*Enumerating brightness values*/
typedef enum Brightness {		
	TEN_PERCENT = 1,
	TWENTY_PERCENT = 2,
	THIRTY_PERCENT = 3,
	FORTY_PERCENT = 4,
	FIFTY_PERCENT = 5,
	SIXTY_PERCENT = 6,
	SEVENTY_PERCENT = 7,
	EIGHTY_PERCENT = 8,
	NINTY_PERCENT = 9,
	HUNDRED_PERCENT = 10
}Brightness_t;


/*Enumerating the errors for messaging*/
typedef enum ERR_t {
	CHKSUM_FAIL = 1,
	CHKSUM_PASS = 2
}ERR;


/* Function to decode the incoming message packet*/
void decode_message (void);

/*Function to send acknowledge after the message is received*/
void ack_nack (CMDS packet);

/*Function to validate the checksum at the end of each message using addition algorithm*/
ERR checksum_validate (void);

#endif
