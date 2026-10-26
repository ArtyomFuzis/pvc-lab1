#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

void GpioDriver_Init(void);
void GpioDriver_SetLeds(uint8_t mask);

#endif
