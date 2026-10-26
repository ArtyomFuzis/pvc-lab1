#include "buttons_driver.h"
#include "stm32f407xx.h"

#define BTN_PC15_PIN    (1U << 15)
#define DEBOUNCE_MS     20U

static uint8_t  s_last_raw    = 1;
static uint8_t  s_stable      = 1;
static uint32_t s_last_change = 0;

void ButtonPc15_Init(void)
{

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;

    GPIOC->MODER &= ~(0x3U << (15 * 2));
    GPIOC->PUPDR &= ~(0x3U << (15 * 2));
    GPIOC->PUPDR |=  (0x1U << (15 * 2));
    GPIOC->OTYPER  &= ~BTN_PC15_PIN;
    GPIOC->OSPEEDR &= ~(0x3U << (15 * 2));
}

uint8_t ButtonPc15_WasPressed(uint32_t now_ms)
{
    uint8_t raw = (GPIOC->IDR & BTN_PC15_PIN) ? 1U : 0U;

    if (raw != s_last_raw) {
        s_last_raw    = raw;
        s_last_change = now_ms;
    }

    if ((uint32_t)(now_ms - s_last_change) >= DEBOUNCE_MS && raw != s_stable) {
        s_stable = raw;
        if (s_stable == 0U) {
            return 1U;
        }
    }
    return 0U;
}
