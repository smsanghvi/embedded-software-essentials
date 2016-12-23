
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

#include "MKL25Z4.h"                    // Device header
#include "pwm.h"

#define PWM_OVERFLOW_VALUE 20970u


void RED_PWM_init(void){
    SIM_SCGC6 |= 0x04000000;            //Enable clock to TPM2
    SIM_SOPT2 |= 0x01000000;            //Select FLL clock to TPM2
    TPM2_SC = 0x00000000;               //Disable Timer for config
    TPM2_MOD = PWM_OVERFLOW_VALUE;      //Set PWM at 1kHz, considering the system clock is at 20.97 MHz
    TPM2_C0SC = 0x00000024;             //Select edge aligned inverted PWM
    TPM2_C0V = 10485;                   //Set PWM Duty Cycle to 50% default
    TPM2_SC = 0x0000008;                //Set divider as 0, select LPTPM to count on every clock, up counting mode
}

void GREEN_PWM_init(void){
    SIM_SCGC6 |= 0x04000000;            //Enable clock to TPM2
    SIM_SOPT2 |= 0x01000000;            //Select FLL clock to TPM2
    TPM2_SC = 0x00000000;               //Disable Timer for config
    TPM2_MOD = PWM_OVERFLOW_VALUE;      //Set PWM at 1kHz, considering the system clock is at 20.97 MHz
    TPM2_C1SC = 0x00000024;             //Select edge aligned inverted PWM
    TPM2_C1V = 10485;                   //Set PWM Duty Cycle to 50% default
    TPM2_SC = 0x0000008;                //Set divider as 0, select LPTPM to count on every clock, up counting mode
}

void BLUE_PWM_init(void){
    SIM_SCGC6 |= 0x01000000;            //Enable clock to TPM0
    SIM_SOPT2 |= 0x01000000;            //Select FLL clock to TPM0
    TPM0_SC = 0x00000000;               //Disable Timer for config
    TPM0_MOD = PWM_OVERFLOW_VALUE;      //Set PWM at 1kHz, considering the system clock is at 20.97 MHz
    TPM0_C1SC = 0x00000024;             //Select edge aligned inverted PWM
    TPM0_C1V = 10485;                   //Set PWM Duty Cycle to 50% default
    TPM0_SC = 0x0000008;                //Set divider as 0, select LPTPM to count on every clock, up counting mode
}

void PWM_init(void){
    RED_PWM_init();
    GREEN_PWM_init();
    BLUE_PWM_init();
}
