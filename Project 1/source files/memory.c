/********************************************
*   File: memory.c
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
*   Description: Source file for the memory operation functions
*               -my_memmove 
*               -my_memzero
*               -my_reverse
*
*
********************************************************/

#include "stdio.h"
#include "stdint.h"
#include "../Headers/memory.h"




/*
my_memmove will copy the values from src to dst for the length of bytes provided 
*/

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
        printf("\n The memory is overlapping, it will overlap the source string");
        return 0;
    }
    else if(  ((dst+length)  >src) && (dst < src)){                 //destination has lower memory than source and overlaps
        while(length--)
            *dst++ = *src++;
        printf("\n The memory is overlapping, it will overlap the source string");
        return 0;
    }
    else return 1;                             //Error code for fail
}


/* 
my_memzero will turn all elements of the memory space to '0'
*/
	
int8_t my_memzero(uint8_t *src, uint32_t length){
    if(*src == '\0')
        return 1;
    while(length--)
        *src++ = 0;
    return 0;
}

/*
my_reverse reverses all the characters of the string
*/

#define INT_MAX 2147483647

int8_t my_reverse(uint8_t *src, uint32_t length){
    if(length<0) // check for negative length
    return 1;
    else if(length>INT_MAX) //check for integer overflow
    return 2;
    else if(length ==0) //check for 0 length
    return 3;
    else{
        int i;
        char temp=0;
        for(i=0;i<length/2;i++) //loop to reverse string
        {
            temp=*(src+i);
            *(src+i) = *(src+length-i-1);
            *(src+length-i-1)=temp;
        }
        return 0;
    }
}
      


