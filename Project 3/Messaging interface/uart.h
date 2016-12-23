
/******************************************************
*   File: uart.c
*
*   Copyrights 2016 Shalin Shah and Snehal Sanghvi 
*   All Rights Reserved
*
*
*   The information contained herein is property of the Authors. 
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*
*   Authors: Snehal Sanghvi and Shalin Shah
*   Date Edited: 12 Oct 2016
*
*   Description: Header file for implementing UART functions
*               -UART0_init
*               -UART0_ReadChar
*               -UART0_ReadString
*               -UART0_WriteChar
*               -UART0_WriteString
*               -UART0_IRQHandler
*               -Control
*
********************************************************/

#ifndef _UART_H
#define _UART_H

#include "MKL25Z4.h"
#include "LED.h"
#include "pwm.h"
#include "Circular_buffer.h"
//#include "messaging.h"

//extern CircBuf_t *rx_buffer;
//extern CircBuf_t *tx_buffer;

void UART0_init(void);

char UART0_ReadChar(void);

void UART0_WriteChar(char byte);
    
void UART0_WriteString(char string[]);

void UART0_ReadString(char * str);

void Control(char a);

#endif
