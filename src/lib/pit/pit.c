#include "pit.h"
#include <stdint.h>

uint32_t timer_ticks = 0;

uint32_t get_ticks() {
	return timer_ticks;
}

void timer_handler(isr_registers_t *regs) {
	/* Increment 'Tick count' */
	timer_ticks++;
}


void init_pit(){
	irqSetHandler(0, timer_handler);
	init_timer(100);	// 100 hz is just a good general frequency according to the wiki
}


void init_timer(int hz) {
	disableInterrupts();
	int divisor = OSCILLATIONS / hz;	/* divisor */
	outb(CMD_REG_PORT, 0x36);	                /* Write 0b00110110 to the command register */
	outb(CHAN_0_PORT, divisor & 0xFF);       /* Set low byte of divisor */
	outb(CHAN_0_PORT, divisor >> 8);	        /* Set high byte of divisor */
	enableInterrupts();
}


void sleep(uint32_t dur){
	uint32_t start = timer_ticks;

	/* Timer ticks increases continuosly */
	while(timer_ticks - start < dur) {
		__asm__("hlt");
	}
}
