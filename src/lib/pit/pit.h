#ifndef PIT_H
#define PIT_H

#include "../../os/hard/idt.h"
#include "../../lib/video/VGA_text.h"
#include <stdint.h>

#define OSCILLATIONS 1193180 // The PIT oscillates this many HZ

/***
 *  Handles the timer
 */
void timer_handler(isr_registers_t *regs);

/***
 *	Sets up the system clock by installing the
 *	timer handler into IRQ0
 */
void init_pit();

/***
 *	Setting the freqency for the timer
 */
void init_timer(int hz);

/***
 *
 *	Sleep function to halt for dur.
 *  note that how long it sleeps for dur dpeends on what freq the timer is set at
 */
void sleep(uint32_t dur);

#endif
