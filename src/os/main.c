#include "hard/idt.h"

#include "video/VGA_text.h"
#include "pit/pit.h"

#include "device/serial.h"

int os_main(){
    makeInterruptTable();
		init_pit();
    serialInit();
    clearScreen(black);

    writeText("Welcome To mOS!", (80 - 15)/2, 5, red);

		println("It booted!!!", green);

    serialWrite(COM1, (uint8_t*)("Hello, Serial!"), sizeof("Hello, Serial!"));

    VGA_Color colour = light_cyan;
    const char *string = "Hello, World!";
    println(string, colour);
    
    while (1 + 1 == 2)
        ;
        
    return 0;
}
