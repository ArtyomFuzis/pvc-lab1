#include "keyboard_driver.h"
#include "pca9538_driver.h"

#define PCA9538_REG_INPUT 0x00U
#define PCA9538_REG_OUTPUT 0x01U
#define PCA9538_REG_CONFIG 0x03U
#define KEYBOARD_ROW1_CONFIG 0xFEU

uint8_t KeyboardDriver_Init(void)
{
    return Pca9538Driver_Init();
}

uint8_t KeyboardDriver_ReadRow1Columns(void)
{
    uint8_t input;

    if (Pca9538Driver_WriteRegister(PCA9538_REG_OUTPUT, 0x00U) == 0U) {
        return 0U;
    }
    if (Pca9538Driver_WriteRegister(PCA9538_REG_CONFIG, KEYBOARD_ROW1_CONFIG) == 0U) {
        return 0U;
    }
    if (Pca9538Driver_ReadRegister(PCA9538_REG_INPUT, &input) == 0U) {
        return 0U;
    }

    return (uint8_t)((~input >> 4U) & 0x07U);
}
