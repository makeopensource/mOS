#include "palloc.h"
#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAX_CHUNKS 100

void handle_chunk(Chunk *c);
void sort_chunks(int len, Chunk *array);
void merge_chunks(Chunk *chunks, size_t len);
int compar(const void *ina, const void *inb);

void init_palloc() {
    // last entry is repeat of first
    size_t len = *(uint32_t *)MEM_STRUCT_ADDR - 1;

    if (len > MAX_CHUNKS)
        len = MAX_CHUNKS;

    Chunk *chunks = (Chunk *)(MEM_STRUCT_ADDR + sizeof(uint32_t));

    isort(chunks, len, sizeof(Chunk), compar);
    // merge_chunks(chunks, len);
    // remove_duplicates(chunks, len)
}

// sorts first by type, then by base address
//
// Types:
// - type 1: usable memory
// - type > 1: unusable (some types are reclaimable, but is left unimplemented for now)
int compar(const void *ina, const void *inb) {
    Chunk a = *(Chunk *)ina;
    Chunk b = *(Chunk *)inb;

    if (a.type < b.type)
        return -1;
    else if (a.type > b.type)
        return 1;
    else if (a.base_lower < b.base_lower)
        return -1;
    else if (a.base_lower == b.base_lower) {
        ((Chunk *)inb) -> type = 2; // a bit janky for sure: changes duplicate to unusable type
        return -1;
    }
    return 1;
}

// void merge_chunks(Chunk *chunks, size_t len) {
//     if (len < 2)
//         return;
// 
//     for (int i = 0; i < len - 1; i++) {
//         Chunk curr = chunks[i];
//         Chunk next = chunks[i+1];
// 
//         if (curr.base_lower + curr.len_lower < next.base_lower) 
//             continue;
// 
//         // check if two chunks intersect (after sort)
//         else if (curr.base_lower >= next.base_lower) // This should never happen?????
//             return;
// 
//         else if (curr.base_lower + curr.len_lower >= next.base_lower) {
//             
//         }
//     }
// }
