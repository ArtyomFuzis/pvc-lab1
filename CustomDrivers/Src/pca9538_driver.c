#include "pca9538_driver.h"
#include "stm32f407xx.h"

#define PCA9538_ADDRESS_WRITE 0xE2
#define PCA9538_ADDRESS_READ  0xE3
#define PCA9538_REG_INPUT     0x00
#define PCA9538_REG_OUTPUT    0x01
#define PCA9538_REG_POLARITY  0x02
#define PCA9538_REG_CONFIG    0x03

#define I2C_SCL_PIN (1 << 8)
#define I2C_SDA_PIN (1 << 9)
#define I2C_PIN_MASK (I2C_SCL_PIN | I2C_SDA_PIN)

static void BusDelay(void)
{
    for (volatile uint32_t i = 0; i < 240; i++) {
        __NOP();
    }
}

static void SclRelease(void)
{
    GPIOB->BSRR = I2C_SCL_PIN;
}

static void SclLow(void)
{
    GPIOB->BSRR = I2C_SCL_PIN << 16;
}

static void SdaRelease(void)
{
    GPIOB->BSRR = I2C_SDA_PIN;
}

static void SdaLow(void)
{
    GPIOB->BSRR = I2C_SDA_PIN << 16;
}

static uint8_t SdaRead(void)
{
    return (uint8_t)((GPIOB->IDR & I2C_SDA_PIN) != 0);
}

static void StartCondition(void)
{
    SdaRelease();
    SclRelease();
    BusDelay();
    SdaLow();
    BusDelay();
    SclLow();
    BusDelay();
}

static void StopCondition(void)
{
    SdaLow();
    BusDelay();
    SclRelease();
    BusDelay();
    SdaRelease();
    BusDelay();
}

static uint8_t WriteByte(uint8_t value)
{
    uint8_t bit;
    uint8_t acknowledged;

    for (bit = 0; bit < 8; bit++) {
        if ((value & 0x80) != 0) {
            SdaRelease();
        } else {
            SdaLow();
        }

        BusDelay();
        SclRelease();
        BusDelay();
        SclLow();
        BusDelay();
        value <<= 1;
    }

    SdaRelease();
    BusDelay();
    SclRelease();
    BusDelay();
    acknowledged = (uint8_t)(SdaRead() == 0);
    SclLow();
    BusDelay();

    return acknowledged;
}

static uint8_t ReadByte(uint8_t acknowledge)
{
    uint8_t bit;
    uint8_t value = 0;

    SdaRelease();
    for (bit = 0; bit < 8; bit++) {
        value <<= 1;
        SclRelease();
        BusDelay();
        if (SdaRead() != 0) {
            value |= 1;
        }
        SclLow();
        BusDelay();
    }

    if (acknowledge != 0) {
        SdaLow();
    } else {
        SdaRelease();
    }
    BusDelay();
    SclRelease();
    BusDelay();
    SclLow();
    BusDelay();
    SdaRelease();

    return value;
}

static void GpioBusInit(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    (void)RCC->AHB1ENR;

    GPIOB->MODER &= ~((3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOB->MODER |= (1 << (8 * 2)) | (1 << (9 * 2));
    GPIOB->OTYPER |= I2C_PIN_MASK;
    GPIOB->OSPEEDR &= ~((3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOB->OSPEEDR |= (1 << (8 * 2)) | (1 << (9 * 2));
    GPIOB->PUPDR &= ~((3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOB->PUPDR |= (1 << (8 * 2)) | (1 << (9 * 2));

    SclRelease();
    SdaRelease();
    BusDelay();
}

uint8_t Pca9538Driver_WriteRegister(uint8_t reg, uint8_t value)
{
    uint8_t acknowledged;

    StartCondition();
    acknowledged = WriteByte(PCA9538_ADDRESS_WRITE);
    if (acknowledged != 0) {
        acknowledged = WriteByte(reg);
    }
    if (acknowledged != 0) {
        acknowledged = WriteByte(value);
    }
    StopCondition();

    return acknowledged;
}

uint8_t Pca9538Driver_ReadRegister(uint8_t reg, uint8_t *value)
{
    uint8_t acknowledged;

    if (value == 0) {
        return 0;
    }

    StartCondition();
    acknowledged = WriteByte(PCA9538_ADDRESS_WRITE);
    if (acknowledged != 0) {
        acknowledged = WriteByte(reg);
    }
    if (acknowledged == 0) {
        StopCondition();
        return 0;
    }

    StartCondition();
    acknowledged = WriteByte(PCA9538_ADDRESS_READ);
    if (acknowledged != 0) {
        *value = ReadByte(0);
    }
    StopCondition();

    return acknowledged;
}

uint8_t Pca9538Driver_Init(void)
{
    uint8_t input;

    GpioBusInit();

    if (Pca9538Driver_WriteRegister(PCA9538_REG_POLARITY, 0x00) == 0) {
        return 0;
    }
    if (Pca9538Driver_WriteRegister(PCA9538_REG_OUTPUT, 0x00) == 0) {
        return 0;
    }
    if (Pca9538Driver_WriteRegister(PCA9538_REG_CONFIG, 0xFE) == 0) {
        return 0;
    }

    return Pca9538Driver_ReadRegister(PCA9538_REG_INPUT, &input);
}
