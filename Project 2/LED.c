
/******************************************************
*   File: LED.c
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
*   Description: Source file for implementing LED functions
*               -LED_init
*               -LED_SET_COLOR
*               -LED_SET_BRIGHTNESS
*               -LED_Control
*               
*
********************************************************/

#include "MKL25Z4.h"                    // Device header
#include "LED.h"
#include "uart.h"

//      COLOR       Value          R G B
#define OFF         0           // 0 0 0 
#define BLUE        1           // 0 0 1 
#define GREEN       2           // 0 1 0 
#define CYAN        3           // 0 1 1 
#define RED         4           // 1 0 0 
#define PINK        5           // 1 0 1 
#define ORANGE      6           // 1 1 0 
#define WHITE       7           // 1 1 1 

#define PWM_OVERFLOW_VALUE          20970u
#define RED_LED                     TPM2_C0V
#define GREEN_LED                   TPM2_C1V
#define BLUE_LED                    TPM0_C1V
    

int color = 0;                              //Integer to store the color number from 0 to 7 
int brightness_value = 5;                   //Integer to store brightness on a scale of 1 to 10
int brightness = 0.5*PWM_OVERFLOW_VALUE;    //Integer to store duty cycle for set brightness

void LED_init(void){
    SIM_SCGC5 |= 0x00000400;            // enable clock to Port B
    SIM_SCGC5 |= 0x00001000;            // enable clock to Port D
    PORTB_PCR18 = PORT_PCR_MUX(3);      // set PTB18(red LED) pin as PWM 
    PTB->PDDR |= 0x00040000;            // set PTB18 as output pin 
    RED_LED = 0;                        // turn off red LED
    PORTB_PCR19 = PORT_PCR_MUX(3);      // set PTB19(green LED) pin as PWM
    PTB->PDDR |= 0x00080000;            // set PTB19 as output pin
    //GREEN_LED = 0;                      // turn off green LED
    PORTD_PCR1 = PORT_PCR_MUX(4);       // make PTD1(blue LED) pin as PWM 
    PTD->PDDR |= 0x00000002;            // make PTD1 as output pin
    //BLUE_LED = 0;                       // turn off blue LED
}


void LED_SET_COLOR(int COLOR){
    color = COLOR;
    if(color<0)                         //if color goes beyond selection then rotate back
        color = 7;
    else if (color > 7)
        color = 0;
    switch(color){
        case 0: 
            RED_LED = 0;                // red LED off
            GREEN_LED = 0;              // green LED off            OFF
            BLUE_LED = 0;               // blue LED off
            break;
        
        case 1: 
            RED_LED = 0;                // red LED off
            GREEN_LED = 0;              // green LED off            BLUE
            BLUE_LED = brightness;      // blue LED on
            break;
        
        case 2: 
            RED_LED = 0;                // red LED off
            GREEN_LED = brightness;     // green LED on             GREEN
            BLUE_LED = 0;               // blue LED off
            break;
        
        case 3: 
            RED_LED = 0;                // red LED off
            GREEN_LED = brightness;     // green LED on             LIGHT BLUE
            BLUE_LED = brightness;      // blue LED on
            break;
        
        case 4:
            RED_LED = brightness;       // red LED on
            GREEN_LED = 0;              // green LED off            RED
            BLUE_LED = 0;               // blue LED off
            break;
                
        case 5: 
            RED_LED = brightness;       // red LED on
            GREEN_LED = 0;              // green LED off            PINK
            BLUE_LED = brightness;      // blue LED on
            break;
                
        case 6:
            RED_LED = brightness;       // red LED on
            GREEN_LED = brightness;     // green LED on             ORANGE
            BLUE_LED = 0;               // blue LED off
            break;
          
        case 7: 
            RED_LED = brightness;       // red LED on
            GREEN_LED = brightness;     // green LED on             WHITE
            BLUE_LED = brightness;      // blue LED on
            break;
                
        default: 
            RED_LED = brightness;       // red LED on
            GREEN_LED = brightness;     // green LED on             WHITE
            BLUE_LED = brightness;      // blue LED on
            break;
                   
    }
    
}


