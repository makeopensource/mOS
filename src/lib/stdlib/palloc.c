#include "palloc.h"
#include <stdint.h>
#include "stdlib/stdio.h"
#include "stdlib/string.h"
#include "video/VGA_text.h"

void handle_chunk(Chunk *c);
void sort_chunks(int len, Chunk *array);

void init_palloc() {
    int len = *(uint32_t *)MEM_STRUCT_ADDR;
    Chunk *chunk = (Chunk *)(MEM_STRUCT_ADDR + sizeof(uint32_t));

    sort_chunks(len, chunk);

    for (int i = 0; i < len; i++, chunk++) {
        handle_chunk(chunk);
    }
}

void handle_chunk(Chunk *c) {
}

//  5, 2 , 4 , 5 , 9 , 3
// [2, 5], 4 , 5 , 9 , 3
//  2,[4 , 5], 5 , 9 , 3
//  2, 4 ,[5 , 5], 9 , 3
//  2, 4 , 5 ,[5 , 9], 3
//  2, 4 , 5 , 5 ,[3 , 9]

// Insertion Sort; Theta(1) space complexity, Theta(1) time
// complexity if chunks are upper bounded by some constant
// (usually about 10)
void sort_chunks(int len, Chunk *array) {
    for (int i = 0; i < len; i++) {
        for (int j = i+1; j < len; j++) {
            if (array[i].base_lower < array[j].base_lower) {
                Chunk a = array[i];
                Chunk b = array[j];

                memcpy(&array[j], &a, sizeof(Chunk));
                memcpy(&array[i], &b, sizeof(Chunk));
            }
        }
    }
}
