/********************************************
*   File: project_1_frdm.c
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
*   Description: Source file for the testing functions for project
*               for NXP FRDM board
*               -project_1_report
*
*
*
********************************************************/

#ifndef NULL
#define NULL '\0'
#endif

#pragma GCC poison printf

#include "stdio.h"
#include "stdint.h"
#include "../Headers/project_1_frdm.h"
#include "../Headers/memory_frdm.h"
#include "../Headers/data_frdm.h"

/*
Mandatory function required for project report
*/
void project_1_report(){
    
    //Create an array of 32 bytes
    uint8_t ReportArray[32]={'\0'};
    #define LENGTH 32
    
    //Create three pointers to various points in this array
    uint8_t * aptr_1 = ReportArray;        //Initializing aptr_1 to the first element of the array
    uint8_t * aptr_2 = (aptr_1 + 8);     //Initializing aptr_2 to the 9th element of the array
    uint8_t * aptr_3 = (aptr_1 +16);     //Initializing aptr_3 to the 16th element of the array
    
    //Initialize memory at two of the pointers
    int count = 16;                 //Set count value
    while(count--){
        *(aptr_1+count)=(31+count);    //Put value of the first 16 bytes as 31-46 respectively
        
    }
   
        
    my_memzero(aptr_3,16);           //Using my_memzero to set the remaining values of the array to '0'
    
    //Use memmove to move 8 bytes from aptr_1 to aptr_3
    #define LENGTH8 8
    uint8_t Error = my_memmove(aptr_1,aptr_3,LENGTH8);
    if(Error!=0)
        printf("\n Error code %d",Error);            //Print error if memmove fails
    
    
    //Use memmove to move 16 bytes from aptr_2 to aptr_1
    #define LENGTH16 16
    Error = my_memmove(aptr_2,aptr_1,LENGTH16);
    if(Error!=0)
        printf("\n Error code %d",Error);            //Print error if memmove fails
    
    //Use reverse on aptr_1 to reverse the entire 32 byte array
    Error = my_reverse(aptr_1,LENGTH);
    if(Error!=0)
        printf("\n Error code %d",Error);            //Print error if reverse fails
        

    //Print the entire 32 byte array
    printf("\n The new modified array is:  ");
    for(count=0;count<LENGTH;++count){
        printf("%d",*(aptr_1+count));
    }
    
    //Testing all the fuctions of the source files
    printf("\n\n\n\n Testing working of all functions");      
    uint8_t TestArray[] = {"HelloWorld"};           //Initialize Test string
    printf("\n Original string:  %s",TestArray);
    
    
    uint8_t TestArray2[20]={'\0'};                      //Initialize test string to 
    my_memmove(TestArray,TestArray2,10);
    printf("\nMoving 10 bytes to another array: %s",TestArray2); //Print the output of my_memmove
    
    my_reverse(TestArray,10);
    printf("\nReversing 10 bytes of the array:%s   ",TestArray);      //Print the output of the reversed string
    
    my_memzero(TestArray, 10);
    printf("\nSetting 10 bytes of the array to zero:  ");       //Print the output after testing my_memzero
    for (count=0;count<10;++count)
        printf("%d",*(TestArray+count));
    
    int32_t TestData = -12345678;
    printf("\nOriginal test data:%d  ",TestData);                   //Initialize data for testing functions of the data source file
    
    
    int8_t TestString[20] = {};                         //Initializ8e test strings to test data.c source file.
    int8_t TestString2[20] = {};
 
    
    my_itoa(TestString, TestData, 16);     //Testing interger conversion to string with base 16 and negative value
    printf("\nNegative integer base 16 converted to string:  %s",TestString);
    
    my_itoa(TestString2, TestData, 10);     //Testing interger conversion to string with base 10 and negative value
    printf("\nNegative integer base 10 converted to string:%s  ",TestString2);
    
    TestData = - TestData;                              //Changing sign for testing positive values
    
    my_itoa(TestString, TestData, 16);     //Testing same interger conversion to string with base 16
    printf("\nInteger base 16 converted to string:  %s",TestString);
    
    my_itoa(TestString, TestData, 10);     //Testing interger conversion to string with base 10
    printf("\nInteger base 10 converted to string:%s  ",TestString);
    
        
    printf("\nPositive string converted to integer:%d  ", my_atoi(TestString));  //Testing positive string to integer conversion
    
    printf("\n Negative string converted to integer:%d  ", my_atoi(TestString2)); //Testing negative string to integer conversion
    
    printf("\n Dumping 10 bytes of memory of the positive converted string  ");
    dump_memory((uint8_t*)TestString,10);               //Dumping 10 bytes of memory starting from TestString
    
    TestData = 0xdeadbeef;                              //Initializing TestData to test endian conversions
    
    TestData = little_to_big(TestData);
    printf("\nCoverting for little endian to big endian:%x  ",TestData); //Printing little to big endian conversion
    
    printf("\nCoverting for big endian to little endian:%x  ",big_to_little(TestData)); //COnverting big endian back to little endian   
    
}



