#include "VGA_text.h"

VGA_Char* cursor = VGA_MEMORY;

VGA_Char getVGAchar(unsigned char chr, VGA_Color foreground, VGA_Color background) {
    VGA_Char out = { chr, ((background << 4) | foreground) };
    return out; 
}


void writeText(const char* str, int x, int y, VGA_Color color) {
    // culling
    if (x < 0 || x >= VGA_WIDTH) return;
    if (y < 0 || y >= VGA_HEIGHT) return;

    VGA_Char* buffer = VGA_MEMORY + x + VGA_WIDTH * y;

    unsigned short maxLen = VGA_WIDTH - x; // also culling
    for (unsigned short i = 0; i < maxLen && *str != '\0'; ++i) {
        // preserve background
        VGA_Char chr = { *str++, color ^ (buffer[i].color & 0xf0) };
        buffer[i] = chr;
    }
}


// checks and fixes the cursor, may do nothing
void adjustCursor() {
    if (cursor < VGA_MEMORY) {
        cursor = VGA_MEMORY;
    } else if (cursor >= VGA_END) {
        scroll();

        //set cursor to start of last line
        cursor = VGA_MEMORY + VGA_SIZE - VGA_WIDTH;
    }
}

void print(const char* str, VGA_Color color) {
    while( *str != 0 )
    {
        // preserve background
        VGA_Char chr = { *str++, color ^ (cursor->color & 0xf0) };
        *cursor++ = chr;

        // prevent writing out of bounds!
        adjustCursor();
    }
}

void println(const char* str, VGA_Color color) {
    print(str, color);

    // update cursor position to next line
    cursor += VGA_WIDTH;
    int remain = (int)(cursor - VGA_MEMORY) % (VGA_WIDTH);
    cursor -= remain;

    adjustCursor(); // bounds check
}

void scroll() {
    cursor -= VGA_WIDTH; // move cursor up
    adjustCursor(); // bounds check

    VGA_Char* current = VGA_MEMORY;
    VGA_Char* next = VGA_MEMORY + VGA_WIDTH;

    // move all lines down
    for (unsigned short i = 0; i < VGA_SIZE - VGA_WIDTH; ++i) {
        current[i] = next[i];
    }

    // replace bottom
    VGA_Char bottomChr = { ' ', black };
    for (unsigned short i = VGA_SIZE - VGA_WIDTH; i < VGA_SIZE; ++i) {
        current[i] = bottomChr;
    }
}


void clearScreen(VGA_Color color) {
    VGA_Char clearChr = getVGAchar(' ', black, color);

    clearScreenC(clearChr);    
}

void clearScreenC(VGA_Char character) {
    for (unsigned short i = 0; i < VGA_SIZE; ++i) {
        VGA_MEMORY[i] = character;
    }
    cursor = VGA_MEMORY; // reset cursor
}