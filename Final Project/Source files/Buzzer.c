/******************************************************
*   File: Buzzer.c
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
*   Description: Source file for Buzzer
*               -Buzzer_PWM_init
*               -Buzzer_ON
*               -Buzzer_OFF
*
********************************************************/

#include "Buzzer.h"


void Buzzer_init(void){
    SIM_SCGC6 |= 0x01000000;            //Enable clock to TPM0
    SIM_SOPT2 |= 0x01000000;            //Select FLL clock to TPM0
    TPM0_SC = 0x00000000;               //Disable Timer for config
    TPM0_MOD = 20970u;      //Set PWM at 1kHz, considering the system clock is at 20.97 MHz
    TPM0_C3SC = 0x00000024;             //Select edge aligned inverted PWM
    TPM0_C3V = 0;                       //Set PWM Duty Cycle to 50% default
    TPM0_SC = 0x0000008;                //Set divider as 0, select LPTPM to count on every clock, up counting mode
    SIM_SCGC5 |= 0x00002000;            // enable clock to Port E
    PORTE_PCR30 = PORT_PCR_MUX(3);      // set PTE30 pin as Buzzer
    PTE->PDDR |= 0x40000000;            // set PTE30 as output pin 
}


void Buzzer_ON(void){
    TPM0_C3V = 10485;                   //Set PWM Duty Cycle to 50% default
}


void Buzzer_OFF(void){
    TPM0_C3V = 0;                       //Set PWM Duty Cycle to 50% default
}
