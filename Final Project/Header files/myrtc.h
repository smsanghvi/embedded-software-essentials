/******************************************************
*   File: myrtc.h
*
*   Copyrights 2016 Snehal Sanghvi and Shalin Shah
*   All Rights Reserved
*
*   The information contained herein is property of the Authors.
*   The copying and distribution of the files is prohibited except
*   by express written agreement with the Authors.
*
*   Authors: Shalin Shah and Snehal Sanghvi 
*   Date Edited: 3 Dec 2016
*
*   Description: Header file for using Real time clock on KL25Z
*               -rtc_init
*               -get_unix_time
*               -calculate_date
*               -get_rtc_seconds
*               -get_rtc_minutes
*               -get_rtc_hours
*               -get_rtc_year
*               -get_rtc_month
*               -get_rtc_day
*
********************************************************/

#ifndef MYRTC_H_
#define MYRTC_H_

#include "MKL25Z4.h"
#include "stdint.h"




typedef struct myrtc_t {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint16_t year;
    uint8_t month;
    uint8_t day;
}rtc;


void rtc_init(void) ;

uint32_t get_unix_time(void);

void calculate_date(void);

uint8_t get_rtc_seconds(void);

uint8_t get_rtc_minutes(void);

uint8_t get_rtc_hours(void);

uint16_t get_rtc_year(void);

uint8_t get_rtc_month(void);

uint8_t get_rtc_day(void);

#endif
