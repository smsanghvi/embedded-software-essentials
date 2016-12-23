
/******************************************************
*   File: pwm.h
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
*               -PWM_init
*               -RED_PWM__init
*               -GREEN_PWM__init
*               -BLUE_PWM__init
*
********************************************************/

#include "MKL25Z4.h"    

#define PWM_OVERFLOW_VALUE 20970u

void RED_PWM_init(void);

void GREEN_PWM_init(void);

void BLUE_PWM_init(void);

void PWM_init(void);
