/******************************************************
*   File: log.h
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

#ifndef INCLUDES_LOG_H_
#define INCLUDES_LOG_H_

#include <stdint.h>

// reverses a string 'arr' of length 'length'
void reverse_string(char *arr, int length);

// integer to ascii converter
char * my_itoa(char arr[], uint32_t number, int32_t base);

// floating point to ascii converter
void my_ftoa(char * str, float data, uint8_t resolution);

// LOG_0 function is used to write data bytes onto the terminal
void LOG_0(char * data, uint8_t len);

//LOG_1 function is a concatenation of the LOG_0 functionality with another parameter
//the parameter can only be a an integer for this function
void LOG_1(char * data, uint8_t len, uint32_t param, uint8_t data_type_size);

//implementing LOG_2 function, which has the same functionality of LOG_1 but the parameter is a floating type
void LOG_2(char * data, uint8_t len, float param, uint8_t data_type_size);

//LOG_1 function is a concatenation of the LOG_0 functionality with another parameter
//the parameter can only be a an integer for this function. This one shows the strings being concatenated
void LOG_cat(char * data, uint8_t len, uint32_t param, uint8_t data_type_size);

#endif /* INCLUDES_LOG_H_ */
