#include <stdint.h>
#include <stdbool.h>
#include "idt.h"

#include "pic.h"

#define UPPER_HALF 0xFFFF0000
#define LOWER_HALF 0x0000FFFF
#define GATE_MASK 0b00001111
#define PRIVILEGE_MASK 0b01100000
#define PRESENT_MASK 0b10000000

uint32_t getOffset (IdtEntry entry) {
    return (((uint32_t)(entry.high) << 16) | (uint32_t)(entry.low));
}

uint8_t getGateType (IdtEntry entry) {
    return entry.flags & GATE_MASK;
}

uint8_t getPrivilegeLevels (IdtEntry entry) {
    return (entry.flags & PRIVILEGE_MASK) >> 5;
}

bool isValid (IdtEntry entry) {
    return ((entry.flags & PRESENT_MASK) != 0);
}

extern void* idt_stub_table[];
static IdtEntry idt[256];
static idtr_t idtr;

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

static int_handler_t isrHandlers[32] = {};
static int_handler_t irqHandlers[16] = {};


void isrSetHandler(uint8_t isr_vec, int_handler_t handler) {
    isrHandlers[isr_vec] = handler;
}

void irqSetHandler(uint8_t irq_vec, int_handler_t handler) {
    irqHandlers[irq_vec] = handler;
}

void isrHandler(isr_registers_t* state) {

    int_handler_t handler = isrHandlers[state->vec_idx];
    if (handler != 0) {
        handler(state);
    }
} 

void irqHandler(isr_registers_t* state) {

    int_handler_t handler = irqHandlers[state->vec_idx];
    if (handler != 0) {
        handler(state);
    }

    ackPIC(state->vec_idx);
}

void disableInterrupts(void) {
    __asm__ volatile ("cli");
}

void enableInterrupts(void) {
    __asm__ volatile ("sti");
}