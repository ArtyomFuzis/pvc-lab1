#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h>

uint8_t KeyboardDriver_Init(void);
uint8_t KeyboardDriver_ReadRow1Columns(void);

#endif
