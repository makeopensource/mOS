#include "ps2.h"
#include "../../os/hard/port_io.h"
#include "../../os/hard/idt.h"

#include "container/ring_buffer.h"

typedef ring_buffer(64) ps2_buffer_t;

ps2_buffer_t PS2Port1;
ps2_buffer_t PS2Port2;

static bool secondDeviceEnabled = true;

static bool port1active = false;
static bool port2active = false;

static struct PS2Device dev1;
static struct PS2Device dev2;

void ps2HandlerPort1(isr_registers_t* regs) {
    uint8_t b = inb(PS2_DATA);
    ring_buffer_push(&PS2Port1, b);
}
void ps2HandlerPort2(isr_registers_t* regs) {
    uint8_t b = inb(PS2_DATA);
    ring_buffer_push(&PS2Port2, b);
}

int readController() {
    return inb(PS2_STAT_CMD);
}

int readData() {
    return inb(PS2_DATA);
}

struct PS2Device *getPortType(int portnum) {
    if (portnum == 1) {
        return &dev1;
    }
    else if (portnum == 2) {
        return &dev2;
    }
    else {
        return NULL;
    }
}

int writeController(uint8_t b) {
    for (int i = 0; (readController() & 0b0010) != 0; i++) { //check status of ps2 write buffer
        if (i >= PS2_TIMEOUT) return -1; //if past timeout, error
    }
    outb(PS2_STAT_CMD, b); //status and command are the same port (don't ask me why)
    return 0;
}

int writePort(uint8_t b) {
    for (int i = 0; (readController() & 0b0010) != 0; i++) { //check status of ps2 write buffer
        if (i >= PS2_TIMEOUT) return -1; //if past timeout, error
    }
    outb(PS2_DATA, b);
    return 0;
}

bool writePortWithACK(uint8_t b) {
    if (0 > writePort(b)) return false;
    for (int i = 0; ring_buffer_empty(&PS2Port1); i++) {
        if (i >= PS2_TIMEOUT) return false;
    }
    if (ring_buffer_pop(&PS2Port1) != 0xFA) return false;
    return true;
}

int writePort2(uint8_t b) {
    if (0 > writeController(0xD4)) return -1;
    return writePort(b);
}

bool writePort2WithACK(uint8_t b) {
    if (0 > writePort2(b)) return false;
    for (int i = 0; ring_buffer_empty(&PS2Port2); i++) {
        if (i >= PS2_TIMEOUT) return false;
    }
    if (ring_buffer_pop(&PS2Port2) != 0xFA) return false;
    return true;
}

//

char readConfig() {
    if (0 > writeController(0x20)) return 0x0;
    return inb(PS2_DATA);
}

enum DeviceType translateDeviceType(uint8_t b) {
    switch (b)
    {
    case StandardMouse:
        return StandardMouse;
    case ScrollMouse:
        return ScrollMouse;
    case QuintMouse:
        return QuintMouse;
    case MF2Key1:
        return MF2Key1;
    case MF2Key2:
        return MF2Key2;
    case IBMShortKey:
        return IBMShortKey;
    case NCD97Or122:
        return NCD97Or122;
    case Standard122:
        return Standard122;
    case JPNGKey:
        return JPNGKey;
    case JPNPKey:
        return JPNPKey;
    case JPNAKey:
        return JPNAKey;
    case SunKey:
        return SunKey;
    default:
        return Unknown;
        break;
    }
}

