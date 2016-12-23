/********************************************
*   File: profiler.c
*
*   Copyrights 2016 Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*
*   Authors: Snehal Sanghvi and Shalin Shah
*   Date Edited: 14 Oct 2016
*
*   Description: Source file for testing the timing of various functions
*   		 on the Beaglebone Black and Freedom Freescale KL25z
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
*               -my_memmove
*
*
********************************************************/

#ifdef FRDM
	#include "MKL25Z4.h"
#endif
#ifdef BBB
	#include <time.h>
	#include <sys/time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "profiler.h"
#include <string.h>

uint16_t overflow_counts=0;

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
 #ifdef VERBOSE	//switch to turn or off the output
 	#ifdef FRDM		//for freedom freescale
 			UART0_WriteString(data);
 	#endif

 	#ifdef BBB		//for Beaglebone Black
 		for(int i=0; i<len; i++){
 			printf("\n\r%c", *data);
 			data++;
 		}
 	#endif
 #endif
 }


 //implementing LOG_1 function, which is a concatenation of the LOG_0 functionality with another parameter
 //the parameter can only be a an integer for this function
 void LOG_1(uint8_t * data, uint8_t len, uint32_t param, uint8_t data_type_size)
 {
 #ifdef VERBOSE	//switch to turn or off the output
 	#ifdef FRDM		//for freedom freescale
 		LOG_0(data, len);
 		char str[20];
 		my_itoa(str, param, 0);
 		uint8_t length = strlen(str);
 		LOG_0(str, length);
 	#endif

 	#ifdef BBB
 		for(int i=0;i<len;i++){
 			printf("\n\r%c", *data);
 			data++;
 		}
 		printf(" %d", param);
 	#endif
 #endif
 }


 //implementing LOG_2 function, which has the same functionality of LOG_1 but the parameter is a floating type
 void LOG_2(uint8_t * data, uint8_t len, float param, uint8_t data_type_size)
 {
 #ifdef VERBOSE	//switch to turn or off the output
 	#ifdef FRDM		//for freedom freescale
 		LOG_0(data, len);
 		char str[20];
 		my_ftoa(str, param, 6);	//6 digits of precision after decimal point
 		uint8_t length = strlen(str);
 		LOG_0(str, length); //using sizeof since the size of float maybe different on different architectures
 	#endif

 	#ifdef BBB
 		for(int i=0; i<len; i++){
 			printf("\n\r%c",*data);
 			data++;
 		}
 		printf(" %.6f",param);
 	#endif
 #endif
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


//function to be tested
int8_t my_memmove(uint8_t *src, uint8_t *dst, uint32_t length){

    if(  ( (dst+length)  <src) && (dst < src)){       //destination has lower memory than source and does not overlap
        while(length--)
            *dst++ = *src++;
        return 0;
    }
    else if (   ( (src+length) < dst)   && ( src < dst) ){        //source has lower memory than destination and does not overlap
       while(length--)
            *dst++ = *src++;
       return 0;
    }
    else if( (src<dst) && ( (src+length) > dst)  ){               //source has lower memory than destination and overlaps
        while(length--)
            *(dst+length-1) = *(src+length-1);
       // printf("\n The memory is overlapping, it will overlap the source string");
        return 0;
    }
    else if(  ((dst+length)  >src) && (dst < src)){                 //destination has lower memory than source and overlaps
        while(length--)
            *dst++ = *src++;
       // printf("\n The memory is overlapping, it will overlap the source string");
        return 0;
    }
    else return 1;                             //Error code for fail
}


int main(){

#ifdef FRDM
	uint8_t src[10];
	uint8_t dst[10];

    __disable_irq();
	UART0_init();
	start_profiling();			        //calling the profiler function
	__enable_irq();

    TPM1_SC |= TPM_SC_CMOD(1);		    // Start Counter
	my_memmove(src, dst, 10);
    TPM1->SC = 0x0;                 //stopping the counter
	profiler_stop();                //stop_profiling();
#endif
#ifdef BBB
	uint8_t src[] = "1234567890";  //used for testing my_atoi
	uint8_t dst[10];

	struct timeval start, end;
	gettimeofday(&start, NULL);		//getting initial time
	my_memmove(src,dst,10);			// function to be profiled
	gettimeofday(&end, NULL);		//getting final time

    printf("\nNumber of clock cycles %ld\n",((end.tv_sec * 1000000 + end.tv_usec)
			  - (start.tv_sec * 1000000 + start.tv_usec))*300);

	printf("Execution time(in us): %ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
			  - (start.tv_sec * 1000000 + start.tv_usec)));
#endif

	return 0;
}

