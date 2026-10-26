#include "systick_driver.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"

static volatile uint32_t milliseconds;

void SystickDriver_Init(void)
{
    milliseconds = 0U;
    SysTick->CTRL = 0U;
    SysTick->LOAD = (SystemCoreClock / 1000U) - 1U;
    SysTick->VAL = 0U;
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

void SystickDriver_Delay(uint32_t delay_ms)
{
    uint32_t started = SystickDriver_GetMilliseconds();

    while ((uint32_t)(SystickDriver_GetMilliseconds() - started) < delay_ms) {
        __NOP();
    }
}
