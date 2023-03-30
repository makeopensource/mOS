#include "hard/idt.h"

#include "video/VGA_text.h"

#include "device/serial.h"

#include "test.h"

int os_main() {
    makeInterruptTable();
    serialInit();


    clearScreen(black);

    writeText("Welcome To mOS!", (80 - 15)/2, 5, red);

    println("It booted!!!", green);

    VGA_Color colour = light_cyan;
    const char *string = "Hello, World!";
    println(string, colour);
    
    uint8_t buf[256];

    while (1 + 1 == 2) {
        
        size_t read = serialReadReady(COM1);
        if (read > 256) read = 256;

        serialRead(COM1, buf, read);

        if (buf[0] == 't' && buf[1] == 'e' && buf[2] == 's' && buf[3] == 't' && buf[4] == '\0') {
            enterTesting();
        }
    }
    
        
    return 0;
}
