#include "ring_buffer.h"

// yay, undefined behavior reliance!
typedef ring_buffer(2) _ring_buffer_placeholder_t;

// initializes the container, size is size, stride is sizeof(data_t)
void ring_buffer_init(void* ringbuf, size_t size) {
    _ring_buffer_placeholder_t* rb = (_ring_buffer_placeholder_t*)(ringbuf);

    rb->start = 0;
    rb->end = 0;
    rb->capacity = size;
    rb->used = 0;
    
    for (int i = 0; i < size; ++i) {
        rb->buffer[i] = 0;
    }
}

bool ring_buffer_full(const void* ringbuf) {
    const _ring_buffer_placeholder_t* rb = (_ring_buffer_placeholder_t*)(ringbuf);
    return rb->used == rb->capacity;
}

bool ring_buffer_empty(const void* ringbuf) {
    const _ring_buffer_placeholder_t* rb = (_ring_buffer_placeholder_t*)(ringbuf);
    return rb->used == 0;
}

// appends a new element to the ring buffer,
// will overwrite when at capacity
void ring_buffer_push(void* ringbuf, uint8_t elem) {
    _ring_buffer_placeholder_t* rb = (_ring_buffer_placeholder_t*)(ringbuf);
    
    rb->buffer[rb->end] = elem;
    
    // update end
    ++rb->end;
    rb->end %= rb->capacity;

    // update used amount
    ++rb->used;
    if (rb->used > rb->capacity) {
        // adjust start
        rb->start += rb->used - rb->capacity;
        rb->start %= rb->capacity;

        rb->used = rb->capacity;
    }
}

// removes the top element of the buffer, 0 if empty
uint8_t ring_buffer_pop(void* ringbuf) {
    _ring_buffer_placeholder_t* rb = (_ring_buffer_placeholder_t*)(ringbuf);

    // edge-case of empty buffer
    if (ring_buffer_empty(ringbuf)) return 0;

    uint8_t out = rb->buffer[rb->start];

    ++rb->start;
    rb->start %= rb->capacity;

    --rb->used;

    return out;
}

// returns the top element of the buffer, 0 if empty
uint8_t ring_buffer_top(const void* ringbuf) {
    const _ring_buffer_placeholder_t* rb = (_ring_buffer_placeholder_t*)(ringbuf);

    if (ring_buffer_empty(ringbuf)) return 0;

    return rb->buffer[rb->start];
}