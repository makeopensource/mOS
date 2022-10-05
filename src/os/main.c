#include "video/VGA_text.h"

int os_main(){

    clearScreen(black);

    writeText("Welcome To mOS!", (80 - 15)/2, 5, red);

    println("It booted!!!", green);

    VGA_Color colour = light_cyan;
    const char *string = "Hello, World!";
    println(string, colour);
    
    while (1==1) 
        ;
        
    return 0;
}
