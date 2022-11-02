#include <stdint.h>
#include <stdbool.h>
#include "idt.h"

#define UPPER_HALF 0xFFFF0000
#define LOWER_HALF 0x0000FFFF
#define GATE_MASK 0x00000F00
#define PRIVILEGE_MASK 0x00006000
#define PRESENT_MASK 0x00008000

uint32_t getOffset (IdtEntry entry) {
    return ((entry.upper & UPPER_HALF) | (entry.lower & LOWER_HALF));
}

uint16_t getSegmentSelector (IdtEntry entry) {
    return (uint16_t)(entry.lower &  UPPER_HALF);
}

uint8_t getGateType (IdtEntry entry) {
    return (uint8_t)((entry.upper & GATE_MASK) >> 8);
}

uint8_t getPrivilegeLevels (IdtEntry entry) {
    return (uint8_t)((entry.upper & PRIVILEGE_MASK) >> 13);
}

bool isValid (IdtEntry entry) {
    return ((entry.upper & PRESENT_MASK) != 0);
}

extern void* isr_stub_table[];
static IdtEntry idt[256];
static idtr_t idtr;

void makeInterruptTable () {

    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(IdtEntry) * 255;

    for (uint8_t idx = 0; idx < 32; idx++) {
        idtSetDesc(idx, isr_stub_table[idx], 0x8E);
    }

    __asm__ volatile ("lidt [0]" : : "m"(idtr));
    __asm__ volatile ("sti");
}

void idtSetDesc (uint8_t idx, void* isr, uint8_t flags) {
    IdtEntry *desc = &idt[idx];

    //set offset
    desc->lower = (uint32_t)isr & 0xFFFF;
    desc->upper = (uint32_t)isr & 0xFFFF0000;

    //set segment selector
    desc->lower |= 0x08 << 16;

    //set flags 1xx0xxxx
    desc->upper |= (uint32_t)flags << 8;
} 

void exceptionHandler() {
    __asm__ volatile ("cli; hlt");
} 
