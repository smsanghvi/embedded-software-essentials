/***************************************************
*   File: LED.h
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
*   Description: Header file for implementing LED functions
*               -LED_init
*               -LED_SET_COLOR
*               -LED_SET_BRIGHTNESS
*               -LED_Control
*               
*
* LEDs are Active Low in configuration. Thus setting the pin high will turn them off
*
* The color variable selects color based on the charts below
*
* The brightness variable sets brightness on a scale of 1 to 10
*
*****************************************************/

#ifndef LED_H_
#define LED_H_


#include "MKL25Z4.h" 



//      COLOR       Value          R G B
#define OFF         0           // 0 0 0 
#define BLUE        1           // 0 0 1 
#define GREEN       2           // 0 1 0 
#define CYAN        3           // 0 1 1 
#define RED         4           // 1 0 0 
#define PINK        5           // 1 0 1 
#define ORANGE      6           // 1 1 0 
#define WHITE       7           // 1 1 1 


void LED_init(void);

void LED_SET_COLOR(int color);

void LED_SET_BRIGHTNESS(int BRIGHTNESS);

void LED_Control(char value);

#endif
