/********************************************
*   File: dma.c
*
*   Copyrights 2016 Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*
*   Author: Snehal Sanghvi and Shalin Shah
*   Date Edited: 9 Nov 2016
*
*   Description: Source file for testing the timing of various functions
*   			on the Freedom Freescale KL25z and transferring data using DMA
*   			-memzero_dma
*   			-memmove_dma
*               -UART0_init
*               -UART0_WriteChar
*               -UART0_WriteString
*               -reverse_string
*               -my_itoa
*               -my_ftoa
*               -LOG_0
*               -LOG_1
*               -LOG_2
*               -start_profiling
*               -profiler_stop
*
********************************************************/

#include "MKL25Z4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dma.h"

#define LENGTH_MEMZERO	10		// change this value to change number of bytes to transfer using DMA for memzero function
#define LENGTH_MEMMOVE	10		// change this value to change number of bytes to transfer using DMA for memmove function

// variable for counting number of overflows in timer
uint16_t overflow_counts=0;

//arrays for memzero_dma function
#ifdef EIGHT_BIT_TRANSFER
	uint8_t zeros_array[1] = {'0'};
	uint8_t array[LENGTH_MEMZERO];
	uint8_t *src_memzero = zeros_array;
	uint8_t *dst_memzero = array;
#elif THIRTY_TWO_BIT_TRANSFER
	uint32_t zeros_array[1] = {'0'};
	uint32_t array[LENGTH_MEMZER0/4];
	uint32_t *src_memzero = zeros_array;
	uint32_t *dst_memzero = array;
#endif


//arrays for memmove_dma function
#ifdef EIGHT_BIT_TRANSFER
	uint8_t src_array[LENGTH_MEMMOVE] = {'0'};
	uint8_t dest_array[LENGTH_MEMMOVE] = {'0'};
	uint8_t *src_memmove = src_array;
	uint8_t *dst_memmove = dest_array;
#elif THIRTY_TWO_BIT_TRANSFER
	uint32_t src_array[LENGTH_MEMMOVE / 4] = {'0'};
	uint32_t dest_array[LENGTH_MEMMOVE / 4] = {'0'};
	uint32_t *src_memmove = src_array;
	uint32_t *dst_memmove = dest_array;
#endif



//function to write zeros to the given memory location and also profile it
void memzero_dma(uint16_t length){
			//Enable clock gating to dma and dmamux
			SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
			SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

			// Disable DMA Mux channel first
			DMAMUX0_CHCFG0 = 0x00;

			// Clear pending errors and/or the done bit
			if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
				| ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
				| ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
				| ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
			DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;

			// Set Source Address
			DMA_SAR0 = (uint32_t)src_memzero;

			#ifdef EIGHT_BIT_TRANSFER
				DMA_DSR_BCR0 |= LENGTH_MEMZERO;

				// Set DMA
				DMA_DCR0 |= ( DMA_DCR_EINT_MASK			// Int. enable... used if FREEDOM macro is set
							| DMA_DCR_AA_MASK			// Enable auto-aligned
							| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
							| DMA_DCR_SSIZE(1)			//Source size is 8-bit
							| DMA_DCR_DINC_MASK			//Destination address increments
							| DMA_DCR_DSIZE(1)			//Destination size is 8-bit
							| DMA_DCR_D_REQ_MASK		//DMA request is cleared
							);

				// Set destination address
				DMA_DAR0 = (uint32_t)dst_memzero;

				// Enables the DMA channel and select the DMA Channel Source
				DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);

				NVIC_EnableIRQ(DMA0_IRQn);
				__enable_irq();

				TPM1_SC |= TPM_SC_CMOD(1);		// Start Counter
				DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer

			#elif THIRTY_TWO_BIT_TRANSFER

				// if length is divisible by 4
				if(LENGTH_MEMZERO % 4 == 0){
					// Set BCR to know how many bytes to transfer
					DMA_DSR_BCR0 |= LENGTH_MEMZERO;

					// Set DMA
					DMA_DCR0 |= ( DMA_DCR_EINT_MASK
								| DMA_DCR_AA_MASK			// Enable auto-aligned
								| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
								| DMA_DCR_SSIZE(0)			//Source size is 32-bit
								| DMA_DCR_DINC_MASK			//Destination address increments
								| DMA_DCR_DSIZE(0)			//Destination size is 32-bit
								| DMA_DCR_D_REQ_MASK		//DMA request is cleared
								);

					// Set destination address
					DMA_DAR0 = (uint32_t)dst_memzero;

					// Enables the DMA channel and select the DMA Channel Source
					DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);

					NVIC_EnableIRQ(DMA0_IRQn);
					__enable_irq();

					TPM1_SC |= TPM_SC_CMOD(1);		// Start Counter
					DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer
				}


				//if not divisible by 4
				else{
					// Set BCR to know how many bytes to transfer by 32 bit transfers
					DMA_DSR_BCR0 |= (LENGTH_MEMZERO - LENGTH_MEMZERO % 4);

					// Set DMA
					DMA_DCR0 |= ( DMA_DCR_AA_MASK			// Enable auto-aligned
								| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
								| DMA_DCR_SSIZE(0)			//Source size is 32-bit
								| DMA_DCR_DINC_MASK			//Destination address increments
								| DMA_DCR_DSIZE(0)			//Destination size is 32-bit
								| DMA_DCR_D_REQ_MASK		//DMA request is cleared
								);

					// Set destination address
					DMA_DAR0 = (uint32_t)dst_memzero;

					// Enables the DMA channel and select the DMA Channel Source
					DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);

					TPM1_SC |= TPM_SC_CMOD(1);			// Start Counter
					DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer

					DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;

					DMAMUX0_CHCFG0 = 0x00;

					//sending the remaining length as 8 bit transfer
					DMA_DSR_BCR0 |= LENGTH_MEMZERO % 4;

					DMA_DCR0 |= ( DMA_DCR_EINT_MASK
								| DMA_DCR_AA_MASK			// Enable auto-aligned
								| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
								| DMA_DCR_SSIZE(1)			//Source size is 8-bit
								| DMA_DCR_DINC_MASK			//Destination address increments
								| DMA_DCR_DSIZE(1)			//Destination size is 8-bit
								| DMA_DCR_D_REQ_MASK		//DMA request is cleared
		    			    	);

					DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);
					NVIC_EnableIRQ(DMA0_IRQn);
					__enable_irq();

					DMA_DCR0 |= DMA_DCR_START_MASK;
				}
			#endif

}



