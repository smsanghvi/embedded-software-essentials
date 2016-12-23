In this project, I have configured the DMA peripheral to transfer data so that the processor can do other work and its time doesnt
have to be utilized in transferring data. Two functions namely memmove and memzero have been tested for transferring different number of
bytes using DMA and have been profiled to determine the time that it took for the transfer. As expected, the time to transfer data using 
DMA came out to be much lesser since it a hardware peripheral which offloads some of the work of the processor. Both, 8 bit transfers and
32 bit transfers have been implemented.

Apart from that, a message interface has been setup for the UART to send basic commands like changing the LED color, adjusting the
brightness, printing to the UART terminal, acknowleding or not acknowledging a packet. The packet structure consists of a command id, 
length of packet, data to be sent with variable length depending on command id and a checksum to ensure data integrity.

Finally, a firmware-level interface has been designed for the SPI peripheral on FRDM KL25z which has functions like spi_init, spi_read_write
and spi_checkstatus. Next, a library has been created for the NORDIC nRF24L01+ RF module which has a SPI interface. This higher level
library makes use of the lower-level SPI firmware interface that has already been designed to communicate with the FRDM KL25z and
Beaglebone Black.
