#include <drivers/ide/ide.h>
#include <std/stddef.h>
#include <sys/portio.h>
#include <sys/osutil.h>

static volatile uint8_t ide_irq_invoked = 0;
static uint8_t atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

ide_register_t ide_reg_buff[2];
ide_device_t ide_dev_buff[4];
uint8_t ide_buff[2048];

uint8_t ide_read(uint8_t channel, uint8_t reg) {
    uint8_t result;
    if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | ide_reg_buff[channel].nien);
   if (reg < 0x08)
      result = inb(ide_reg_buff[channel].base + reg - 0x00);
   else if (reg < 0x0C)
      result = inb(ide_reg_buff[channel].base  + reg - 0x06);
   else if (reg < 0x0E)
      result = inb(ide_reg_buff[channel].ctrl  + reg - 0x0A);
   else if (reg < 0x16)
      result = inb(ide_reg_buff[channel].bmide + reg - 0x0E);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, ide_reg_buff[channel].nien);
   return result;
}

void ide_write(uint8_t channel, uint8_t reg, uint8_t data) {
    if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | ide_reg_buff[channel].nien);
   if (reg < 0x08)
      outb(ide_reg_buff[channel].base  + reg - 0x00, data);
   else if (reg < 0x0C)
      outb(ide_reg_buff[channel].base  + reg - 0x06, data);
   else if (reg < 0x0E)
      outb(ide_reg_buff[channel].ctrl  + reg - 0x0A, data);
   else if (reg < 0x16)
      outb(ide_reg_buff[channel].bmide + reg - 0x0E, data);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, ide_reg_buff[channel].nien);
}

void ide_read_buff(uint8_t channel, uint8_t reg, void* buff, uint32_t quads) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | ide_reg_buff[channel].nien);

    if (reg < 0x08)
        insl(ide_reg_buff[channel].base  + reg - 0x00, buff, quads);
    else if (reg < 0x0C)
        insl(ide_reg_buff[channel].base  + reg - 0x06, buff, quads);
    else if (reg < 0x0E)
        insl(ide_reg_buff[channel].ctrl  + reg - 0x0A, buff, quads);
    else if (reg < 0x16)
        insl(ide_reg_buff[channel].bmide + reg - 0x0E, buff, quads);

    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, ide_reg_buff[channel].nien);
}


uint8_t ide_polling(uint8_t channel, uint8_t advanced_check) {
   // (I) Delay 400 nanosecond for BSY to be set:
   // -------------------------------------------------
   for(int i = 0; i < 4; i++)
      ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times.

   // (II) Wait for BSY to be cleared:
   // -------------------------------------------------
   while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
      ; // Wait for BSY to be zero.

   if (advanced_check) {
      uint8_t state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.

      // (III) Check For Errors:
      // -------------------------------------------------
      if (state & ATA_SR_ERR)
         return 2; // Error.

      // (IV) Check If Device fault:
      // -------------------------------------------------
      if (state & ATA_SR_DF)
         return 1; // Device Fault.

      // (V) Check DRQ:
      // -------------------------------------------------
      // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
      if ((state & ATA_SR_DRQ) == 0)
         return 3; // DRQ should be set

   }

   return 0; // No Error.
}

static char* geterrormsg(uint8_t err, uint8_t drive) {
    if (err == 1) return "Device fault.";
    else if (err == 1) {
        uint8_t st = ide_read(ide_dev_buff[drive].channel, ATA_REG_ERROR);
        if (st & ATA_ER_AMNF) return "No adress mark found";
        if (st & ATA_ER_TK0NF)   return "No media or media err";
        if (st & ATA_ER_ABRT) return "Aborted";
        if (st & ATA_ER_MCR) return "No media or media error";
        if (st & ATA_ER_IDNF) return "ID mark not found!";
        if (st & ATA_ER_MC) return "Media err or no media";
        if (st & ATA_ER_UNC) return "Uncorrectable data";
        if (st & ATA_ER_BBK) return "Bad sectors!";
    } else if (err == 3) return "Reads nothing!!";
    else if (err == 4) return "Write protected!!";
}

static uint8_t geterrnum(uint8_t err, uint8_t drive) {
    if (err == 1) { err = 19;}
    else if (err == 2) {
      unsigned char st = ide_read(ide_dev_buff[drive].channel, ATA_REG_ERROR);
      if (st & ATA_ER_AMNF)   err = 7;
      if (st & ATA_ER_TK0NF)     err = 3;
      if (st & ATA_ER_ABRT)   err = 20; 
      if (st & ATA_ER_MCR)      err = 3;
      if (st & ATA_ER_IDNF)       err = 21;
      if (st & ATA_ER_MC)    err = 3;
      if (st & ATA_ER_UNC)    err = 22;
      if (st & ATA_ER_BBK)   err = 13;
   } else  if (err == 3)     err = 23;
    else  if (err == 4)  err = 8;
}

