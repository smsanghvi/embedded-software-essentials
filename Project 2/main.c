/***************************************************


LEDs are Active Low in configuration. Thus setting the pin high will turn them off

*****************************************************/

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

//char c;                                 //Global variable to read or write UART0
char array[100] = {'\0'};               //Global array to read string UART0
//extern CircBuf_t rx_buffer;
//extern CircBuf_t tx_buffer;

int main () {
    int i = 0;
    initialize_Buffer(rx_buffer, 0);
    initialize_Buffer(tx_buffer, 1);
    __disable_irq();
    UART0_init();
    PWM_init();
    LED_init();
    __enable_irq();    
    
    UART0_WriteString("Testing concatenation strings");
    LOG_cat("\r\nThis is an integer number: ",32,200,8);
    LOG_0("\r\nTesting123,Serial Print Test,no params", 42);
    LOG_1("\r\nThis is an integer number: ",32,4096,16);
    LOG_1("\r\nThis is an integer number: ",32,123456,32);
    LOG_2("\r\nThis is a floating point number: ",35,1543.321, 32);
    
    while (1) {
        Control(remove_item(rx_buffer));

        for(i=0; i<10000;){
            i++;
        }
    }
}


