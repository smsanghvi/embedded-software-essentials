/********************************************
*   File: memory_frdm.h
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
*   Description: Header file for the memory operation functions
*               for NXP FRDM board
*               -my_memmove 
*               -my_memzero
*               -my_reverse
*
*
********************************************************/


#ifndef _MEMORY_H
#define _MEMORY_H

#include "stdint.h"

/*
my_memmove will copy the values from src to dst for the length of bytes provided 
*/
int8_t my_memmove(uint8_t *src, uint8_t *dst, uint32_t length); 


/* 
my_memzero will turn all elements of the memory space to '0'
*/
int8_t my_memzero(uint8_t *src, uint32_t length); 


/*
my_reverse reverses all the characters of the string
*/
int8_t my_reverse(uint8_t *src, uint32_t length); 

#endif


