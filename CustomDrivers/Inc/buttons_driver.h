#ifndef BUTTONS_DRIVER_H
#define BUTTONS_DRIVER_H

#include <stdint.h>

typedef enum {
    BUTTON_NEXT = 0,
    BUTTON_COUNT
} ButtonId_t;

void    ButtonPc15_Init(void);
uint8_t ButtonPc15_WasPressed(uint32_t now_ms);

#endif
