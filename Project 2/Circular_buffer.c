/******************************************************
*   File: Circular_buffer.c
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
*   Description: Source file for implementing Circular buffer functions
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
#include "Circular_buffer.h"
 
uint8_t Rarray[50];
uint8_t Tarray[50];

uint8_t no_of_tests = 0;
uint8_t pass = 0;
uint8_t fail = 0;


// This function checks if the buffer is empty
enum bufferStates is_Buffer_empty(CircBuf_t *circ_ptr){
	if(circ_ptr -> count == 0){
		return Buffer_Empty;
	}
	else{
		return Buffer_Ok;
	}
}


// This function checks if the buffer is full
enum bufferStates is_Buffer_full(CircBuf_t *circ_ptr){
	if(circ_ptr -> count == MAX_SIZE){
		return Buffer_Full;	
	}
	return Buffer_Ok;
}


// This function adds an item to the buffer array
enum bufferStates add_item(CircBuf_t *circ_ptr, uint8_t item){

	if(is_Buffer_full(circ_ptr) == Buffer_Full)	{			      // if buffer is full, return a buffer full error
        //printf("\nCannot add any more elements! Buffer is full.\n");
		return Buffer_Full_Error;                                    
    }
	circ_ptr->count += 1;							    // incrementing the number of elements

	if(circ_ptr->head == (circ_ptr -> buff + (circ_ptr -> size -1))){
	    circ_ptr -> head = circ_ptr -> buff;            // so that we wrap around after the last element
	}
	else
	    circ_ptr -> head++;
	
	*(circ_ptr -> head) = item;              // assigning the item to the incremented head position
	return Buffer_Ok;
}


//This function removes the oldest element from the buffer
uint8_t remove_item(CircBuf_t *circ_ptr){

	if(is_Buffer_empty(circ_ptr) == 0){							// if buffer is empty, do not add more elements. 
		//printf("\nCannot read any more elements! Buffer is empty.\n");
		return 4;												// return 4 which is code for Buffer empty error
    }	
	
	if(circ_ptr -> tail == (circ_ptr -> buff + (circ_ptr -> size -1))){  
	    circ_ptr -> tail = circ_ptr -> buff;    // so that we can wrap around after the last element
	}	
	
	else
	    circ_ptr -> tail++;	
	    	  	
	circ_ptr -> count--;							// number of active elements is decreased
	return *(circ_ptr -> tail);                     // returning the value read
}


// This function initializes the buffer, allocates memory and returns a pointer to the allocated heap
void initialize_Buffer(CircBuf_t *circ_ptr, uint8_t type)
{   
    if (circ_ptr != NULL) {
        // initializations
        if (type == 0)
            circ_ptr -> buff = Rarray;
        else
            circ_ptr -> buff = Tarray;
        circ_ptr -> count  = 0;
        circ_ptr -> size = MAX_SIZE;
        
        // allocating memory to the buffer
        
        
        //checking for successful memory allocation
        if(circ_ptr -> buff == NULL){
            //printf("\nError : Expected memory allocation is bigger than expected.\n");
            //return NULL;
        }
        else{
            circ_ptr -> head = circ_ptr -> buff;
            circ_ptr -> tail = circ_ptr -> buff;   
            //return (circ_ptr -> buff); 
        }  
   }
}

void create(CircBuf_t * circ_ptr, uint16_t length){
    circ_ptr->buff = malloc (length *sizeof(uint16_t));
}
// This function frees up the circular buffer from dynamic memory
void destroy(CircBuf_t *circ_ptr)
{
    if (circ_ptr != NULL) {
        free(circ_ptr -> buff);
        circ_ptr -> buff = NULL;
    }
}


// Writing unit tests for all modules
void unit_testing(CircBuf_t *buf_ptr){
 
    uint8_t i = 0;
    uint8_t *buff_address;
    uint8_t read = 0;
    
    
    printf("STARTING THE CIRCBUFF UNIT TEST SUITE...\n");
    
    // Testing if initialization and allocation was successful
//    buff_address = initialize_Buffer(buf_ptr);
    no_of_tests++;
    if (buff_address == NULL){
        printf("CB UNIT TEST: 1/10 - <INITIALIZE AND ALLOCATE> ...FAIL\n");
        fail++;
    }
    else{
        printf("CB UNIT TEST: 1/10 - <INITIALIZE AND ALLOCATE> ...PASS\n");
        pass++;
    }
    
    //checking an empty buffer for the empty condition
    no_of_tests++;
    if(is_Buffer_empty(buf_ptr) == Buffer_Empty){
        printf("CB UNIT TEST: 2/10 - <CHECKING AN EMPTY BUFFER> ...PASS\n");
        pass++;
    }
    else{
        printf("CB UNIT TEST: 2/10 - <CHECKING AN EMPTY BUFFER> ...FAIL\n");
        fail++;
    }
    
    //test to read value from an empty buffer
    
    no_of_tests++;
    read = remove_item(buf_ptr);
    if(read == 4){
         printf("CB UNIT TEST: 3/10 - <READING ITEM FROM EMPTY BUFFER> ...PASS\n"); 
         pass++;
    }
    else{
        printf("CB UNIT TEST: 3/10 - <READING ITEM FROM EMPTY BUFFER> ...FAIL\n");
        fail++;
    }   
        
    //for loop to add elements into the buffer till the max count
    // checking if the add item module works correctly
    no_of_tests++;
	for(i=0 ; i< MAX_SIZE ; i++){
		add_item(buf_ptr, i);
	}

	
	if(buf_ptr->count == MAX_SIZE){
	    printf("CB UNIT TEST: 4/10 - <VERIFYING ADD ITEM FUNCTIONALITY> ...PASS\n");
	    pass++;
	}
	else{
	    printf("CB UNIT TEST: 4/10 - <VERIFYING ADD ITEM FUNCTIONALITY> ...FAIL\n");
	    fail++;
	}	
	
	//checking if the buffer full condition is detected
	no_of_tests++;
	if(is_Buffer_full(buf_ptr) == Buffer_Full){
        printf("CB UNIT TEST: 5/10 - <CHECKING A FULL BUFFER> ...PASS\n");
        pass++;
    }
    else{
        printf("CB UNIT TEST: 5/10 - <CHECKING A FULL BUFFER> ...FAIL\n");
        fail++;
    }
    
    // checking if an extra item can be added to a full buffer
    no_of_tests++;
    add_item(buf_ptr, 100);
    if(buf_ptr -> count == MAX_SIZE){
         printf("CB UNIT TEST: 6/10 - <ADDING ITEM TO FULL BUFFER> ...PASS\n"); 
         pass++;
    }
    else{
        printf("CB UNIT TEST: 6/10 - <ADDING ITEM TO FULL BUFFER> ...FAIL\n");
        fail++;
    }   
    
    //reading items from a non-empty buffer
    no_of_tests++;
    for(i=0;i<MAX_SIZE;i++){
        remove_item(buf_ptr);
    }
    if(buf_ptr -> count == 0){
        printf("CB UNIT TEST: 7/10 - <REMOVING ITEMS FROM A FULL BUFFER> ...PASS\n"); 
        pass++;
    }    
    else{
        printf("CB UNIT TEST: 7/10 - <REMOVING ITEMS FROM A FULL BUFFER> ...FAIL\n");
        fail++;
    }  
    
    //adding 1 item and then removing it to see if there is a proper wraparound
    no_of_tests++;
    add_item(buf_ptr, 100);
    remove_item(buf_ptr);
    if(buf_ptr -> count == 0){
        printf("CB UNIT TEST: 8/10 - <CHECKING THE WRAP-AROUND CASE> ...PASS\n"); 
        pass++;
    }  
    else{
        printf("CB UNIT TEST: 8/10 - <CHECKING THE WRAP-AROUND CASE> ...FAIL\n");
        fail++;
    }  
      
    // adding an element and removing two to check for an error
    no_of_tests++;
    add_item(buf_ptr,100);
    remove_item(buf_ptr);
    remove_item(buf_ptr);
    if(is_Buffer_empty(buf_ptr) == 0){
        printf("CB UNIT TEST: 9/10 - <READING MORE ELEMENTS THAN PRESENT> ...PASS\n"); 
        pass++;
    }  
    else{
        printf("CB UNIT TEST: 9/10 - <READING MORE ELEMENTS THAN PRESENT> ...FAIL\n"); 
        fail++;
    }       
    
    // test to check if the buff pointer has been freed after using the free function
    no_of_tests++;
    destroy(buf_ptr);
    if(buf_ptr -> buff == NULL){
        printf("CB UNIT TEST: 10/10 - <DYNAMIC MEMORY SUCCESSFULLY CLEARED> ...PASS\n"); 
        pass++;
    }  
    else{
        printf("CB UNIT TEST: 10/10 - <DYNAMIC MEMORY SUCCESSFULLY CLEARED> ...FAIL\n"); 
        fail++;
    }    
    
    
    // printing the result of the tests
    if(pass == no_of_tests){
        printf("CIRCBUFF UNIT TEST SUITE: PASS (%d/%d PASS)\n", pass, no_of_tests);
    }
    else{
        printf("CIRCBUFF UNIT TEST SUITE: FAIL (%d/%d PASS)\n", pass, no_of_tests);
    }       

}
