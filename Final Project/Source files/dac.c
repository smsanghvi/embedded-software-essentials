/*
 * dac.c
 *
 *  Created on: 27-Nov-2016
 *      Author: Snehal and Shalin
 */

#include "MKL25Z4.h"
#include <stdint.h>
#include "dac.h"

void DAC0_init(void) {
SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
PORTE_PCR30 |= PORT_PCR_MUX(0);
PTC->PDDR |= 0x40000000; //setting as output

SIM->SCGC6 |= 0x80000000; /* clock to DAC module */
DAC0->C1 = 0; /* disable the use of buffer */
DAC0->C0 = 0x80 | 0x20; /* enable DAC and use software trigger */
}

