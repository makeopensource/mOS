#include "pit.h"
#include "../../os/hard/port_io.h"
#include <stdint.h>

int timer_ticks = 0;

void timer_handler(isr_registers_t *regs) {
	/* Increment 'Tick count' */
	timer_ticks++;
}


void init_pit(){
	irqSetHandler(0, timer_handler);
	init_timer(100);	// 100 hz is just a good general frequency according to the wiki
}


void init_timer(int hz) {
	int divisor = OSCILLATIONS / hz;	/* divisor */
	outb(0x43, 0x36);	                /* Write 0b00110110 to the command register */
	outb(0x40, divisor & 0xFF);       /* Set low byte of divisor */
	outb(0x40, divisor >> 8);	        /* Set high byte of divisor */
}


void sleep(uint32_t dur){
	uint32_t start = timer_ticks;

	/* Timer ticks increases continuosly */
	while(timer_ticks - start <= dur) {
		__asm__("hlt");
	}
}
