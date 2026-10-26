#include "clock_driver.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"

#define CLOCK_STARTUP_TIMEOUT 1000000

static uint8_t WaitForRegister(volatile uint32_t *reg, uint32_t mask, uint32_t expected)
{
    uint32_t timeout = CLOCK_STARTUP_TIMEOUT;

    while (((*reg) & mask) != expected && timeout > 0) {
        timeout--;
    }

    return (uint8_t)(timeout > 0);
}

static uint8_t SwitchToHsi(void)
{
    RCC->CR |= RCC_CR_HSION;
    if (!WaitForRegister(&RCC->CR, RCC_CR_HSIRDY, RCC_CR_HSIRDY)) {
        return 0;
    }

    RCC->CFGR &= ~RCC_CFGR_SW;
    if (!WaitForRegister(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_HSI)) {
        return 0;
    }

    RCC->CR &= ~RCC_CR_PLLON;
    RCC->CR &= ~RCC_CR_HSEON;
    SystemCoreClockUpdate();
    return 1;
}

uint8_t ClockDriver_Init(void)
{
    RCC->CR |= RCC_CR_HSION;
    if (!WaitForRegister(&RCC->CR, RCC_CR_HSIRDY, RCC_CR_HSIRDY)) {
        return 0;
    }

    RCC->CFGR &= ~RCC_CFGR_SW;
    if (!WaitForRegister(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_HSI)) {
        return 0;
    }

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    (void)RCC->APB1ENR;
    PWR->CR |= PWR_CR_VOS;

    RCC->CR |= RCC_CR_HSEON;
    if (!WaitForRegister(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY)) {
        return SwitchToHsi();
    }

    RCC->PLLCFGR = 25
                 | (336 << RCC_PLLCFGR_PLLN_Pos)
                 | RCC_PLLCFGR_PLLSRC_HSE
                 | (4 << RCC_PLLCFGR_PLLQ_Pos);

    RCC->CR |= RCC_CR_PLLON;
    if (!WaitForRegister(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY)) {
        return SwitchToHsi();
    }

    FLASH->ACR = FLASH_ACR_ICEN
               | FLASH_ACR_DCEN
               | FLASH_ACR_PRFTEN
               | FLASH_ACR_LATENCY_5WS;

    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    if (!WaitForRegister(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL)) {
        return SwitchToHsi();
    }

    SystemCoreClockUpdate();
    return 1;
}
