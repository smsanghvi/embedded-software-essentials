/******************************************************
*   File: main.c
*
*   Copyrights 2016  Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
**
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
**   Authors: Snehal Sanghvi and Shalin Shah
*   Date Edited: 11 Nov 2016
*
*   Description: Main function to test the functionality of the module
*               
*               LEDs are Active Low in configuration. Thus setting the pin high will turn them off
*
********************************************************/


#include "MKL25Z4.h"                    // Device header
#include "LED.h"
#include "uart.h"
#include "pwm.h"
#include "Circular_buffer.h"
#include "log.h"



struct CircBuf rx_buff;
CircBuf_t *rx_buffer = &rx_buff;

struct CircBuf tx_buff;
CircBuf_t *tx_buffer = &tx_buff;


char array[100] = {'\0'};               //Global array to read string UART0

int main () {
    int i = 0;
    initialize_Buffer(rx_buffer, 0);
    initialize_Buffer(tx_buffer, 1);
    __disable_irq();
    UART0_init();
    PWM_init();
    LED_init();
    __enable_irq();    
    
    UART0_WriteString("Board running");
    
    while (1) {
        Control(remove_item(rx_buffer));

        for(i=0; i<10000;){
            i++;
        }
    }
}


