#include "ps2.h"
#include "../../os/hard/port_io.h"
#include "../../os/hard/idt.h"

#include "container/ring_buffer.h"

#include "video/VGA_text.h"

#define PS2_BUF_SIZE 64

typedef ring_buffer(PS2_BUF_SIZE) ps2_buffer_t;

ps2_buffer_t PS2Port1;
ps2_buffer_t PS2Port2;

struct PS2Device dev1;
struct PS2Device dev2;

void ps2HandlerPort1(isr_registers_t* regs) {
    uint8_t b = inb(PS2_DATA);

    char buf[2] = " ";
    buf[0] = b;
    print(buf, red);

    ring_buffer_push(&PS2Port1, b);
}
void ps2HandlerPort2(isr_registers_t* regs) {
    uint8_t b = inb(PS2_DATA);
    ring_buffer_push(&PS2Port2, b);
}

enum DeviceType translateDeviceType(uint8_t b) {
    switch (b) {
    case StandardMouse:
    case ScrollMouse:
    case QuintMouse:
    case MF2Key1: case MF2Key2:
    case IBMShortKey:
    case NCD97Or122:
    case Standard122:
    case JPNGKey: case JPNPKey: case JPNAKey:
    case SunKey:
        return (enum DeviceType)(b);
    default:
        return Unknown;
    }
}



uint8_t readStat(void) {
    return inb(PS2_STAT_CMD);
}

void sendCMD(uint8_t b) {
    // await ready
    while (readStat() & 0b10) ;

    outb(PS2_STAT_CMD, b);
}

bool dataReady(void) {
    uint8_t status = readStat();
    return status & 1;
}

uint8_t readData(void) {
    // await ready
    while (!dataReady()) ;

    return inb(PS2_DATA);
}

void sendData(uint8_t b) {
    while (readStat() & 0b10) ;

    outb(PS2_DATA, b);
}

void ps2Disable(void) {
    sendCMD(0xAD);
    sendCMD(0xA7);
}

// ps2 should be disabled when this is called
void ps2Flush(void) {
    while (dataReady()) {
        // discard all data
        inb(PS2_DATA);
    }
}

uint8_t getConf(void) {
    sendCMD(0x20);
    return readData();
}

void setConf(uint8_t newconf) {
    sendCMD(0x60);
    sendData(newconf);
}

int ps2Init() {
    ring_buffer_init(&PS2Port1, PS2_BUF_SIZE);
    ring_buffer_init(&PS2Port2, PS2_BUF_SIZE);

    ps2Disable();
    ps2Flush();

    uint8_t conf = readConf();

    // see if 2nd port is potentially present
    bool port2present = conf & 0b00100000;

    setConf(conf & ~(0b01000011)); // disable IRQs

    // test controller
    sendCMD(0xAA);
    uint8_t ctest = readData();

    // controller test failed! PS2 broken!
    if (ctest == 0xFC) return ERR_PS2_BROKEN;

    setConf(readConf() & ~(0b01000011)); // set config again

    if (port2present) {
        sendCMD(0xA8); //enable port2

        //check configuration to see if port2 exists
        if (readConf() & 0b00100000) {
            port2present = false;
        }
        else {
            sendCMD(0xA7); //disable port2
        }
    }

    // test port 1
    sendCMD(0xAB);
    uint8_t res1 = readData();
    bool port1works = res1 == 0;

    // test port 2
    bool port2works = false;
    if (port2present) {
        sendCMD(0xA9);
        uint8_t res2 = readData();
        port2works = res2 == 0;
    }

    // set handlers and enable
    if (port1works) {
        irqSetHandler(1, ps2HandlerPort1);
        sendCMD(0xAE);
    }

    if (port2works) {
        irqSetHandler(12, ps2HandlerPort2);
        sendCMD(0xA8);
    }
   
    setConf(readConf() | 0b01000011); //enable IRQs

    return 0;
}

