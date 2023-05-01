#include "ps2.h"
#include "../../os/hard/port_io.h"
#include "../../os/hard/idt.h"

#include "container/ring_buffer.h"

typedef ring_buffer(64) ps2_buffer_t;

ps2_buffer_t PS2Port1;
ps2_buffer_t PS2Port2;

struct PS2Device dev1;
struct PS2Device dev2;


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

int ps2Init() {
    ring_buffer_init(&PS2Port1, 64);
    ring_buffer_init(&PS2Port2, 64);
    irqSetHandler(1, ps2HandlerPort1);
    irqSetHandler(12, ps2HandlerPort2);

    return 0;
}

