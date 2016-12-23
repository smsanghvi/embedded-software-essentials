/******************************************************
*   File: log.c
*
*   Copyrights 2016  Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
**
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
**   Authors: Snehal Sanghvi and Shalin Shah
*   Date Edited: 11 Nov 2016
*
*   Description: Source file for implementing a logger function that is independent
*               -reverse_string
*               -my_itoa
*               -my_ftoa
*               -LOG_0
*               -LOG_1
*               -LOG_2
*
********************************************************/

#include "MKL25Z4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"


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
 int my_itoa(char arr[], char number, int no_of_digits)
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

