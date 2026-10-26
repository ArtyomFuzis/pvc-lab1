#ifndef PCA9538_DRIVER_H
#define PCA9538_DRIVER_H

#include <stdint.h>

uint8_t Pca9538Driver_Init(void);
uint8_t Pca9538Driver_WriteRegister(uint8_t reg, uint8_t value);
uint8_t Pca9538Driver_ReadRegister(uint8_t reg, uint8_t *value);

#endif
