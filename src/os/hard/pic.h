#ifndef PIC_H
#define PIC_H

#define PIC0_CTRL 0x20
#define PIC0_DATA 0x21

#define PIC1_CTRL 0xA0
#define PIC1_DATA 0xA1

// offset should be atleast 32 to avoid ISR and IRQ conflicts
void initPIC(unsigned char offset);

// send EOI (acknowledge) to the PIC
void ackPIC(int irq);

#endif