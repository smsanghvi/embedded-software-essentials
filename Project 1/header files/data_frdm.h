/********************************************
*   File: data_frdm.h
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
*   Date Edited: 17 Sept 2016
*
*   Description: Header file for the data manipulation functions
*               for NXP FRDM board
*               -my_itoa 
*               -my_atoi
*               -dump_memory
*               -big_to_little
*               -little_to_big
*
*
********************************************************/

#ifndef _DATA_H
#define _DATA_H

#include "stdint.h"

/*
Integer to ASCII (Null terminated string)
*/
int8_t * my_itoa(int8_t *str, int32_t data, int32_t base);

/*
ASCII to integer 
*/
int32_t my_atoi(int8_t *str);

/*
Print the hex output of the values in the memory space 
*/
void dump_memory(uint8_t * start, uint32_t length);

/*
Convert big endian representation to little endian
*/
uint32_t big_to_little(uint32_t data);

/*
Convert little endian representation to big endian
*/
uint32_t little_to_big(uint32_t data);

#endif 