//function to move values from one memory location to another and also profile it
void memmove_dma(uint16_t length){
		uint16_t i;

		//Enable clock gating to dma and dmamux
		SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
		SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

		// Disable DMA Mux channel first
		DMAMUX0_CHCFG0 = 0x00;

		// Clear pending errors and/or the done bit
		if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
			| ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
			| ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
			| ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
		DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;


		// Set Source Address
		DMA_SAR0 = (uint32_t)src_memmove;

		// for eight bit transfers
		#ifdef EIGHT_BIT_TRANSFER
				// Set BCR to know how many bytes to transfer
				DMA_DSR_BCR0 |= LENGTH_MEMMOVE;

				// Set DMA
				DMA_DCR0 |= ( DMA_DCR_EINT_MASK			// Int. enable... used if FREEDOM macro is set
							| DMA_DCR_AA_MASK			// Enable auto-aligned
							| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
							| DMA_DCR_SSIZE(1)			//Source size is 8-bit
							| DMA_DCR_SINC_MASK			//Increment source address after a succesful transfer
							| DMA_DCR_DINC_MASK			//Destination address increments
							| DMA_DCR_DSIZE(1)			//Destination size is 8-bit
							| DMA_DCR_D_REQ_MASK		//DMA request is cleared
							);

				// Set destination address
				DMA_DAR0 = (uint32_t)dst_memmove;

				// Enables the DMA channel and select the DMA Channel Source
				DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);

				NVIC_EnableIRQ(DMA0_IRQn);
			    __enable_irq();

			    TPM1_SC |= TPM_SC_CMOD(1);			// Start Counter
			    DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer

		// for thirty-two bit transfers
		#elif THIRTY_TWO_BIT_TRANSFER

			// if length is divisible by 4
			if(LENGTH_MEMMOVE % 4 == 0){
				// Set BCR to know how many bytes to transfer
				DMA_DSR_BCR0 |= LENGTH_MEMMOVE;

				// Set DMA
				DMA_DCR0 |= ( DMA_DCR_EINT_MASK			// Int. enable... used if FREEDOM macro is set
							| DMA_DCR_AA_MASK			// Enable auto-aligned
							| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
							| DMA_DCR_SSIZE(0)			//Source size is 8-bit
							| DMA_DCR_SINC_MASK			//Increment source address after a succesful transfer
							| DMA_DCR_DINC_MASK			//Destination address increments
							| DMA_DCR_DSIZE(0)			//Destination size is 8-bit
							| DMA_DCR_D_REQ_MASK		//DMA request is cleared
							);

				// Set destination address
				DMA_DAR0 = (uint32_t)dst_memmove;

				// Enables the DMA channel and select the DMA Channel Source
				DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);

				NVIC_EnableIRQ(DMA0_IRQn);
				__enable_irq();

				TPM1_SC |= TPM_SC_CMOD(1);		// Start Counter
				DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer
			}

			//if length is not multiple of 4
			else{
				// Set BCR to know how many bytes to transfer
				DMA_DSR_BCR0 |= LENGTH_MEMMOVE - (LENGTH_MEMMOVE % 4);

				// Set DMA
				DMA_DCR0 |= ( DMA_DCR_AA_MASK			// Enable auto-aligned
							| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
							| DMA_DCR_SSIZE(0)			//Source size is 8-bit
							| DMA_DCR_SINC_MASK			//Increment source address after a succesful transfer
							| DMA_DCR_DINC_MASK			//Destination address increments
							| DMA_DCR_DSIZE(0)			//Destination size is 8-bit
							| DMA_DCR_D_REQ_MASK		//DMA request is cleared
							);

				// Set destination address
				DMA_DAR0 = (uint32_t)dst_memmove;

				// Enables the DMA channel and select the DMA Channel Source
				DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);

				TPM1_SC |= TPM_SC_CMOD(1);			// Start Counter
				DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer

				DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;

				DMAMUX0_CHCFG0 |= 0;

				DMA_DSR_BCR0 |= LENGTH_MEMMOVE % 4;

				// Set DMA
				DMA_DCR0 |= ( DMA_DCR_EINT_MASK
							| DMA_DCR_AA_MASK			// Enable auto-aligned
							| DMA_DCR_EADREQ_MASK		//Enable Async. DMA Requests
							| DMA_DCR_SSIZE(1)			//Source size is 8-bit
							| DMA_DCR_SINC_MASK			//Increment source address after a succesful transfer
							| DMA_DCR_DINC_MASK			//Destination address increments
							| DMA_DCR_DSIZE(1)			//Destination size is 8-bit
							| DMA_DCR_D_REQ_MASK		//DMA request is cleared
							);

				// Set destination address
				DMA_DAR0 = (uint32_t)dst_memmove;

				// Enables the DMA channel and select the DMA Channel Source
				DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);
				NVIC_EnableIRQ(DMA0_IRQn);
				__enable_irq();

				DMA_DCR0 |= DMA_DCR_START_MASK;		//start dma transfer

			}
		#endif
}



