#include "buttons_driver.h"
#include "keyboard_driver.h"

#define BUTTON_DEBOUNCE_MS 25U
#define BUTTON_COUNT 2U
#define COLUMN_LEFT 0x01U
#define COLUMN_RIGHT 0x04U

typedef struct {
    uint8_t raw_state;
    uint8_t stable_state;
    uint8_t pressed_event;
    uint32_t raw_changed_at;
} ButtonState_t;

static ButtonState_t buttons[BUTTON_COUNT];

static uint8_t ReadButtons(void)
{
    uint8_t columns = KeyboardDriver_ReadRow1Columns();
    uint8_t pressed = 0U;

    if ((columns & COLUMN_LEFT) != 0U) {
        pressed |= (1U << BUTTON_PREVIOUS);
    }
    if ((columns & COLUMN_RIGHT) != 0U) {
        pressed |= (1U << BUTTON_NEXT);
    }

    return pressed;
}

void ButtonsDriver_Init(void)
{
    uint8_t pressed = ReadButtons();
    uint8_t i;

    for (i = 0U; i < BUTTON_COUNT; i++) {
        uint8_t state = (uint8_t)((pressed >> i) & 1U);
        buttons[i].raw_state = state;
        buttons[i].stable_state = state;
        buttons[i].pressed_event = 0U;
        buttons[i].raw_changed_at = 0U;
    }
}

void ButtonsDriver_Update(uint32_t now_ms)
{
    uint8_t pressed = ReadButtons();
    uint8_t i;

    for (i = 0U; i < BUTTON_COUNT; i++) {
        uint8_t raw = (uint8_t)((pressed >> i) & 1U);

        if (raw != buttons[i].raw_state) {
            buttons[i].raw_state = raw;
            buttons[i].raw_changed_at = now_ms;
        }

        if (raw != buttons[i].stable_state
            && (uint32_t)(now_ms - buttons[i].raw_changed_at) >= BUTTON_DEBOUNCE_MS) {
            buttons[i].stable_state = raw;
            if (raw != 0U) {
                buttons[i].pressed_event = 1U;
            }
        }
    }
}

uint8_t ButtonsDriver_WasPressed(Button_t button)
{
    uint8_t event;

    if ((uint32_t)button >= BUTTON_COUNT) {
        return 0U;
    }

    event = buttons[button].pressed_event;
    buttons[button].pressed_event = 0U;
    return event;
}
