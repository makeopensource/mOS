#include "../stdlib/stdio.h"

#include "../pit/pit.h"
#include "disk.h"
#include "port_io.h"

static void insw(uint16_t reg, uint8_t *buffer, int bytes) {
    for(int i = 0; i < bytes; i++) {
        buffer[i] = inb(reg);
    }
}

void ide_initialize(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2,
                    unsigned int BAR3, unsigned int BAR4) {
    int i, j, k, count = 0;

    channels[ATA_PRIMARY].base = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
    channels[ATA_PRIMARY].ctrl = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
    channels[ATA_SECONDARY].base = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
    channels[ATA_SECONDARY].ctrl = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
    channels[ATA_PRIMARY].bmide = (BAR4 & 0xFFFFFFFC) + 0;
    channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8;

    ide_write(ATA_PRIMARY, ATA_REG_CONTROL, 0b10);
    ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 0b10);

    for(i = 0; i < 2; i++) {
        for(j = 0; j < 2; j++) {
            uint8_t err = 0, type = IDE_ATA, status;
            ide_devices[count].Reserved = 0;

            ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
            sleep(1);

            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
            sleep(1);

            while (1) {
                status = ide_read(i, ATA_REG_STATUS);
                if((status & ATA_SR_ERR)) { err = 1; break; }
                if(!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break;
            }
            
            if(err != 0) {
                uint8_t cl = ide_read(i, ATA_REG_LBA1);
                uint8_t ch = ide_read(i, ATA_REG_LBA2);

                if(cl == 0x14 && ch == 0xEB)
                    type = IDE_ATAPI;
                else if(cl == 0x69 && ch == 0x96)
                    type = IDE_ATAPI;
                else
                    continue;

                ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                sleep(1);
            }

            ide_read_buffer(i, ATA_REG_DATA, ide_buf, 512);

            ide_devices[count].Reserved = 1;
            ide_devices[count].Type = type;
            ide_devices[count].Channel = i;
            ide_devices[count].Drive = j;
            ide_devices[count].Signature = *((uint16_t *)(ide_buf + ATA_IDENT_DEVICETYPE));
            ide_devices[count].Capabilities = *((uint16_t *)(ide_buf + ATA_IDENT_CAPABILITIES));
            ide_devices[count].CommandSets = *((uint32_t *)(ide_buf + ATA_IDENT_COMMANDSETS));

            if(ide_devices[count].CommandSets & (1 << 26))
                ide_devices[count].Size = *((uint32_t *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
            else
                ide_devices[count].Size = *((uint32_t *)(ide_buf + ATA_IDENT_MAX_LBA));

            for(k = 0; k < 40; k += 2) {
                ide_devices[count].Model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
                ide_devices[count].Model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];
            }
            ide_devices[count].Model[40] = 0;

            count++;
        }
    }

    for(i = 0; i < 4; i++) {
        char buff[256];
        if(ide_devices[i].Reserved) {
            snprintf(buff, 256, " Found %s Drive %dGB - %s\n",
                (const char* []){"ATA", "ATAPI"}[ide_devices[i].Type],
                ide_devices[i].Size / 1024 / 1024 / 2,
                ide_devices[i].Model);
        }
    }
}

uint8_t ide_ata_access(uint8_t direction, uint8_t drive, uint32_t lba,
                       uint8_t numsects, uint16_t selector, uint32_t edi) {
    uint8_t lba_mode, dma, cmd;
    uint8_t lba_io[6];
    uint32_t channel = ide_devices[drive].Channel;
    uint32_t slavebit = ide_devices[drive].Drive;
    uint32_t bus = channels[channel].base;
    uint32_t words = 256;
    uint16_t cyl, i;
    uint8_t head, sect, err;

    if(lba >= 0x10000000) {
        // LBA48
        lba_mode = 2;
        lba_io[0] = (lba & 0x000000FF) >> 0;
        lba_io[1] = (lba & 0x0000FF00) >> 8;
        lba_io[2] = (lba & 0x00FF0000) >> 16;
        lba_io[3] = (lba & 0xFF000000) >> 24;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = 0;
    } else if(ide_devices[drive].Capabilities & 0x200) {
        // LBA28
        lba_mode = 1;
        lba_io[0] = (lba & 0x00000FF) >> 0;
        lba_io[1] = (lba & 0x000FF00) >> 8;
        lba_io[2] = (lba & 0x0FF0000) >> 16;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (lba & 0xF000000) >> 24;
    } else {
        // CHS
        lba_mode = 0;
        sect = (lba & (63 - 1)) + 1;
        cyl = (lba + 1 - sect) / (16 * 63);
        lba_io[0] = sect;
        lba_io[1] = (cyl >> 0) & 0xFF;
        lba_io[2] = (cyl >> 8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (lba + 1 - sect) & ((16 * 63) - 1) / 63;
    }

    dma = 0; // DMA is not supported

    while(ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY);

    if(lba_mode == 0) {
        ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head);
    } else {
        ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head);
    }

    if(lba_mode == 2) {
        ide_write(channel, ATA_REG_SECCOUNT1, 0);
        ide_write(channel, ATA_REG_LBA3, lba_io[3]);
        ide_write(channel, ATA_REG_LBA4, lba_io[4]);
        ide_write(channel, ATA_REG_LBA5, lba_io[5]);
    }
    ide_write(channel, ATA_REG_SECCOUNT0, numsects);
    ide_write(channel, ATA_REG_LBA0, lba_io[0]);
    ide_write(channel, ATA_REG_LBA1, lba_io[1]);
    ide_write(channel, ATA_REG_LBA2, lba_io[2]);

    if (lba_mode == 0 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == 0 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
    ide_write(channel, ATA_REG_COMMAND, cmd);

    if(dma) {
        if(direction == 0) {
            // DMA Read
        } else {
            // DMA Write
        }
    } else {
        if(direction == 0) {
            for(i = 0; i < numsects; i++) {
                if((err = ide_polling(channel, 1))) {
                    return err;
                }
                __asm__ volatile("pushw %es");
                __asm__ volatile("mov %%ax, %%es"::"a"(selector));
                __asm__ volatile("rep insw"::"c"(words), "d"(bus), "D"(edi));
                __asm__ volatile("popw %es");
                edi += (words * 2);
            } else {
                for(i = 0; i < numsects; i++) {
                    ide_polling(channel, 0);
                    __asm__ volatile("push");
                    __asm__ volatile("mov %%ax, %%ds"::"a"(selector));
                    __asm__ volatile("rep outsw"::"c"(words), "d"(bus), "S"(edi));
                    __asm__ volatile("popw %ds");
                    edi += (words*2);
                }
                ide_write(channel, ATA_REG_COMMAND, (char [3]) {ATA_CMD_CACHE_FLUSH,
                    ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH_EXT});
                ide_polling(channel, 0);
            }
        }
    }

    return 0;
}

unsigned char ide_read(unsigned char channel, unsigned char reg) {
    unsigned char result = 0;
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
    if (reg < 0x08)
        result = inb(channels[channel].base + reg - 0x00);
    else if (reg < 0x0C)
        result = inb(channels[channel].base + reg - 0x06);
    else if (reg < 0x0E)
        result = inb(channels[channel].ctrl + reg - 0x0A);
    else if (reg < 0x16)
        result = inb(channels[channel].bmide + reg - 0x0E);
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
    return result;
}

void ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
    if (reg < 0x08)
        outb(channels[channel].base  + reg - 0x00, data);
    else if (reg < 0x0C)
        outb(channels[channel].base  + reg - 0x06, data);
    else if (reg < 0x0E)
        outb(channels[channel].ctrl  + reg - 0x0A, data);
    else if (reg < 0x16)
        outb(channels[channel].bmide + reg - 0x0E, data);
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

void ide_read_buffer(uint16_t channel, uint16_t reg, uint8_t *buffer, int bytes) {
    if(reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
    asm("pushw %es; movw %ds, %ax; movw %ax, %es");
    if(reg < 0x08)
        insw(channels[channel].base + reg - 0x00, buffer, bytes);
    else if(reg < 0x0C)
        insw(channels[channel].base + reg - 0x06, buffer, bytes);
    else if(reg < 0x0E)
        insw(channels[channel].ctrl + reg - 0x0A, buffer, bytes);
    else if(reg < 0x16)
        insw(channels[channel].bmide + reg - 0x0E, buffer, bytes);
    asm("popw %es;");
    if(reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

uint8_t ide_polling(uint8_t channel, int advanced_check) {
    for(int i = 0; i < 4; i++)
        ide_read(channel, ATA_REG_ALTSTATUS);
    
    while(ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY);

    if(advanced_check) {
        uint8_t state = ide_read(channel, ATA_REG_STATUS);

        if(state & ATA_SR_ERR)
            return 2;

        if(state & ATA_SR_DF)
            return 1;

        if((state & ATA_SR_DRQ) == 0)
            return 3;
    }

    return 0;
}

uint8_t ide_print_error(uint32_t drive, uint8_t err) {
    if(err == 0)
        return err;
    char buff[256];

    snprintf(buff, 32, "IDE:");
    if(err == 1) { snprintf(buff, 32, "- Device Fault\n"); err = 19; }
    else if(err == 2) {
        uint8_t st = ide_read(ide_devices[drive].Channel, ATA_REG_ERROR);
        if(st & ATA_ER_AMNF) { snprintf(buff, 32, "- No Address Mark Found\n"); err = 7; }
        if(st & ATA_ER_TK0NF) { snprintf(buff, 32, "- No Media or Media Error\n"); err = 3; }
        if(st & ATA_ER_ABRT) { snprintf(buff, 32, "- Command Aborted\n"); err = 20; }
        if(st & ATA_ER_MCR) { snprintf(buff, 32, "- No Media or Media Error\n"); err = 3; }
        if(st & ATA_ER_IDNF) { snprintf(buff, 32, "- ID Mark not Found\n"); err = 21; }
        if(st & ATA_ER_MC) { snprintf(buff, 32, "- No Media or Media Error\n"); err = 3; }
        if(st & ATA_ER_UNC) { snprintf(buff, 32, "- Uncorrectable Data Error\n"); err = 22; }
        if(st & ATA_ER_BBK) { snprintf(buff, 32, "- Bad Sectors\n"); err = 13; }
    } else if(err == 3) { snprintf(buff, 32, "- Reads Nothing\n"); err = 23; }
      else if(err == 4) { snprintf(buff, 32, "- Write Protected\n"); err = 8; }
    snprintf(buff, 256, "- [%s %s] %s\n",
        (const char* []){"Primary", "Secondary"}[ide_devices[drive].Channel],
        (const char* []){"Master", "Slave"}[ide_devices[drive].Drive],
        ide_devices[drive].Model);
    
    return err;
}
