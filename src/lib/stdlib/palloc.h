#include <stdint.h>

// RAM addr of structure describing useable memory 
#define MEM_STRUCT_ADDR 0x8000
#define MEM_FREE 0
#define MEM_RESERVED 1

typedef struct {
    uint64_t base;
    uint64_t len;
    uint32_t type;
    uint32_t attrs; // ACPI v3.0 Extended Attributes bitfield
} Chunk;
