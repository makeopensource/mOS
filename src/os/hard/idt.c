#include <stdint.h>
#include <stdbool.h>
#include "idt.h"

#include "pic.h"

#define UPPER_HALF 0xFFFF0000
#define LOWER_HALF 0x0000FFFF
#define GATE_MASK 0b00001111
#define PRIVILEGE_MASK 0b01100000
#define PRESENT_MASK 0b10000000

#define DEFAULT_FLAGS 0x8E
#define CODE_SEGMENT 0x8


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
static IdtEntry idt[IDT_ENTRIES];
static idtr_t idtr;

void makeInterruptTable () {

    idtr.base = (uint32_t)(&idt);
    idtr.limit = sizeof(idt) - 1;

    for (uint8_t idx = 0; idx < ISR_COUNT + IRQ_COUNT; idx++) {
        idtSetDesc(idx, idt_stub_table[idx], DEFAULT_FLAGS);
    }

    initPIC(32);

    __asm__ volatile ("lidt [%0]" : : "r"(&idtr));
    enableInterrupts();
}

void idtSetDesc (uint8_t idx, void* isr, uint8_t flags) {
    IdtEntry *desc = &idt[idx];

    //set offset
    desc->low = (uint32_t)isr & 0xFFFF;
    desc->high = ((uint32_t)(isr) >> 16) & 0xFFFF;

    //set segment selector
    desc->selector = CODE_SEGMENT;

    desc->zero = 0;

    //set flags
    desc->flags = flags;
}

static int_handler_t isrHandlers[ISR_COUNT] = {};
static int_handler_t irqHandlers[IRQ_COUNT] = {};


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