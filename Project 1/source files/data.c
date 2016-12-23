/********************************************
*   File: data.c
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
*   Authors: Shalin Shah and Snehal Sanghvi
*   Date Edited: 18 Sept 2016
*
*   Description: Source file for the data manipulation functions
*               -my_itoa
*               -my_atoi
*               -dump_memory
*               -big_to_little
*               -little_to_big
*
*
********************************************************/


#include <stdio.h>
#include <stdint.h>
#include "../Headers/data.h"


/*
Integer to ASCII (Null terminated string)
*/
int8_t * my_itoa(int8_t *str, int32_t data, int32_t base){

    // If the number is '0'
    if (data == 0){
        *str++ = '0';
        *str-- = '\0';   //Adding null for end of string and resetting str to initial value
        return str;
    }


    int8_t neg = 0;    // To check if the number is negative
    int rem = 0;     //variable to store remainder
    uint8_t length = 0;         //Calculating length of the string

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

    uint8_t j=0;         //Initializing counter for the loop
    int8_t temp;
    str = str - length;        //Resetting str to initial value
    for(j=0;j<length/2;j++){ //loop to reverse string
        temp=*(str+j);
        *(str+j) = *(str+length-j-1);
        *(str+length-j-1)=temp;
    }
    return str;
}





/*
ASCII to integer
*/
int32_t my_atoi(int8_t *str){
    if (*str == '\0')          //If the string is empty, return integer is '0'
       return 0;

    int32_t number = 0;  // Initialize the output number
    int8_t neg = 1;  // For negative values, setting it as positive by default
    //uint8_t i = 0;  // Initialize index of first digit

    // If number is negative, then update sign
    if (*str == '-'){
        neg = -1;
        str++;
    }

    // Calculating and adding each digit of the string.
    while(*str != '\0')
    {
        if (*str >= '0' && *str <= '9')
            number = number*10 + *str++ - '0';
        else {
        printf("\nInvalid numerical string");           //Generate error message if string does not have a number
        return 0;
        }
    }

    // Return result with sign
    return neg*number;

}




/*
Print the hex output of the values in the memory space
*/
void dump_memory(uint8_t * start, uint32_t length){
    while(length--)
        printf("\n%x",*start++);
}





/*
Convert big endian representation to little endian
*/
uint32_t big_to_little(uint32_t data){
    return ( ((data<<24)&0xFF000000) |              //Put last byte to first byte
            ((data<<8)&0x00FF0000) |                //Put third byte to second byte
            ((data>>8)&0x0000FF00) |                //Put second byte to third byte
            ((data>>24)&0x000000FF)                 //Put first byte to last byte
            );
}




/*
Convert little endian representation to big endian
*/
uint32_t little_to_big(uint32_t data){
    return ( ((data<<24)&0xFF000000) |              //Put last byte to first byte
            ((data<<8)&0x00FF0000) |                //Put third byte to second byte
            ((data>>8)&0x0000FF00) |                //Put second byte to third byte
            ((data>>24)&0x000000FF)                 //Put first byte to last byte
            );
}

