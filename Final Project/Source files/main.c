/***************************************************


LEDs are Active Low in configuration. Thus setting the pin high will turn them off

*****************************************************/


#include "MKL25Z4.h"                    // Device header
#include "LED.h"
#include "uart.h"
#include "pwm.h"
#include "Circular_buffer.h"
#include "log.h"
#include "myrtc.h"



struct CircBuf rx_buff;
CircBuf_t *rx_buffer = &rx_buff;

struct CircBuf tx_buff;
CircBuf_t *tx_buffer = &tx_buff;


char array[100] = {'\0'};               //Global array to read string UART0

int main () {
    uint32_t a=0,b=1;
    int year;
    initialize_Buffer(rx_buffer, 0);
    initialize_Buffer(tx_buffer, 1);
    __disable_irq();
    UART0_init();
    PWM_init();
    LED_init();
    rtc_init();
    __enable_irq();    
    UART0_WriteString("Hello");
    while (1) {
        Control(remove_item(rx_buffer));
        a=get_unix_time();
        if(a>b){
            b=a;
            UART0_WriteChar(get_rtc_hours()+48);
            UART0_WriteChar(':');
            UART0_WriteChar(get_rtc_minutes()/10+48);
            UART0_WriteChar(get_rtc_minutes()%10+48);
            UART0_WriteChar(':');
            UART0_WriteChar(get_rtc_seconds()/10+48);
            UART0_WriteChar(get_rtc_seconds()%10+48);
            
            UART0_WriteChar('\t');
            year = get_rtc_year();
            UART0_WriteChar((year/10-152));
            UART0_WriteChar(year%10+48);
            UART0_WriteChar('-');
            UART0_WriteChar(get_rtc_month()/10+48);
            UART0_WriteChar(get_rtc_month()%10+48);
            UART0_WriteChar('-');
            UART0_WriteChar(get_rtc_day()+48);
            UART0_WriteChar('\n');
            UART0_WriteChar('\r');
        }
    }
}

void Control(char a){
    if(a == 'i'){
        UART0_WriteString("\n\rUser Echo mode");
        UART0_ReadString(array);
        UART0_WriteString("\n\rThe string entered is:");
        UART0_WriteString(array);
    }
    else if (a == 'm'){
        UART0_WriteString("\n\rMessaging mode");
        decode_message();
    }
    else 
        LED_Control(a);
}

