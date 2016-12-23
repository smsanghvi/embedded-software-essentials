This project involves configuring the UART functionality of the FRDM KL25z to accept inputs from the keyboard to control the brightness
and color of the on-board RGB LED. Both polling and interrupts have been implemented with the UART and also, wrapper functions (or LOG functions) have been created to
print out directly to the terminal.

The circular buffer (data structure that writes to one end and reads from the other till the head and tail meet) has been implemented and 
a unit test suite has been created to extensively test the buffer. There are functions to initialize the buffer in heap memory, add 
item to it, remove item from the buffer and checking for buffer full and empty conditions. Tests have also been arried out to check
if there is a wraparound at the edge cases.

There is also a profiler that has been developed that gives the amount of CPU cycles and time (in us) it takes to execute a function
and changing the number of bytes that need to be processed by these functions. The profiler is a really useful tool to estimate what
routines are taking the most number of CPU cycles - so that these can be optimized. A compile-time switch has been created to switch the
profiler between Beaglebone Black and FRDM KL25z architectures.
