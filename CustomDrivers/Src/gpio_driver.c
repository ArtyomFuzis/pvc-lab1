#include "gpio_driver.h"
#include "stm32f407xx.h"

#define LED_PINS (0x07 << 13)

void GpioDriver_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    (void)RCC->AHB1ENR;

    GPIOD->ODR &= ~LED_PINS;
    GPIOD->MODER &= ~(0x3F << 26);
    GPIOD->MODER |= (0x15 << 26);
    GPIOD->OTYPER &= ~LED_PINS;
    GPIOD->OSPEEDR &= ~(0x3F << 26);
    GPIOD->PUPDR &= ~(0x3F << 26);
}

void GpioDriver_SetLeds(uint8_t mask)
{
    uint32_t pins = ((uint32_t)mask & 0x07) << 13;
    GPIOD->BSRR = ((LED_PINS & ~pins) << 16) | pins;
}
