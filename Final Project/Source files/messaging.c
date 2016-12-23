/******************************************************
*   File: messaging.c
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
*   Authors: Shalin Shah and Snehal Sanghvi
*   Date Edited: 9 Nov 2016
*
*   Description: Source file for messaging using Circular buffer
*               -decode_message
*               -ack_nack
*               -checksum_validate
*
********************************************************/

#include "messaging.h"


extern CircBuf_t *rx_buffer;
extern CircBuf_t *tx_buffer;


/* Function to decode the incoming message packet*/
void decode_message (void){   
    int total_data = 0;                                             //To count the total data bytes
    __wfi();                                                        //Wait till a new char is received     
    message.Command =  (int)remove_item(rx_buffer);                 //Read the command
    UART0_WriteChar('c');                                           
    __wfi();                                                        //Wait till a new char is received                 
    message.length =   (uint8_t)remove_item(rx_buffer);             //Read the lenght of the incoming data
    UART0_WriteChar('l');
    for(; total_data<message.length; total_data++){
        __wfi();                                                    //Wait till a new char is received     
        message.data[total_data] = (uint8_t)remove_item(rx_buffer); //Get the data and store it in an array
    }
    message.data[total_data] = '\0';                                //Append null to end data
    UART0_WriteChar('d');   
    __wfi();                                                        //Wait till a new char is received     
    message.checksum = ((uint16_t)remove_item(rx_buffer)<<8);       //Checksum MSB
    UART0_WriteChar('c');
    __wfi();                                                        //Wait till a new char is received     
    message.checksum |= (uint16_t)remove_item(rx_buffer);           //Checksum LSB
    UART0_WriteChar('s');
    if(checksum_validate()== CHKSUM_FAIL)                           //Validate the message
    {
        ack_nack(NACK);                                             //Send NACK for checksum fail
        LOG_0("Message corrupted",17);
    }
    else{
        ack_nack(ACK);                                              //Send ACK for checksum pass
        LOG_0("Transmit successful",19);
        if(message.Command == LED_TOGGLE){                          //Execute the command
            if(*message.data == OFF){   
                LED_Control('0');                     //For LED off
            }
            else if(*message.data == BLUE){
                LED_Control('1');                     //For LED BLUE
            }
            else if(*message.data == GREEN){
                LED_Control('2');                     //For LED GREEN
            }
            else if(*message.data == CYAN){
                LED_Control('3');                     //For LED CYAN
            }
            else if(*message.data == RED){
                LED_Control('4');                     //For LED RED
            }
            else if(*message.data == PINK){
                LED_Control('5');                     //For LED PINK
            }
            else if(*message.data == ORANGE){
                LED_Control('6');                     //For LED ORANGE
            }
            else if(*message.data == WHITE){
                LED_Control('7');                     //For LED WHITE
            }
        }
        else if(message.Command == BRIGHTNESS){
            if(*message.data == TEN_PERCENT){
                LED_SET_BRIGHTNESS((int)TEN_PERCENT);  //Set brightness to TEN_PERCENT    
            }
            else if(*message.data == TWENTY_PERCENT){
                LED_SET_BRIGHTNESS(TWENTY_PERCENT);    //Set brightness to TWENTY_PERCENT    
            }
            else if(*message.data == THIRTY_PERCENT){
                LED_SET_BRIGHTNESS(THIRTY_PERCENT);    //Set brightness to THIRTY_PERCENT    
            }
            else if(*message.data == FORTY_PERCENT){
                LED_SET_BRIGHTNESS(FORTY_PERCENT);     //Set brightness to FORTY_PERCENT    
            }
            else if(*message.data == FIFTY_PERCENT){
                LED_SET_BRIGHTNESS(FIFTY_PERCENT);     //Set brightness to FIFTY_PERCENT    
            }
            else if(*message.data == SIXTY_PERCENT){
                LED_SET_BRIGHTNESS(SIXTY_PERCENT);     //Set brightness to SIXTY_PERCENT    
            }
            else if(*message.data == SEVENTY_PERCENT){
                LED_SET_BRIGHTNESS(SEVENTY_PERCENT);   //Set brightness to SEVENTY_PERCENT    
            }
            else if(*message.data == EIGHTY_PERCENT){
                LED_SET_BRIGHTNESS(EIGHTY_PERCENT);    //Set brightness to EIGHTY_PERCENT    
            }
            else if(*message.data == NINTY_PERCENT){
                LED_SET_BRIGHTNESS(NINTY_PERCENT);     //Set brightness to NINTY_PERCENT    
            }
            else if(*message.data == HUNDRED_PERCENT){
                LED_SET_BRIGHTNESS(HUNDRED_PERCENT);   //Set brightness to HUNDRED_PERCENT    
            }
        }
        else if(message.Command == ECHO){
            UART0_WriteString("\n\r Echo mode: ");
            UART0_WriteString((char*)message.data);
        }
    }        
}


/*Function to send acknowledge after the message is received*/
void ack_nack (CMDS reply){
    UART0_WriteChar(reply);                             //Send the ack/nack reply 
}


/*Function to validate the checksum at the end of each message using addition algorithm*/
ERR checksum_validate (void){
    int i=0;                                            //Integer to count the data
    uint16_t check_sum;                                 //Generate checksum using the message data    

	check_sum = message.Command + (uint16_t)message.length;
	for(;i<((message.length));i++)
	{
		check_sum += (uint16_t)message.data[i];
	}
	if(check_sum == message.checksum)                   //Verify it with the checksum received
		return CHKSUM_PASS;                             //Return corresponding error code
	else
		return CHKSUM_FAIL;                             
}
