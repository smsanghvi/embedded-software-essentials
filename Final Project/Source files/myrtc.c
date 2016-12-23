/******************************************************
*   File: myrtc.c
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
*   Description: Source file for using Real time clock on KL25Z
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

#include "myrtc.h"
#include "stdint.h"

struct myrtc_t myrtc;
uint32_t rtc_time;

void rtc_init(void) 
{   
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;      //Enable Port C 
    MCG_C1 |= MCG_C1_IRCLKEN_MASK;      //Enable internal reference clock
	MCG_C2 &= ~(MCG_C2_IRCS_MASK);      //Internal Reference Clock Slow
	PORTC_PCR1 |= (PORT_PCR_MUX(0x1));              //PTC1 as RTC_CLKIN
    SIM_SOPT1 |= 0x00080000;                    //RTC_CLKIN selected for OSC32KCLK
    SIM_SOPT2 |= 0x00000080;                    //MCGIRCLK selected for clock out
    PORTC_PCR3 |= (PORT_PCR_MUX(0x5));              //PTC3 as CLKOUT
    
    __enable_irq();
    
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;        //Enable the clock to RTC
    RTC_CR  = RTC_CR_SWR_MASK;              
    RTC_CR  &= ~RTC_CR_SWR_MASK;            //Disable and reset RTC
  
    if (RTC_SR & RTC_SR_TIF_MASK){          //Check if time invalid
        RTC_TSR = 0x00000000;               //Clears the Time invalid flag
    }
    
    RTC_TCR = RTC_TCR_CIR(1) | RTC_TCR_TCR(0xCF);   //Setup compensation parameters
    RTC_CR |= RTC_CR_OSCE_MASK;                     //Enable 32.768Hz oscillator
    RTC_TSR = 0x58425F60;                           //Reset the seconds register
    RTC_SR |= RTC_SR_TCE_MASK;                      //Enable RTC
    myrtc.seconds=0;
    myrtc.minutes=0;
    myrtc.hours=0;
}



 uint32_t get_unix_time(void){
     rtc_time = RTC_TSR;   
     return rtc_time;             //Send the UNIX timestamp
}


void calculate_date(void){
    uint32_t years, days ;
    uint8_t leap_years;
    rtc_time = get_unix_time();         //Get latest timestamp
    
    myrtc.seconds = rtc_time%60;        //Update seconds
    rtc_time /= 60;
    
    myrtc.minutes = rtc_time%60;        //Update minutes
    rtc_time /= 60;
    
    myrtc.hours = rtc_time%24;          //Update hours
    rtc_time /= 24;
    
    years = rtc_time*4;
    years /= (365*4 +1);                //Calculate number of years since 1970
    myrtc.year = (1970+years);          //Update years
    
    leap_years = years/4;
    years -= leap_years;
    
    days = leap_years*366 + years*365;
    days = rtc_time - days + 1;             //Calculate number of days since 1970
    
    if(myrtc.year%4 == 0){              //For leap years
        if(days <= 31){
            myrtc.month = 1;            //January
            myrtc.day = days;
        }
        else if(days <= 60){
            myrtc.month = 2;            //February
            myrtc.day = days - 31;
        }
        else if(days <= 91){
            myrtc.month = 3;            //March
            myrtc.day = days - 60;
        }
        else if(days <= 121){
            myrtc.month = 4;            //April
            myrtc.day = days - 91;
        }
        else if(days <= 152){
            myrtc.month = 5;            //May
            myrtc.day = days - 121;
        }
        else if(days <= 182){
            myrtc.month = 6;            //June
            myrtc.day = days - 152;
        }
        else if(days <= 213){
            myrtc.month = 7;            //July
            myrtc.day = days - 182;
        }
        else if(days <= 244){
            myrtc.month = 8;            //August
            myrtc.day = days - 213;
        }
        else if(days <= 274){
            myrtc.month = 9;            //September
            myrtc.day = days - 244;
        }
        else if(days <= 305){
            myrtc.month = 10;            //October
            myrtc.day = days - 274;
        }
        else if(days <= 335){
            myrtc.month = 11;            //November
            myrtc.day = days - 305;
        }
        else{
            myrtc.month = 12;            //December
            myrtc.day = days - 335;
        }
    }
    else if(myrtc.year%4 != 0){              //For non-leap years
        if(days <= 31){
            myrtc.month = 1;            //January
            myrtc.day = days;
        }
        else if(days <= 59){
            myrtc.month = 2;            //February
            myrtc.day = days - 31;
        }
        else if(days <= 90){
            myrtc.month = 3;            //March
            myrtc.day = days - 59;
        }
        else if(days <= 120){
            myrtc.month = 4;            //April
            myrtc.day = days - 90;
        }
        else if(days <= 151){
            myrtc.month = 5;            //May
            myrtc.day = days - 120;
        }
        else if(days <= 181){
            myrtc.month = 6;            //June
            myrtc.day = days - 151;
        }
        else if(days <= 212){
            myrtc.month = 7;            //July
            myrtc.day = days - 181;
        }
        else if(days <= 243){
            myrtc.month = 8;            //August
            myrtc.day = days - 212;
        }
        else if(days <= 273){
            myrtc.month = 9;            //September
            myrtc.day = days - 243;
        }
        else if(days <= 304){
            myrtc.month = 10;            //October
            myrtc.day = days - 273;
        }
        else if(days <= 334){
            myrtc.month = 11;            //November
            myrtc.day = days - 304;
        }
        else{
            myrtc.month = 12;            //December
            myrtc.day = days - 334;
        }
    }
}



uint8_t get_rtc_seconds(void){
    calculate_date();
    return myrtc.seconds;
}

uint8_t get_rtc_minutes(void){
    calculate_date();
    return myrtc.minutes;
}

uint8_t get_rtc_hours(void){
    calculate_date();
    return myrtc.hours;
}

uint16_t get_rtc_year(void){
    calculate_date();
    return myrtc.year;
}

uint8_t get_rtc_month(void){
    calculate_date();
    return myrtc.month;
}

uint8_t get_rtc_day(void){
    calculate_date();
    return myrtc.day;
}

