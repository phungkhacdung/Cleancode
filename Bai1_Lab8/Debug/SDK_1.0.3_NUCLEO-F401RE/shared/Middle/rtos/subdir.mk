################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.c \
C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.c 

OBJS += \
./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.o \
./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.o 

C_DEPS += \
./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.d \
./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.d 


# Each subdirectory must supply rules for building sources it contributes
SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.o: C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.c SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/lib_stm" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/CMSIS/Include" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/STM32F401RE_StdPeriph_Driver/inc" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/button" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/buzzer" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/led" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/sensor" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/serial" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/ucglib" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.o: C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.c SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/lib_stm" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/CMSIS/Include" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Drivers/STM32F401RE_StdPeriph_Driver/inc" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/button" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/buzzer" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/led" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/sensor" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/serial" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Middle/ucglib" -I"C:/Users/Dung/Desktop/SDK_1.0.3_NUCLEO-F401RE/SDK_1.0.3_NUCLEO-F401RE/shared/Utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SDK_1-2e-0-2e-3_NUCLEO-2d-F401RE-2f-shared-2f-Middle-2f-rtos

clean-SDK_1-2e-0-2e-3_NUCLEO-2d-F401RE-2f-shared-2f-Middle-2f-rtos:
	-$(RM) ./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.d ./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/eventman.o ./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.d ./SDK_1.0.3_NUCLEO-F401RE/shared/Middle/rtos/timer.o

.PHONY: clean-SDK_1-2e-0-2e-3_NUCLEO-2d-F401RE-2f-shared-2f-Middle-2f-rtos

