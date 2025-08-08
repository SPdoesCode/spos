/*

    A basic serial port driver.

*/

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <std/stddef.h>

#define SERIAL_COM1_PORT 0x3F8
#define SERIAL_ENABLE_DLAB 0x80

void serialportsetup(void);
uint8_t transmissionempty(void);
void serialwritechar(char c);
void serialwriteline(char *msg);

#endif
