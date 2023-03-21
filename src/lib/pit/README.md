# The PIT (Programmable interval timer)

- The PIT is a hardware device that consists of an ocillator a prescaler and three independent frequency dividers.
- Frequency divider:
	- Each pulse from input into the chip decreases the count, then after the count hits zero a pulse is sent to output. Thus making the frequency smaller.
	
	
	
## Outputs
- The PIT chip has three outputs,
	- Channel 0
		- This is the only one we are actually concerened about because it generates a IRQ0
	- Channel 1
		- Generally unused in this day and age (certainly unused for our purposes)
	- Channel 2
		- Used to produce sound from a speaker
		
## IO Ports
- There are four ports, Three for reading and writing to the channels and one as a write only command register
