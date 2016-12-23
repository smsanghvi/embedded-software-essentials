/********************************************
*   File: main.c
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
*   Description: Main file for project 1 library
*
*
********************************************************/


#ifdef FRDM 
    #include "../Headers/project_1_frdm.h"
#elif  BBB
    #include "../Headers/project_1.h"
#else
    #include "../Headers/project_1.h"
#endif


int main(){
    project_1_report();
    return 0;
}
