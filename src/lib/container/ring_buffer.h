#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Header only generic ring buffer data structure
// to use larger elements, multiply your size by sizeof(data_t),
// then call push/pop sizeof(data_t) times. Beware of endianess.

// The magic generic macro, size > 0, otherwise behavior is undefined
// we do have to store the length since C lacks generic support.
#define ring_buffer(size) \
struct { \
    size_t start; \
    size_t end; \
    size_t capacity; \
    size_t used; \
\
    uint8_t buffer[size]; \
}

// initializes the container, size is size, stride is sizeof(data_t)
void ring_buffer_init(void* ringbuf, size_t size);

bool ring_buffer_full(const void* ringbuf);
bool ring_buffer_empty(const void* ringbuf);

// appends a new element to the ring buffer,
// will overwrite when at capacity
void ring_buffer_push(void* ringbuf, uint8_t elem);

// removes the top element of the buffer, 0 if empty
uint8_t ring_buffer_pop(void* ringbuf);

// returns the top element of the buffer, 0 if empty
uint8_t ring_buffer_top(const void* ringbuf);

#endif