#include <stdint.h>
#include <stdbool.h>
#include "idt.h"

#include "pic.h"

#define UPPER_HALF 0xFFFF0000
#define LOWER_HALF 0x0000FFFF
#define GATE_MASK 0x00000F00
#define PRIVILEGE_MASK 0x00006000
#define PRESENT_MASK 0x00008000

uint32_t getOffset (IdtEntry entry) {
    return (((uint32_t)(entry.high) << 16) | (uint32_t)(entry.low));
}

/* @TODO fix these
uint8_t getGateType (IdtEntry entry) {
    return (uint8_t)((entry.upper & GATE_MASK) >> 8);
}

uint8_t getPrivilegeLevels (IdtEntry entry) {
    return (uint8_t)((entry.upper & PRIVILEGE_MASK) >> 13);
}

bool isValid (IdtEntry entry) {
    return ((entry.upper & PRESENT_MASK) != 0);
}*/

extern void* idt_stub_table[];
IdtEntry idt[256];
idtr_t idtr;

void makeInterruptTable () {

    idtr.base = (uint32_t)(&idt);
    idtr.limit = sizeof(IdtEntry) * 256 - 1;

    for (uint8_t idx = 0; idx < 32 + 16; idx++) {
        idtSetDesc(idx, idt_stub_table[idx], 0x8E);
    }

    initPIC(32);

    __asm__ volatile ("lidt [%0]" : : "r"(&idtr));
    __asm__ volatile ("sti");
}

void idtSetDesc (uint8_t idx, void* isr, uint8_t flags) {
    IdtEntry *desc = &idt[idx];

    //set offset
    desc->low = (uint32_t)isr & 0xFFFF;
    desc->high = ((uint32_t)(isr) >> 16) & 0xFFFF;

    //set segment selector
    desc->selector = 0x08;

    desc->zero = 0;

    //set flags
    desc->flags = flags;
}

void isrHandler(isr_registers_t* state) {
} 

void irqHandler(isr_registers_t* state) {
    ackPIC(state->vec_idx);
}
