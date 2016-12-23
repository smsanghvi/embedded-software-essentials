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
*   Date Edited: 12 Oct 2016
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

#include <stdio.h>
#include "log.h"
#include "uart.h"
#include <string.h>


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
char * my_itoa(char *str, uint32_t data, int32_t base){

    uint8_t j=0;         //Initializing counter for the loop
    int8_t temp;
    uint8_t length = 0;         //Calculating length of the string
    int8_t neg = 0;    // To check if the number is negative
    int rem = 0;     //variable to store remainder
    // If the number is '0'
    if (data == 0){
        *str++ = '0';
        *str-- = '\0';   //Adding null for end of string and resetting str to initial value
        return str;
    }

    //If data is negative
    if (data < 0){
        neg = -1;
        data = -data;
    }

    //Dividing with the base to get the value of data in that base and storing it in the string
    while (data != 0){
        rem = data % base;
        *str++ = (rem > 9)? (rem-10) + 'A' : rem + '0';             //Ternary for base values greater than 10.
        length++;
        data = data/base;
    }

    // If data is negative adding minus sign
    if(neg == -1){
        *str++ = '-';
        length++;
    }

    *str = '\0'; // Append null character for end of string

    // Reverse the string for final output as the loop gives us the last value first

    
    str = str - length;        //Resetting str to initial value
    for(j=0;j<length/2;j++){ //loop to reverse string
        temp=*(str+j);
        *(str+j) = *(str+length-j-1);
        *(str+length-j-1)=temp;
    }
    return str;
}


// floating point to ascii converter
void my_ftoa(char * str, float data, uint8_t resolution)
{
    uint8_t length = 0;                         //Initializing length of the string
    int32_t integer = (int32_t)data;            //Saving the integer part
    data = data - integer;                      //Now only the decimal part remains in the data


    if(data<0)
        data = -data;
    my_itoa(str, integer, 10);                  //Pass the pointer, integer with a default base 10

    
    while(*(str+length))                        //Calculate the length of the string excluding the null character
        ++length;

    /*The string now has the integer part*/
    str += length;                              //Offsetting the length to end of the integer
    *str++ = '.';                               //Adding the decimal point

    while(resolution--){
        data *= 10;                             //Converting the required decimal resolution to integer
        if((uint32_t)data == 0)                 //Adding zeros for the zeros before the first non zero number after the decimal
            *str++ = '0';
    }
    my_itoa(str,(uint32_t)data, 10);            //Pass the pointer, decimal part with a default base 1
}



// implementing LOG_0 function, which is used to write data bytes onto the terminal
void LOG_0(char * data, uint8_t len)
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
void LOG_1(char * data, uint8_t len, uint32_t param, uint8_t data_type_size)
{
#ifdef VERBOSE	//switch to turn or off the output
	#ifdef FRDM		//for freedom freescale
        uint8_t length;	
        char str[20] = {'\0'};
        char fullstr[100] = {'\0'};
        my_itoa(str, param, 10);
		length = strlen(str);
        strcat(fullstr,data);
        strcat(fullstr,str);
        length = strlen(fullstr);
		LOG_0(fullstr, length);
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
void LOG_2(char * data, uint8_t len, float param, uint8_t data_type_size)
{
#ifdef VERBOSE	//switch to turn or off the output
	#ifdef FRDM		//for freedom freescale
		uint8_t length;
        char str[40] = {'\0'};  
        char fullstr[150] = {'\0'};
        strcat(fullstr,data);
		my_ftoa(str, param, 6);	//6 digits of precision after decimal point
		strcat(fullstr,str);
        length = strlen(fullstr);
		LOG_0(fullstr, length); //using sizeof since the size of float maybe different on different architectures
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


void LOG_cat(char * data, uint8_t len, uint32_t param, uint8_t data_type_size)
{
#ifdef VERBOSE	//switch to turn or off the output
	#ifdef FRDM		//for freedom freescale
        uint8_t length;	
        char str[20] = {'\0'};
        char fullstr[100] = {'\0'};
        UART0_WriteString("Before Concatenation");
        LOG_0(data, len);
        my_itoa(str, param, 10);
		length = strlen(str);
		LOG_0(str, length);
        strcat(fullstr,data);
        strcat(fullstr,str);
        UART0_WriteString("After Concatenation");
        length = strlen(fullstr);
		LOG_0(fullstr, length);
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
