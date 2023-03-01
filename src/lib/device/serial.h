#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define SERIAL_BUFFER_SIZE 256

// serial ports ports
#define COM1 0x03f8
#define COM2 0x02f8

// some convenience baud rates (these are divisors)
#define BAUD_115200 1
#define BAUD_57600 2
#define BAUD_38400 3
#define BAUD_19200 6
#define BAUD_14400 8
#define BAUD_9600 12
#define BAUD_4800 24
#define BAUD_2400 48
#define BAUD_1200 96
#define BAUD_600 192
#define BAUD_300 384
#define BAUD_100 1152

// default baud rate to use
#ifndef DEFAULT_BAUD
#define DEFAULT_BAUD BAUD_9600
#endif

// initializes COM1 and COM2 with DEFAULT_BAUD
// we are using 8 data bits with 1 stop.
void serialInit();

// set the baud rate of a serial port and initialize
void serialSetBaud(uint16_t port, uint16_t baud);

// non-blocking read, returns 0 if buffer empty
uint8_t serialReadByte(uint16_t port);

// waits for a byte to be read
uint8_t serialReadByteBlocking(uint16_t port);

// reads n bytes into buffer, blocking until n recieved
void serialRead(uint16_t port, uint8_t* buffer, size_t n);

// returns the amount of bytes ready to be read in the buffer
size_t serialReadReady(uint16_t port);

bool serialEmpty(uint16_t port);


// IMPORTANT!!! calling any send function before serialSend has finished is undefined behavior,
// you have been warned.

// sends a byte over serial, blocking
void serialWriteByte(uint16_t port, uint8_t data);

// sends n bytes over serial, blocking
void serialWriteBlocking(uint16_t port, uint8_t* data, size_t n);

// sends n bytes over serial, non-blocking
// WARNING, WILL ENABLE INTERRUPTS
void serialWrite(uint16_t port, uint8_t* data, size_t n);

// returns true if the async serial send has finished
bool serialWriteReady(uint16_t port);

#endif