#include "hard/idt.h"

#include "video/VGA_text.h"
#include "pit/pit.h"

int os_main(){
    makeInterruptTable();
		init_pit();
		
    clearScreen(black);

    writeText("Welcome To mOS!", (80 - 15)/2, 5, red);

		println("It booted!!!", green);

    VGA_Color colour = light_cyan;
    const char *string = "Hello, World!";
    println(string, colour);

		//	 println("Timer Start", red);
		 //		sleep(100);
		//		println("Timer End", red);

    while (1==1)
        ;
        
    return 0;
}
