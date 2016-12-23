
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
*   Description: Source file for implementing UART functions
*               -UART0_init
*               -UART0_ReadChar
*               -UART0_ReadString
*               -UART0_WriteChar
*               -UART0_WriteString
*               -UART0_IRQHandler
*               -Control
*
********************************************************/

#include "MKL25Z4.h"                    // Device header
#include "uart.h"
#include "LED.h"
#include "pwm.h"
#include "Circular_buffer.h"

char byte;
extern char array[];

extern CircBuf_t *rx_buffer;
extern CircBuf_t *tx_buffer;


void UART0_init(void) {
    SIM_SCGC4 |= 0x00000400;            // enable clock for UART
    SIM_SOPT2 |= 0x04000000;            // use FLL output for UART
    UART0_C2 = 0x00000000;              // turn off UART0
    UART0_BDH = 0x00000000;             // Select BAUD rate as 57600
    UART0_BDL = 0x000000017;            // Select BAUD rate as 57600
    UART0_C4 = 0x0000000F;              // Over Sampling Ratio 16 
    UART0_C1 = 0x00000000;              // Select 8-bit mode 
    UART0_C2 = 0x0000002C;              // enable transmit and receive
    SIM_SCGC5 |= 0x00000200;            // enable clock for PORTA 
    NVIC->ISER[0] = 0x000001000;        // enable IRQ12 which is for UART0
    PORTA_PCR1 = 0x00000200;            // Select PTA1 as Receive pin
    PORTA_PCR2 = 0x00000200;            // Select PTA2 as Transmit pin
}


char UART0_ReadChar(void){
    while(!(UART0->S1 & 0x20)) {}       //wait for receive buffer full
    return UART0_D;                     //Return the read buffer
}


void UART0_WriteChar(char BYTE){
    int a = add_item(tx_buffer, BYTE);
    UART0_C2 |= 0x00000080;              //Enable transmit interrupt 
}

void UART0_WriteString(char string[]){
    char * str = string;
    while(*str)                         //Print the string till a NULL character ending
        UART0_WriteChar(*str++);
    UART0_WriteChar('\n');              //Go to newlinc and return carriage after writing the string
    UART0_WriteChar('\r');
}

void UART0_ReadString(char * str){
    __disable_irq();
    while((*(str++) = UART0_ReadChar() )!= '\r')              //Take input till a newline character is received
    *str = '\0';                                            //End the string with a NULL character
    __enable_irq();
}


void UART0_IRQHandler(){
    if(UART0->S1 & 0x20){                       //Interrupt caused by receiver
        add_item(rx_buffer, UART0_D);
    }
    else if (UART0->S1 & 0x80){                 //Interrupt caused by transmitter
        UART0_D = remove_item(tx_buffer);       //send a char
        UART0_C2 &= 0x7F;                       //Clear transmit receive interrupt flag
        while(!(UART0->S1 & 0x40));             //Wait till transmit receive complete
    }
}


void Control(char a){
    if(a == 'i'){
        UART0_WriteString("\n\rUser Echo mode");
        UART0_ReadString(array);
        UART0_WriteString("\n\rThe string entered is:");
        UART0_WriteString(array);
    }
    else 
        LED_Control(a);
}
