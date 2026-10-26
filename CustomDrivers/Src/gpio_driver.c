#include "gpio_driver.h"
#include "stm32f407xx.h"

#define LED_PINS (0x0FU << 12U)

void GpioDriver_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    (void)RCC->AHB1ENR;

    GPIOD->ODR &= ~LED_PINS;
    GPIOD->MODER &= ~(0xFFU << 24U);
    GPIOD->MODER |= (0x55U << 24U);
    GPIOD->OTYPER &= ~LED_PINS;
    GPIOD->OSPEEDR &= ~(0xFFU << 24U);
    GPIOD->OSPEEDR |= (0xAAU << 24U);
    GPIOD->PUPDR &= ~(0xFFU << 24U);
}

void GpioDriver_SetLeds(uint8_t mask)
{
    uint32_t pins = ((uint32_t)mask & 0x0FU) << 12U;
    GPIOD->BSRR = ((LED_PINS & ~pins) << 16U) | pins;
}
