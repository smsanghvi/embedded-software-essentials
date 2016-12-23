In this project, we have implemented a home-environment monitoring system by making use of a combination of various sensors. 
Using the temperature sensor, LDR and CO sensor, we can effectively monitor the home environment and all of the above sensors have been 
interfaced to the FRDM KL25z using ADC. On crossing the set trigger points, the data is logged to the EEPROM.

The second part of our project deals with setting up a real-time clock based off the on-board 32.768 kHz clock and the time is displayed 
in real-time on a LCD module which we have interfaced and are running in a 4-bit mode. We also created a function to set an alarm in 
some pre-configured time and when the time reaches the set value, a buzzer buzzes. The buzzer continues till the user has snoozed 
the ‘alarm’ by waving his hand in front of the PIR sensor.

This project has been an amalgamation of all the good software constructs that we have learnt throughout the semester. This project 
makes use of constructs like logging, structures, enums, dma, profiling amongst others and has been a particularly exciting one to work on!
