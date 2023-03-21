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

|Port|Usage|
|----|-----|
|0x40| Channel 0 data (Read and Write)|
|0x41| Channel 1 data (Read and Write)|
|0x42| Channel 2 data (Read and Write)|
|0x43| Command Register (Write Only) |

### Channels:
	- These are used to set the 16 bit reload value or read a channels count.

### Command Register:
	- Takes 8 bits and is used to describe how a channel operates
	- The following table shows what each bit is used for.
	
	
| Bits    | Usage            |
|---------|------------------|
| 6 and 7 | Select Channel:  |
|         | 0 0 = Channel 0  |
|         | 0 1 = Channel 1  |
|         | 1 0 = Channel 2  |
|         | 1 1 = Read-Back command |
|         |                  |
| 4 and 5 | Access Mode:     |
|         | 0 0 = Latch count value command |
|         | 0 1 = Access Mode: lobyte only |
|         | 1 0 = Access Mode: hibyte only |
|         | 1 1 = Access Mode: lobyte/hibyte |
| 1 to 3  | Operating Mode: |
|         | 0 0 0 = Mode 0 (interrupt on terminal count) |
|         | 0 0 1 = Mode 1 (Hardware re-triggerable one-shot |
|         | 0 1 0 = Mode 2 (rate generator) |
|         | 0 1 1 = Mode 3 (square wave generator ) |
|         | 1 0 0 = Mode 4 (software triggered strobe) |
| 0       | BCD/Binary Mode: |
|         | 0 = 16-bit binary |
|         | 1 = four-digit BCD | 


### Operating Modes:
	- The mode we use for our timer is the square wave generator
		we could've used the rate generator but the wiki reccomends the 
		square wave generator instead
	- The square wave generator essentially acts as a frequency divider
		where the output signal is a square wave
