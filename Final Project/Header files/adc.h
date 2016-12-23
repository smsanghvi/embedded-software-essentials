/********************************************
*   File: adc.h
*
*   Copyrights 2016 Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*   Author: Snehal Sanghvi and Shalin Shah
*   Date Edited: 26 Nov 2016
*
*   Description: Header file for ADC module
*                -ADC0_init
*                -ADC0_calibrate
*		         -mean filter
********************************************************/


#ifndef ADC_H_
#define ADC_H_

#include "MKL25Z4.h"
#include "stdint.h"

//This function initializes the ADC module
void ADC0_init(void);

//function to calibrate the ADC module
void ADC0_calibrate(void);

//applies a mean filter on temperature samples
uint32_t mean_filter(uint32_t result);

uint16_t ADC0_GetValue(uint8_t channel_number);

#endif 
