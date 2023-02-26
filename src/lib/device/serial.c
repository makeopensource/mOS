#include "serial.h"

#include "../../os/hard/port_io.h"
#include "../../os/hard/idt.h"

#include "container/ring_buffer.h"

typedef ring_buffer(SERIAL_BUFFER_SIZE) serial_buffer_t;

// input buffer
serial_buffer_t inCOM1;
serial_buffer_t inCOM2;

// output buffers
uint8_t* outCOM1 = NULL;
size_t remainCOM1 = 0;

uint8_t* outCOM2 = NULL;
size_t remainCOM2 = 0;

#define DATA_READY_MASK 0b1
#define TRANSMIT_READY_MASK 0b100000

void handlerStub(uint16_t port, serial_buffer_t* inBuf, uint8_t** outBuf, size_t* remainOut) {
    inb(port + 2); // what caused the interrupt, we dont really care

    uint8_t status;
    do {
        status = inb(port + 5);
        if (status & DATA_READY_MASK) {
            ring_buffer_push(inBuf, inb(port));
        }

        if ((status & TRANSMIT_READY_MASK) && *remainOut > 0) {
            outb(port, **outBuf);
            ++(*outBuf);
            --(*remainOut);
        }

    } while ((status & DATA_READY_MASK) || ((status & TRANSMIT_READY_MASK) && *remainOut > 0));
}

void serialHandler1(isr_registers_t* regs) {
    handlerStub(COM1, &inCOM1, &outCOM1, &remainCOM1);
}

void serialHandler2(isr_registers_t* regs) {
    handlerStub(COM2, &inCOM2, &outCOM2, &remainCOM2);
}


void serialInit() {
    ring_buffer_init(&inCOM1, SERIAL_BUFFER_SIZE);
    ring_buffer_init(&inCOM2, SERIAL_BUFFER_SIZE);

    irqSetHandler(4, serialHandler1);
    irqSetHandler(3, serialHandler2);
    
    serialSetBaud(COM1, DEFAULT_BAUD);
    serialSetBaud(COM2, DEFAULT_BAUD);
}

#define DATA8b 0b11
#define STOP1 0b000
#define PARITY_NONE 0b000000

// Since there is lacking internet sources of the FIFO control register, here's some data
// bit 0: enable
// bit 1: clear receiver FIFO
// bit 2: clear transmitter FIFO
// bit 3: DMA mode
// bits 6-7: trigger for DR interrupt,
// 0b00 = 1byte, 0b01 = 4 bytes, then 8, then 14

// port and baud are expected to be the provided macros in serial.h
void serialSetBaud(uint16_t port, uint16_t baud) {
    // a strange thing to note is that port and port + 1 change meaning,
    // the meaning is dependant on port + 3's most significant bit

    outb(port + 3, 0); // ensure DLAB is 0
    outb(port + 1, 0); // disable IRQs

    outb(port + 3, 0b10000000); // set DLAB to 1
    outb(port + 0, (uint8_t)(baud & 0x00ff)); // set baud divisor
    outb(port + 1, (uint8_t)((baud & 0xff00) >> 8));

    outb(port + 3, DATA8b | STOP1 | PARITY_NONE); // set data flags (also sets DLAB to 0)
    outb(port + 2, 0b11000111); // enable fifo and clear buffers and enable dr interrupt

    outb(port + 4, 0x0f); // enable hardware handshaking, kinda

    outb(port + 1, 0b11); // enable Data Ready (DR) and Transmit Empty (THRE) interrupts
}

/* read related functions */

uint8_t serialReadByte(uint16_t port) {
    outb(port + 1, 0b00); // disable serial interrupts
    if (port == COM1) {
        return ring_buffer_pop(&inCOM1);
    } else {
        return ring_buffer_pop(&inCOM2);
    }
    outb(port + 1, 0b11); // enable serial interrupts
}

uint8_t serialReadByteBlocking(uint16_t port) {
    while (serialEmpty(port));

    return serialReadByte(port);
}

void serialRead(uint16_t port, uint8_t* buffer, size_t n) {
    for (int i = 0; i < n; ++i) {
        buffer[i] = serialReadByteBlocking(port);
    }
}

uint8_t serialReadReady(uint16_t port) {
    if (port == COM1) {
        return inCOM1.used;
    } else {
        return inCOM2.used;
    }
}

bool serialEmpty(uint16_t port) {
    if (port == COM1) {
        return ring_buffer_empty(&inCOM1);
    } else {
        return ring_buffer_empty(&inCOM2);
    }
}


/* Write related functions */

bool transmitReady(uint16_t port) {
    return inb(port + 5) & TRANSMIT_READY_MASK; // check is transmission buffer is empty
}

void serialWriteByte(uint16_t port, uint8_t data) {
    while (!transmitReady(port));

    outb(port, data);
}

// sends n bytes over serial, blocking
void serialWriteBlocking(uint16_t port, uint8_t* data, size_t n) {
    for (int i = 0; i < n; ++i) {
        serialWriteByte(port, data[i]);
    }
}

// sends n bytes over serial, non-blocking
void serialWrite(uint16_t port, uint8_t* data, size_t n) {
    clearInterrupts(); // disable cpu interrupts
    outb(port + 1, 0b00); // disable serial interrupts

    if (port == COM1) {
        outCOM1 = data;
        remainCOM1 = n;
    } else {
        outCOM2 = data;
        remainCOM2 = n;
    }

    enableInterrupts();
    outb(port + 1, 0b11); // enable serial interrupts
}

bool serialWriteReady(uint16_t port) {
    if (port == COM1) {
        return remainCOM1 == 0;
    } else {
        return remainCOM2 == 0;
    }
}