################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/EDP/common/container/fifo.c \
../Drivers/EDP/common/container/log_queue.c \
../Drivers/EDP/common/container/ring_buffer.c 

C_DEPS += \
./Drivers/EDP/common/container/fifo.d \
./Drivers/EDP/common/container/log_queue.d \
./Drivers/EDP/common/container/ring_buffer.d 

OBJS += \
./Drivers/EDP/common/container/fifo.o \
./Drivers/EDP/common/container/log_queue.o \
./Drivers/EDP/common/container/ring_buffer.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/EDP/common/container/%.o Drivers/EDP/common/container/%.su Drivers/EDP/common/container/%.cyclo: ../Drivers/EDP/common/container/%.c Drivers/EDP/common/container/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/sys" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-EDP-2f-common-2f-container

clean-Drivers-2f-EDP-2f-common-2f-container:
	-$(RM) ./Drivers/EDP/common/container/fifo.cyclo ./Drivers/EDP/common/container/fifo.d ./Drivers/EDP/common/container/fifo.o ./Drivers/EDP/common/container/fifo.su ./Drivers/EDP/common/container/log_queue.cyclo ./Drivers/EDP/common/container/log_queue.d ./Drivers/EDP/common/container/log_queue.o ./Drivers/EDP/common/container/log_queue.su ./Drivers/EDP/common/container/ring_buffer.cyclo ./Drivers/EDP/common/container/ring_buffer.d ./Drivers/EDP/common/container/ring_buffer.o ./Drivers/EDP/common/container/ring_buffer.su

.PHONY: clean-Drivers-2f-EDP-2f-common-2f-container

