#include "main.h"
#include "clock_driver.h"
#include "gpio_driver.h"
#include "systick_driver.h"
#include "buttons_driver.h"
#include "keyboard_driver.h"

#define LED_MASK_ALL 0x0FU
#define MODE_COUNT 5U

static const uint16_t mode_interval_ms[MODE_COUNT] = {
    130U,
    180U,
    300U,
    180U,
    500U
};

static uint8_t PatternFrame(uint8_t mode, uint32_t step)
{
    uint8_t position;
    uint8_t count;
    uint8_t phase;

    switch (mode) {
        case 0U:
            return (uint8_t)(1U << (step % 4U));

        case 1U:
            position = (uint8_t)(step % 4U);
            return (uint8_t)((1U << position) | (1U << ((position + 1U) % 4U)));

        case 2U:
            return ((step % 2U) == 0U) ? 0x05U : 0x0AU;

        case 3U:
            phase = (uint8_t)(step % 8U);
            count = (phase <= 3U) ? (uint8_t)(phase + 1U) : (uint8_t)(7U - phase);
            return (count == 0U) ? 0U : (uint8_t)((1U << count) - 1U);

        case 4U:
            return ((step % 2U) == 0U) ? LED_MASK_ALL : 0U;

        default:
            return 0U;
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {
        GPIOD->ODR ^= (1U << 12U);
        for (volatile uint32_t delay = 0U; delay < 150000U; delay++) {
            __NOP();
        }
    }
}

int main(void)
{
    uint8_t mode = 0U;
    uint32_t step = 0U;
    uint32_t last_step_at;

    (void)ClockDriver_Init();
    SystickDriver_Init();
    GpioDriver_Init();
    if (KeyboardDriver_Init() == 0U) {
        Error_Handler();
    }
    ButtonsDriver_Init();

    last_step_at = SystickDriver_GetMilliseconds();
    GpioDriver_SetLeds(PatternFrame(mode, step));

    while (1) {
        uint32_t now = SystickDriver_GetMilliseconds();
        uint8_t next_pressed;
        uint8_t previous_pressed;

        ButtonsDriver_Update(now);
        next_pressed = ButtonsDriver_WasPressed(BUTTON_NEXT);
        previous_pressed = ButtonsDriver_WasPressed(BUTTON_PREVIOUS);

        if (next_pressed != previous_pressed) {
            if (next_pressed != 0U) {
                mode = (uint8_t)((mode + 1U) % MODE_COUNT);
            } else {
                mode = (uint8_t)((mode + MODE_COUNT - 1U) % MODE_COUNT);
            }

            step = 0U;
            last_step_at = now;
            GpioDriver_SetLeds(PatternFrame(mode, step));
            continue;
        }

        if ((uint32_t)(now - last_step_at) >= mode_interval_ms[mode]) {
            last_step_at = now;
            step++;
            GpioDriver_SetLeds(PatternFrame(mode, step));
        }
    }
}