// IRQ handler for the DMA module
void DMA0_IRQHandler(void)
{

	//enables the done flag in DSR_BCR
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;

}



//initializing UART
void UART0_init(void) {
     SIM_SCGC4 |= 0x00000400;            // enable clock for UART
     SIM_SOPT2 |= 0x04000000;            // use FLL output for UART
     UART0_C2 = 0x00000000;              // turn off UART0
     UART0_BDH = 0x00000000;             // Select BAUD rate as 115200
     UART0_BDL = 0x00000017;             // Select BAUD rate as 115200
     UART0_C4 = 0x0000000F;              // Over Sampling Ratio 16
     UART0_C1 = 0x00000000;              // Select 8-bit mode
     UART0_C2 = 0x0000000C;              // enable transmit and receive
     SIM_SCGC5 |= 0x00000200;            // enable clock for PORTA
     PORTA_PCR1 = 0x00000200;            // Select PTA1 as Receive pin
     PORTA_PCR2 = 0x00000200;            // Select PTA2 as Transmit pin
 }



// function to print out a character using UART
 void UART0_WriteChar(char byte){
      while(!(UART0->S1 & 0xC0)){}        // Wait till transmit buffer empty and Transmit complete
      UART0_D = byte;                     // send a char
          while(!(UART0->S1 & 0x40)){}        // Wait till transmit buffer empty and Transmit complete
  }



 // function to print out a string using UART
 void UART0_WriteString(char string[]){
     char * str = string;
     while(*str)                         //Print the string till a NULL character ending
         UART0_WriteChar(*str++);
     UART0_WriteChar('\n');              //Go to newlinc and return carriage after writing the string
     UART0_WriteChar('\r');
 }



 // reverses a string 'arr' of length 'length'
 void reverse_string(char *arr, int length)
 {
 	int i=0, temp;
 	while (i<length-1)
 	{
 		temp = *(arr+i);
 		*(arr + i) = *(arr + length - 1 - i);
 		*(arr + length - 1 - i) = temp;
 		i++;
 	}
 }



 // integer to ascii converter
 int my_itoa(char arr[], int number, int no_of_digits)
 {
 	int i = 0;
 	int digit = 0;

     //taking care of negative digits
 	if(number<0)
 	{	digit = -1;
 		number = -number;
 	}

     //dividing the number till it is not 0
 	while (number!=0)
 	{
 		*(arr+i) = (number % 10) + '0';
 		i++;
 		number = number/10;
 	}

 	if(digit==-1){
 		*(arr+i) = '-';
 		i++;
 	}

 	//performing padding manipulations so that the zeros after decimal point are not lost
 	while (i < no_of_digits)
 	{	*(arr+i) = '0';
 		 i++;
 	}

     //reversing string so as to print it in correct order
 	reverse_string(arr, i);
 	*(arr+i) = '\0';
 	return i;
 }



 // floating point to ascii converter
 void my_ftoa(char *arr, float number, int after_decimal_point)
 {
 	// Extract integer part
 	int integer_part = (int)number;

 	// Extract floating part
 	float floating_part = number - (float)integer_part;

 	// convert integer part to string
 	int i = my_itoa(arr, integer_part, 0);


 	if (after_decimal_point != 0)
 	{
 		*(arr+i) = '.';

 		// manipulations to implement the power function in math.h
         int dp = after_decimal_point;
         int a = 1;
         while(dp!=0){
             a = a*10;
             dp--;
         }

 		floating_part = floating_part * a;
 		my_itoa(arr + i + 1, (int)floating_part, after_decimal_point);

 	}
 }



 // implementing LOG_0 function, which is used to write data bytes onto the terminal
 void LOG_0(uint8_t * data, uint8_t len)
 {
 		UART0_WriteString(data);
 }



 //implementing LOG_1 function, which is a concatenation of the LOG_0 functionality with another parameter
 //the parameter can only be a an integer for this function
 void LOG_1(uint8_t * data, uint8_t len, uint32_t param, uint8_t data_type_size)
 {
 		LOG_0(data, len);
 		char str[20];
 		my_itoa(str, param, 0);
 		uint8_t length = strlen(str);
 		LOG_0(str, length);
 }



 //implementing LOG_2 function, which has the same functionality of LOG_1 but the parameter is a floating type
 void LOG_2(uint8_t * data, uint8_t len, float param, uint8_t data_type_size)
 {
 		LOG_0(data, len);
 		char str[20];
 		my_ftoa(str, param, 6);	//6 digits of precision after decimal point
 		uint8_t length = strlen(str);
 		LOG_0(str, length); //using sizeof since the size of float maybe different on different architectures
 }



