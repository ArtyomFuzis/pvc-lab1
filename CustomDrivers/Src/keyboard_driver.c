#include "keyboard_driver.h"
#include "pca9538_driver.h"

#define PCA9538_REG_INPUT 0x00
#define PCA9538_REG_OUTPUT 0x01
#define PCA9538_REG_CONFIG 0x03
#define KEYBOARD_ROW1_CONFIG 0xFE

uint8_t KeyboardDriver_Init(void)
{
    return Pca9538Driver_Init();
}

uint8_t KeyboardDriver_ReadRow1Columns(void)
{
    uint8_t input;

    if (!Pca9538Driver_WriteRegister(PCA9538_REG_OUTPUT, 0x00)) {
        return 0;
    }
    if (!Pca9538Driver_WriteRegister(PCA9538_REG_CONFIG, KEYBOARD_ROW1_CONFIG)) {
        return 0;
    }
    if (!Pca9538Driver_ReadRegister(PCA9538_REG_INPUT, &input)) {
        return 0;
    }

    return (uint8_t)((~input >> 4) & 0x07);
}
