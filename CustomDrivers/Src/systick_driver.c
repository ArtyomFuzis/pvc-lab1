#include "systick_driver.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"

static volatile uint32_t milliseconds;

void SystickDriver_Init(void)
{
    milliseconds = 0;
    SysTick->CTRL = 0;
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
                  | SysTick_CTRL_TICKINT_Msk
                  | SysTick_CTRL_ENABLE_Msk;
}

void SystickDriver_OnInterrupt(void)
{
    milliseconds++;
}

uint32_t SystickDriver_GetMilliseconds(void)
{
    return milliseconds;
}
