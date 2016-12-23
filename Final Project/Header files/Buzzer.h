/******************************************************
*   File: Buzzer.h
*
*   Copyrights 2016 Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*   Authors: Shalin Shah and Snehal Sanghvi 
*   Date Edited: 5 Dec 2016
*
*   Description: Header file for Buzzer
*               -Buzzer_PWM_init
*               -Buzzer_ON
*               -Buzzer_OFF
*
********************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_

#include "MKL25Z4.h"

void Buzzer_init(void);

void Buzzer_ON(void);

void Buzzer_OFF(void);
    
#endif