int ps2Init() {
    ring_buffer_init(&PS2Port1, 64);
    ring_buffer_init(&PS2Port2, 64);
    irqSetHandler(1, ps2HandlerPort1);
    irqSetHandler(12, ps2HandlerPort2);

    disableInterrupts();
    //assuming a PS2 controller exists so I don't have to switch hardware modes just to check
    //also assuming that USB doesn't exist (should disable USB Legacy as soon as OS inits)
        //(USB support is probably a stretch goal)
    writeController(0xAD); //disable 1st device
    writeController(0xA7); //disable 2nd (if it exists)
    while (readController() & 0b0001) inb(PS2_DATA); //flush read buffer while not empty
    char stat = readConfig();
    if ((stat & 0b00100000) == 0) secondDeviceEnabled = false; //check if second device is supported
    stat = stat ^ 0b1000011; // disable interrupts and translation
    writeController(0x60);
    writeController(stat); //write new status config

    writeController(0xAA);
    if (readData() != 0x55)  {
        enableInterrupts();
        return -1; //test controller failed
    }
    
    writeController(0x60);
    writeController(stat); //write statis config again (some machines reset the controller after testing)

    if (secondDeviceEnabled) {
        writeController(0xA8);
        if ((readConfig() & 0b00100000) != 0) { //check whether second device can be enabled (i.e. it exists)
            secondDeviceEnabled = false;
        }
        else {
            writeController(0xA7);
        }
    }

    //testing devices themselves
    //erroring at sign of any error (there is probably a better way to do this)
    int numPorts;
    if (secondDeviceEnabled) numPorts = 2;
    else numPorts = 1;
    writeController(0xAB);
    if(readData() != 0x0) {
        enableInterrupts();
        numPorts--;
    }

    if (secondDeviceEnabled) {
        writeController(0xA9);
        if(readData() != 0x0) {
            enableInterrupts();
            numPorts--;
        }
    }

    if (numPorts == 0) {
        return -2;
    }

    stat = readConfig(); //reenable interrupts for PS2 devices
    writeController(0xAE);
    stat = stat | 0b0001;
    if (secondDeviceEnabled) {
        writeController(0xA8);
        stat = stat | 0b0010;
    }
    writeController(0x60);
    writeController(stat); //write new status config
    enableInterrupts();

    //reseting devices
    port1active = writePortWithACK(0xFF);
    if (!ring_buffer_empty(&PS2Port1) && ring_buffer_pop(&PS2Port1) != 0xAA) port1active = false;
    if (secondDeviceEnabled) {
        port2active = writePort2WithACK(0xFF);
        if (!ring_buffer_empty(&PS2Port2) && ring_buffer_pop(&PS2Port2) != 0xAA) port2active = false;
    }

    //determining device types
    if (port1active) {
        // Get 1st byte of type
        for (int i = 0; ring_buffer_empty(&PS2Port1); i++) {
            if (i >= PS2_TIMEOUT) return false;
        }
        uint8_t p1_b1 = ring_buffer_pop(&PS2Port1);
        if (p1_b1 == 0xAB || p1_b1 == 0xAC) { 
            dev1.isKeyboard = true;
            //get 2nd byte 
            for (int i = 0; ring_buffer_empty(&PS2Port1); i++) {
                if (i >= PS2_TIMEOUT) return false;
            }
            uint8_t p1_b2 = ring_buffer_pop(&PS2Port1);
            dev1.type = translateDeviceType(p1_b2);
            dev1.scancode = SC2;
        }
        else { 
            dev1.isKeyboard = false;
            dev1.type = translateDeviceType(p1_b1); 
            dev1.scancode = None;
        }
        writePortWithACK(0xF4);
    }
    else {
        dev1.isKeyboard = false;
        dev1.type = Unknown;
        dev1.scancode = None;
    }

    if (port2active) {
        // Get 1st byte of type
        for (int i = 0; ring_buffer_empty(&PS2Port2); i++) {
            if (i >= PS2_TIMEOUT) return false;
        }
        uint8_t p1_b1 = ring_buffer_pop(&PS2Port2);
        if (p1_b1 == 0xAB || p1_b1 == 0xAC) { 
            dev2.isKeyboard = 1;
            //get 2nd byte 
            for (int i = 0; ring_buffer_empty(&PS2Port2); i++) {
                if (i >= PS2_TIMEOUT) return false;
            }
            uint8_t p1_b2 = ring_buffer_pop(&PS2Port2);
            dev2.type = translateDeviceType(p1_b2);
            dev2.scancode = SC2;
        }
        else { 
            dev2.isKeyboard = false;
            dev2.type = translateDeviceType(p1_b1); 
            dev2.scancode = None;
        }
        writePort2WithACK(0xF4);
    }
    else {
        dev2.isKeyboard = false;
        dev2.type = Unknown;
        dev2.scancode = None;
    }

    if (!(port1active || port2active)) {
        return -2;   
    }

    return 0;
}

