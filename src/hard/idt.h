#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t upper;
    uint32_t lower;
} IdtEntry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} idtr_t;

// upper half of upper (16-31), lower half of lower (0-15)
uint32_t getOffset(IdtEntry entry); //addr of entry point of ISR
uint16_t getSegmentSelector(IdtEntry entry); //upper half of lower (0-15)
uint8_t getGateType(IdtEntry entry); //bits 8-11 of upper
uint8_t getPrivilegeLevels (IdtEntry entry); //bits 13-14 of upper
bool isValid(IdtEntry entry); //check present bit (upper's 15th bit)

void makeInterruptTable ();
void idtSetDesc (uint8_t idx, void* isr, uint8_t flags); 