uint8_t ide_err_print(uint8_t drive, uint8_t err) {
    if (err == 0) return err;

#   ifdef VGA_TEXTMODE_H
        txtprintln(0x0F, "IDE error with drive %d errored with error %d: %s %d (%s %s: %s)", drive, err, geterrormsg(err, drive), geterrnum(err, drive) \
        (const char *[]){"Primary", "Secondary"}[ide_dev_buff[drive].channel], (const char *[]){"Master", "Slave"}[ide_dev_buff[drive].drive], \
        ide_dev_buff[drive].model);
#   endif
#   ifdef SERIALPORT_H 
        serialprintln("IDE error with drive %d errored with error %d: %s %d (%s %s: %s)", drive, err, geterrormsg(err, drive), geterrnum(err, drive) \
        (const char *[]){"Primary", "Secondary"}[ide_dev_buff[drive].channel], (const char *[]){"Master", "Slave"}[ide_dev_buff[drive].drive], \
        ide_dev_buff[drive].model);
#   endif
    return geterrnum(err, drive);
}

void ide_initialize(uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3, uint32_t BAR4) {
    int j, k, count = 0;

    // 1- Detect I/O Ports which interface IDE Controller:
    ide_reg_buff[ATA_PRIMARY  ].base  = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
    ide_reg_buff[ATA_PRIMARY  ].ctrl  = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
    ide_reg_buff[ATA_SECONDARY].base  = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
    ide_reg_buff[ATA_SECONDARY].ctrl  = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
    ide_reg_buff[ATA_PRIMARY  ].bmide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
    ide_reg_buff[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE

    ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
    ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    // 3- Detect ATA-ATAPI Devices:
    int i = 0;
    for (i = 0; i < 2; i++)
      for (j = 0; j < 2; j++) {

         unsigned char err = 0, type = IDE_ATA, status;
         ide_dev_buff[count].reserve = 0; // Assuming that no drive here.

         // (I) Select Drive:
        ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
        sleep(1); // Wait 1ms for drive select to work.

         // (II) Send ATA Identify Command:
        ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
        sleep(1); // This function should be implemented in your OS. which waits for 1 ms.
                   // it is based on System Timer Device Driver.

        if (ide_read(i, ATA_REG_STATUS) == 0) continue; // If Status = 0, No Device.

        while(1) {
            status = ide_read(i, ATA_REG_STATUS);
            if ((status & ATA_SR_ERR)) {err = 1; break;} // If Err, Device is not ATA.
            if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
        }

        if (err != 0) {
            unsigned char cl = ide_read(i, ATA_REG_LBA1);
            unsigned char ch = ide_read(i, ATA_REG_LBA2);

            if (cl == 0x14 && ch == 0xEB)
               type = IDE_ATAPI;
            else if (cl == 0x69 && ch == 0x96)
               type = IDE_ATAPI;
            else
               continue; 

            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
            sleep(1);
        }

        ide_read_buff(i, ATA_REG_DATA, ide_buff, 128);

        ide_dev_buff[count].reserve     = 1;
        ide_dev_buff[count].type         = type;
        ide_dev_buff[count].channel      = i;
        ide_dev_buff[count].drive        = j;
        ide_dev_buff[count].signature    = *((unsigned short *)(ide_buff + ATA_IDENT_DEVICETYPE));
        ide_dev_buff[count].capabilitys = *((unsigned short *)(ide_buff + ATA_IDENT_CAPABILITIES));
        ide_dev_buff[count].commandsets  = *((unsigned int *)(ide_buff + ATA_IDENT_COMMANDSETS));

        if (ide_dev_buff[count].commandsets & (1 << 26))
            ide_dev_buff[count].size   = *((unsigned int *)(ide_buff + ATA_IDENT_MAX_LBA_EXT));
        else
            ide_dev_buff[count].size   = *((unsigned int *)(ide_buff + ATA_IDENT_MAX_LBA));

        for(k = 0; k < 40; k += 2) {
            ide_dev_buff[count].model[k] = ide_buff[ATA_IDENT_MODEL + k + 1];
            ide_dev_buff[count].model[k + 1] = ide_buff[ATA_IDENT_MODEL + k];}
        ide_dev_buff[count].model[40] = 0; 

        count++;
    }

}