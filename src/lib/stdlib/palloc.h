#include <stdint.h>

// RAM addr of structure describing useable memory 
#define MEM_STRUCT_ADDR 0x8000
#define MEM_FREE 0
#define MEM_RESERVED 1

#define PAGE_SIZE 4096

// for the purposes of our os, which is 32-bit, we 
// will rely soley on the lower 32-bits
typedef struct {
    uint32_t base_lower;
    uint32_t base_upper;
    uint32_t len_lower;
    uint32_t len_upper;
    uint32_t type;
    uint32_t attrs; // ACPI v3.0 Extended Attributes bitfield
} Chunk;


void init_palloc();

// Returns a PAGE_SIZE page of contiguous, useable memory
void *palloc();