//starting the profiler function for KL25z
void start_profiling(void)
{

SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;			// Enable TPM1 Clock gate
SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);			// Select MCGIRFLL clock 21 Mhz

TPM1_SC=0;									// Disable Timer
TPM1 -> MOD = 65535;								//Set Counter for 10 uS


TPM1_SC |= TPM_SC_PS(0);	// Prescaler divide by 1

TPM1->SC |= 0x80;
TPM1_SC |= TPM_SC_TOIE_MASK; 	// Enable Timer Overflow Interrupt
//NVIC_EnableIRQ(TPM1_IRQn);  	// Enable NVIC Interrupt
NVIC->ISER[0] |= 0x00040000;
//__enable_irq();					// Enable global Interrupt

}



//function to stop the timer and print out the clock cycles
void profiler_stop(void)
{
	float time_spent = 0.0;
    char array[20] = {'\0'};

    uint32_t total_clocks = overflow_counts*65535 + TPM1->CNT;

    LOG_1("\r\nClock cycles spent: ",22,total_clocks,10);

    time_spent = total_clocks/20.97;	//total time in microseconds, 20.97 MHz clock is being used
    LOG_2("\r\nTime spent(in uS): ",21, time_spent, 10);
}


//timer overflow interrupt handler
void TPM1_IRQHandler(void)
{
		overflow_counts++;
		TPM1_SC &= ~TPM_SC_TOF_MASK;	//clear the tof bit by writing 1

}


int main(){
	int i;
    __disable_irq();
	UART0_init();

	start_profiling();			        //calling the profiler function
	__enable_irq();

	memmove_dma(10);				        //function to be profiled
	TPM1->SC = 0x0;                 //stopping the counter
	profiler_stop();                //stop_profiling();

	while(1){

	}
	return 0;
}

