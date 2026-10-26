#include "main.h"
#include "clock_driver.h"
#include "gpio_driver.h"
#include "systick_driver.h"
#include "buttons_driver.h"
#include "keyboard_driver.h"

#define MODE_COUNT 5
#define LED_MASK_ALL 0x07

static const uint16_t mode_interval_ms[MODE_COUNT] = {
    130,
    180,
    300,
    180,
    500
};

static uint8_t PatternFrame(uint8_t mode, uint32_t step)
{
    static const uint8_t two_led_frames[] = {3, 6, 5};
    static const uint8_t fill_frames[] = {1, 3, 7, 3, 1, 0};

    switch (mode) {
        case 0:
            return (uint8_t)(1 << (step % 3));
        case 1:
            return two_led_frames[step % 3];
        case 2:
            return (step % 2 == 0) ? 5 : 2;
        case 3:
            return fill_frames[step % 6];
        case 4:
            return (step % 2 == 0) ? LED_MASK_ALL : 0;
        default:
            return 0;
    }
}

void Error_Handler(void)
{
    __disable_irq();
    GPIOD->BSRR = 1 << 15;
    while (1) {
    }
}

int main(void)
{
    uint8_t mode = 0;
    uint32_t step = 0;
    uint32_t last_frame;

    GpioDriver_Init();
    if (!ClockDriver_Init()) {
        Error_Handler();
    }

    SystickDriver_Init();
    if (!KeyboardDriver_Init()) {
        Error_Handler();
    }
    ButtonsDriver_Init();

    last_frame = SystickDriver_GetMilliseconds();
    GpioDriver_SetLeds(PatternFrame(mode, step));

    while (1) {
        uint32_t now = SystickDriver_GetMilliseconds();
        uint8_t next_pressed;
        uint8_t previous_pressed;

        ButtonsDriver_Update(now);
        next_pressed = ButtonsDriver_WasPressed(BUTTON_NEXT);
        previous_pressed = ButtonsDriver_WasPressed(BUTTON_PREVIOUS);

        if (next_pressed != previous_pressed) {
            if (next_pressed) {
                mode = (uint8_t)((mode + 1) % MODE_COUNT);
            } else {
                mode = (uint8_t)((mode + MODE_COUNT - 1) % MODE_COUNT);
            }

            step = 0;
            last_frame = now;
            GpioDriver_SetLeds(PatternFrame(mode, step));
            continue;
        }

        if ((uint32_t)(now - last_frame) >= mode_interval_ms[mode]) {
            last_frame = now;
            step++;
            GpioDriver_SetLeds(PatternFrame(mode, step));
        }
    }
}
