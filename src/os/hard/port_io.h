#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;

    // ret takes the value from al, port is put into dx
    __asm__ volatile("inb %0, %w1" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;

    // ret takes the value from ax, port is put into dx
    __asm__ volatile("inw %0, %w1" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    // val is put into al, port is put into dx
    __asm__ volatile("outb %w1, %0" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val) {
    // val is put into ax, port is put into dx
    __asm__ volatile("outw %w1, %0" : : "a"(val), "Nd"(port));
}

static inline void port_io_wait(void) {
    // 0x80 is just a random unused port
    outb(0x80, 0);
}

#endif