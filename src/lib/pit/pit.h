#ifndef PIT_H
#define PIT_H

#include "../../os/hard/port_io.h"
#include "../../os/hard/idt.h"
#include <stdint.h>

#define OSCILLATIONS 1193180 // The PIT oscillates this many HZ
#define CHAN_0_PORT 0x40
#define CMD_REG_PORT 0x43
/***
 *  Handles the timer
 */
static void timer_handler(isr_registers_t *regs);

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

/***
 *	Gets the current value of timer_ticks
 */
uint32_t get_ticks();

#endif
