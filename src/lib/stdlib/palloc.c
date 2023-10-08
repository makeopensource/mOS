#include "palloc.h"
#include <stdint.h>

void init_pages() {
    int len = *(uint32_t *)MEM_STRUCT_ADDR;
    Chunk *chunk = (Chunk *)(MEM_STRUCT_ADDR + sizeof(int));
    for (int i = 0; i < len; i++, chunk += sizeof(Chunk)) {
        // handle memory chunks
    }
}
