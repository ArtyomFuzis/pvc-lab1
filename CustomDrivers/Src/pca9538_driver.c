#include "pca9538_driver.h"
#include "stm32f407xx.h"

#define PCA9538_ADDRESS_WRITE 0xE2U
#define PCA9538_ADDRESS_READ  0xE3U
#define PCA9538_REG_INPUT     0x00U
#define PCA9538_REG_OUTPUT    0x01U
#define PCA9538_REG_POLARITY  0x02U
#define PCA9538_REG_CONFIG    0x03U

#define I2C_SCL_PIN (1U << 8U)
#define I2C_SDA_PIN (1U << 9U)
#define I2C_PIN_MASK (I2C_SCL_PIN | I2C_SDA_PIN)

static void Delay(void)
{
    for (volatile uint32_t i = 0U; i < 240U; i++) {
        __NOP();
    }
}

static void SclRelease(void)
{
    GPIOB->BSRR = I2C_SCL_PIN;
}

static void SclLow(void)
{
    GPIOB->BSRR = I2C_SCL_PIN << 16U;
}

static void SdaRelease(void)
{
    GPIOB->BSRR = I2C_SDA_PIN;
}

static void SdaLow(void)
{
    GPIOB->BSRR = I2C_SDA_PIN << 16U;
}

static uint8_t SdaRead(void)
{
    return (uint8_t)((GPIOB->IDR & I2C_SDA_PIN) != 0U);
}

static void StartCondition(void)
{
    SdaRelease();
    SclRelease();
    Delay();
    SdaLow();
    Delay();
    SclLow();
    Delay();
}

static void StopCondition(void)
{
    SdaLow();
    Delay();
    SclRelease();
    Delay();
    SdaRelease();
    Delay();
}

static uint8_t WriteByte(uint8_t value)
{
    uint8_t bit;
    uint8_t acknowledged;

    for (bit = 0U; bit < 8U; bit++) {
        if ((value & 0x80U) != 0U) {
            SdaRelease();
        } else {
            SdaLow();
        }

        Delay();
        SclRelease();
        Delay();
        SclLow();
        Delay();
        value <<= 1U;
    }

    SdaRelease();
    Delay();
    SclRelease();
    Delay();
    acknowledged = (uint8_t)(SdaRead() == 0U);
    SclLow();
    Delay();

    return acknowledged;
}

static uint8_t ReadByte(uint8_t acknowledge)
{
    uint8_t bit;
    uint8_t value = 0U;

    SdaRelease();
    for (bit = 0U; bit < 8U; bit++) {
        value <<= 1U;
        SclRelease();
        Delay();
        if (SdaRead() != 0U) {
            value |= 1U;
        }
        SclLow();
        Delay();
    }

    if (acknowledge != 0U) {
        SdaLow();
    } else {
        SdaRelease();
    }
    Delay();
    SclRelease();
    Delay();
    SclLow();
    Delay();
    SdaRelease();

    return value;
}

static void GpioBusInit(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    (void)RCC->AHB1ENR;

    GPIOB->MODER &= ~((3U << (8U * 2U)) | (3U << (9U * 2U)));
    GPIOB->MODER |= (1U << (8U * 2U)) | (1U << (9U * 2U));
    GPIOB->OTYPER |= I2C_PIN_MASK;
    GPIOB->OSPEEDR &= ~((3U << (8U * 2U)) | (3U << (9U * 2U)));
    GPIOB->OSPEEDR |= (1U << (8U * 2U)) | (1U << (9U * 2U));
    GPIOB->PUPDR &= ~((3U << (8U * 2U)) | (3U << (9U * 2U)));
    GPIOB->PUPDR |= (1U << (8U * 2U)) | (1U << (9U * 2U));

    SclRelease();
    SdaRelease();
    Delay();
}

uint8_t Pca9538Driver_WriteRegister(uint8_t reg, uint8_t value)
{
    uint8_t acknowledged;

    StartCondition();
    acknowledged = WriteByte(PCA9538_ADDRESS_WRITE);
    if (acknowledged != 0U) {
        acknowledged = WriteByte(reg);
    }
    if (acknowledged != 0U) {
        acknowledged = WriteByte(value);
    }
    StopCondition();

    return acknowledged;
}

uint8_t Pca9538Driver_ReadRegister(uint8_t reg, uint8_t *value)
{
    uint8_t acknowledged;

    if (value == 0) {
        return 0U;
    }

    StartCondition();
    acknowledged = WriteByte(PCA9538_ADDRESS_WRITE);
    if (acknowledged != 0U) {
        acknowledged = WriteByte(reg);
    }
    if (acknowledged == 0U) {
        StopCondition();
        return 0U;
    }

    StartCondition();
    acknowledged = WriteByte(PCA9538_ADDRESS_READ);
    if (acknowledged != 0U) {
        *value = ReadByte(0U);
    }
    StopCondition();

    return acknowledged;
}

uint8_t Pca9538Driver_Init(void)
{
    uint8_t input;

    GpioBusInit();

    if (Pca9538Driver_WriteRegister(PCA9538_REG_POLARITY, 0x00U) == 0U) {
        return 0U;
    }
    if (Pca9538Driver_WriteRegister(PCA9538_REG_OUTPUT, 0x00U) == 0U) {
        return 0U;
    }
    if (Pca9538Driver_WriteRegister(PCA9538_REG_CONFIG, 0xFEU) == 0U) {
        return 0U;
    }

    return Pca9538Driver_ReadRegister(PCA9538_REG_INPUT, &input);
}