void LED_SET_BRIGHTNESS(int BRIGHTNESS){
    brightness_value=BRIGHTNESS;
    if(brightness_value<0)                          //Dont let brightness go above 10 or below 0
        brightness_value=0;
    else if(brightness_value>10)
        brightness_value=10;
    switch(brightness_value){
        case 0:
            brightness = 0.001*PWM_OVERFLOW_VALUE;      //Set brightness to 0 percent
            break;
        
        case 1:
            brightness = 0.1*PWM_OVERFLOW_VALUE;    //Set brightness to 10 percent
            break;
        
        case 2:
            brightness = 0.2*PWM_OVERFLOW_VALUE;    //Set brightness to 20 percent
            break;
        
        case 3:
            brightness = 0.3*PWM_OVERFLOW_VALUE;    //Set brightness to 30 percent
            break;
        
        case 4:
            brightness = 0.4*PWM_OVERFLOW_VALUE;    //Set brightness to 40 percent
            break;
        
        case 5:
            brightness = 0.5*PWM_OVERFLOW_VALUE;    //Set brightness to 50 percent
            break;
        
        case 6:
            brightness = 0.6*PWM_OVERFLOW_VALUE;    //Set brightness to 60 percent
            break;
        
        case 7:
            brightness = 0.7*PWM_OVERFLOW_VALUE;    //Set brightness to 70 percent
            break;
        
        case 8:
            brightness = 0.8*PWM_OVERFLOW_VALUE;    //Set brightness to 80 percent
            break;
        
        case 9:
            brightness = 0.9*PWM_OVERFLOW_VALUE;    //Set brightness to 90 percent
            break;
        
        case 10:
            brightness = 1*PWM_OVERFLOW_VALUE;    //Set brightness to 100 percent
            break;
        
        default:
            brightness = 0.5*PWM_OVERFLOW_VALUE;    //Set brightness to 50 percent
            break;
    }
}



void LED_Control(char value){
    switch(value){
        case 'w':                                           //Increase the brightness
            LED_SET_BRIGHTNESS(++brightness_value);
            LED_SET_COLOR(color);
            break;
       
        case 's':                                           //Decrease the brightness
            LED_SET_BRIGHTNESS(--brightness_value);
            LED_SET_COLOR(color);
            break;

        case 'a':                                           //Increment color value
            LED_SET_COLOR(++color);                 
            break;

        case 'd':                                           //Decrement color value
            LED_SET_COLOR(--color);
            break;
        
        case 'q':                                           //Set minimum brightness
            LED_SET_BRIGHTNESS(brightness_value = 0);
            LED_SET_COLOR(color);
            break;
       
        case 'e':                                           //Set maximum brightness
            LED_SET_BRIGHTNESS(brightness_value = 10);
            LED_SET_COLOR(color);
            break;

        case '0':
            LED_SET_COLOR(OFF);                             //Set the LED to OFF
            break;

        case '1':
            if(color == BLUE){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(BLUE);                            //Set the LED to BLUE
            break;
        
        case '2':
            if(color == GREEN){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(GREEN);                           //Set the LED to GREEN
            break;
        
        case '3':
            if(color == CYAN){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(CYAN);                            //Set the LED to CYAN
            break;
        
        case '4':
            if(color == RED){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(RED);                             //Set the LED to RED
            break;
        
        case '5':
            if(color == PINK){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(PINK);                            //Set the LED to PINK
            break;
        
        case '6':
            if(color == ORANGE){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(ORANGE);                          //Set the LED to ORANGE
            break;
        
        case '7':
            if(color == WHITE){
                LED_SET_COLOR(OFF);                         //Toggle on off for the same color
                break;
            }
            LED_SET_COLOR(WHITE);                           //Set the LED to WHITE
            break;
            
    }
}
