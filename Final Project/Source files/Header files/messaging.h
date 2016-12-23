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
*   Date Edited: 11 Nov 2016
*
*   Description: Source file for implementing Circular buffer functions
*               -is_Buffer_empty 
*               -is_Buffer_full
*               -add_item
*               -remove_item
*               -initialize_Buffer
*               -destroy
*
********************************************************/
#ifndef MESSAGING_H_
#define MESSAGING_H_

#include "stdint.h"
#include "LED.h"
#include "uart.h"
#include "Circular_buffer.h"
#include "log.h"


#define MAX_DATA_LENGTH 20

typedef enum CMDS_t{
    LED_TOGGLE = 1,
    BRIGHTNESS = 2,
    ACK = 3,
    NACK = 4,
    ECHO = 5
}CMDS;

typedef struct Message_t{
    CMDS Command;
    uint8_t length;
    uint8_t data[MAX_DATA_LENGTH];
    uint16_t checksum;
}Message;

Message message;

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


typedef enum ERR_t {
	CHKSUM_FAIL = 1,
	CHKSUM_PASS = 2
}ERR;


void decode_message (void);


void ack_nack (CMDS packet);


ERR checksum_validate (void);

#endif
