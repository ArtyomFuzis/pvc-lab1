#ifndef SYSTICK_DRIVER_H
#define SYSTICK_DRIVER_H

#include <stdint.h>

void SystickDriver_Init(void);
void SystickDriver_OnInterrupt(void);
uint32_t SystickDriver_GetMilliseconds(void);

#endif
