#include "pic.h"

#include "port_io.h"

uint8_t PIC0_mask = 0x0;
uint8_t PIC1_mask = 0x0;

void initPIC(unsigned char offset) {

    // disable PIC
    outb(PIC0_DATA, 0xff);
    outb(PIC1_DATA, 0xff);

    outb(PIC0_CTRL, 0x11); // send init command
    outb(PIC0_DATA, offset);
    outb(PIC0_DATA, 4);    // set PIC1 location
    outb(PIC0_DATA, 0x01); // use 8086 mode

    outb(PIC1_CTRL, 0x11); // send init command
    outb(PIC1_DATA, offset + 8);
    outb(PIC1_DATA, 42);   // set PIC1 location
    outb(PIC1_DATA, 0x01); // use 8086 mode

    // restore masks
    outb(PIC0_DATA, PIC0_mask);
    outb(PIC1_DATA, PIC1_mask);
}

void ackPIC(int irq) {

    outb(PIC0_CTRL, 0x20);

    if (irq >= 8)
        outb(PIC1_CTRL, 0x20);
}