################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CustomDrivers/Src/buttons_driver.c \
../CustomDrivers/Src/clock_driver.c \
../CustomDrivers/Src/gpio_driver.c \
../CustomDrivers/Src/keyboard_driver.c \
../CustomDrivers/Src/pca9538_driver.c \
../CustomDrivers/Src/systick_driver.c 

OBJS += \
./CustomDrivers/Src/buttons_driver.o \
./CustomDrivers/Src/clock_driver.o \
./CustomDrivers/Src/gpio_driver.o \
./CustomDrivers/Src/keyboard_driver.o \
./CustomDrivers/Src/pca9538_driver.o \
./CustomDrivers/Src/systick_driver.o 

C_DEPS += \
./CustomDrivers/Src/buttons_driver.d \
./CustomDrivers/Src/clock_driver.d \
./CustomDrivers/Src/gpio_driver.d \
./CustomDrivers/Src/keyboard_driver.d \
./CustomDrivers/Src/pca9538_driver.d \
./CustomDrivers/Src/systick_driver.d 


# Each subdirectory must supply rules for building sources it contributes
CustomDrivers/Src/%.o CustomDrivers/Src/%.su: ../CustomDrivers/Src/%.c CustomDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../CustomDrivers/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CustomDrivers-2f-Src

clean-CustomDrivers-2f-Src:
	-$(RM) ./CustomDrivers/Src/buttons_driver.d ./CustomDrivers/Src/buttons_driver.o ./CustomDrivers/Src/buttons_driver.su ./CustomDrivers/Src/clock_driver.d ./CustomDrivers/Src/clock_driver.o ./CustomDrivers/Src/clock_driver.su ./CustomDrivers/Src/gpio_driver.d ./CustomDrivers/Src/gpio_driver.o ./CustomDrivers/Src/gpio_driver.su ./CustomDrivers/Src/keyboard_driver.d ./CustomDrivers/Src/keyboard_driver.o ./CustomDrivers/Src/keyboard_driver.su ./CustomDrivers/Src/pca9538_driver.d ./CustomDrivers/Src/pca9538_driver.o ./CustomDrivers/Src/pca9538_driver.su ./CustomDrivers/Src/systick_driver.d ./CustomDrivers/Src/systick_driver.o ./CustomDrivers/Src/systick_driver.su

.PHONY: clean-CustomDrivers-2f-Src

