/********************************************
*   File: adc.c
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
*   Description: Source file for ADC module
*                -ADC0_init
*                -ADC0_calibrate
*		         -mean filter
********************************************************/

#include "adc.h"
#include "stdint.h"

uint8_t adc_index = 0;
uint32_t avg_result;

//This function initializes the ADC module
void ADC0_init(void)
{
    SIM->SCGC5 |= 0x0800;       //clock to PORTC 
    PORTC->PCR[0] = 0;          //PTC0 analog input for temp sensor
    PORTC->PCR[2] = 0;          // PTC2 analog input for LDR
    SIM->SCGC6 |= 0x8000000;    //clock to ADC0
    ADC0->SC2 &= ~0x40;         // software trigger
    ADC0->CFG1 = 0x40 | 0x10 | 0x0C | 0x00;     // clock div by 4, long sample time, single ended 16 bit, bus clock
}

//function to calibrate the ADC module
void ADC0_calibrate(void)
{
	uint16_t calib = 0; // calibration variable
    ADC0_CFG1 |= (ADC_CFG1_MODE(3)  |  	// 16 bits mode
				  ADC_CFG1_ADICLK(1)|	// Input Bus Clock divided by 2 (20-25 MHz out of reset (FEI mode) / 2)
				  ADC_CFG1_ADIV(2)) ;	// Clock divide by 4 (2.5-3 MHz)

	ADC0_SC3 |= ADC_SC3_AVGE_MASK |		// Enable HW average
				ADC_SC3_AVGS(3)   |		// Set HW average of 32 samples
				ADC_SC3_CAL_MASK;		// Start calibration process

	while(ADC0_SC3 & ADC_SC3_CAL_MASK); // Wait for calibration to end

	if(ADC0_SC3 & ADC_SC3_CALF_MASK)	// Check for successful calibration

	calib += ADC0_CLPS + ADC0_CLP4 + ADC0_CLP3 + ADC0_CLP2 + ADC0_CLP1 + ADC0_CLP0;
	calib /= 2;
	calib |= 0x8000; 	// Set MSB
	ADC0_PG = calib;
	calib = 0;
	calib += ADC0_CLMS + ADC0_CLM4 + ADC0_CLM3 + ADC0_CLM2 + ADC0_CLM1 + ADC0_CLM0;
	calib /= 2;
	calib |= 0x8000;	// Set MSB
	ADC0_MG = calib;
}


//applies a mean filter on temperature samples
uint32_t mean_filter(uint32_t result){
	if(adc_index==0)
		avg_result=0;
	if(adc_index!=50){
		adc_index++;
		avg_result += result;
		return 0;
	}
	else if(adc_index==50){
		adc_index=0;
		avg_result = avg_result/50;
		return (uint16_t)avg_result;
	}
    return 0;
}


uint16_t ADC0_GetValue(uint8_t channel_number){
    uint16_t result_adc;
    ADC0->SC1[0] = channel_number;  // start conversion for defined channel number
	while(!(ADC0->SC1[0] & 0x80)){} // wait for COCO
	result_adc = ADC0->R[0];        // read conversion result and clear COCO flag
    return result_adc;
}
