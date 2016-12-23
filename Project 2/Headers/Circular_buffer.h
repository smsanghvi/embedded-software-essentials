/******************************************************
*   File: Circular_buffer.h
*
*   Copyrights 2016  Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*
*   The information contained herein is property of the Authors. 
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*
*   Authors: Snehal Sanghvi and Shalin Shah
*   Date Edited: 8 Oct 2016
*
*   Description: Header file for implementing Circular buffer functions
*               -is_Buffer_empty 
*               -is_Buffer_full
*               -add_item
*               -remove_item
*               -initialize_Buffer
*               -destroy
*
********************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef _CIRCULAR_BUFFER_H
#define _CIRCULAR_BUFFER_H

#define MAX_SIZE 50

//declaring the structure and its fields
typedef struct CircBuf
{   
    uint8_t *buff; // original memory location of buffer
    uint8_t *head; 
    uint8_t *tail;
    uint8_t size;  // size of the memory buffer 'buff'
    uint8_t count; //number of active elements in the buffer
}CircBuf_t;


//declaring an enum to encode buffer states
enum bufferStates {Buffer_Empty, Buffer_Full, Buffer_Ok, Buffer_Full_Error, Buffer_Empty_Error};   


// This funtion takes in a pointer to structure and returns a code indicating whether buffer is empty or not
enum bufferStates is_Buffer_empty(CircBuf_t *circ_ptr);

// This funtion takes in a pointer to structure and returns a code indicating whether buffer is full or not
enum bufferStates is_Buffer_full(CircBuf_t *circ_ptr);

// This function adds an item to the buffer as long as the buffer is not full
enum bufferStates add_item(CircBuf_t *circ_ptr, uint8_t item);

//This function takes in a pointer to structure and returns the popped value 
uint8_t remove_item(CircBuf_t *circ_ptr);

// This function initializes the buffer
void initialize_Buffer(CircBuf_t *circ_ptr, uint8_t buffsize);

//This fuction allocates memory to the buffer
void create(CircBuf_t * circ_ptr, uint16_t length);

// This function frees up the circular buffer from dynamic memory
void destroy(CircBuf_t *circ_ptr);

#endif
