#ifndef BUTTONS_DRIVER_H
#define BUTTONS_DRIVER_H

#include <stdint.h>

typedef enum {
    BUTTON_PREVIOUS = 0,
    BUTTON_NEXT = 1
} Button_t;

void ButtonsDriver_Init(void);
void ButtonsDriver_Update(uint32_t now_ms);
uint8_t ButtonsDriver_WasPressed(Button_t button);

#endif
