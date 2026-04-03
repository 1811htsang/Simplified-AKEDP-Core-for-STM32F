################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/AK_Imported_Driver/common/container/fifo.c \
../Drivers/AK_Imported_Driver/common/container/log_queue.c \
../Drivers/AK_Imported_Driver/common/container/ring_buffer.c 

C_DEPS += \
./Drivers/AK_Imported_Driver/common/container/fifo.d \
./Drivers/AK_Imported_Driver/common/container/log_queue.d \
./Drivers/AK_Imported_Driver/common/container/ring_buffer.d 

OBJS += \
./Drivers/AK_Imported_Driver/common/container/fifo.o \
./Drivers/AK_Imported_Driver/common/container/log_queue.o \
./Drivers/AK_Imported_Driver/common/container/ring_buffer.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AK_Imported_Driver/common/container/%.o Drivers/AK_Imported_Driver/common/container/%.su Drivers/AK_Imported_Driver/common/container/%.cyclo: ../Drivers/AK_Imported_Driver/common/container/%.c Drivers/AK_Imported_Driver/common/container/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AK_Imported_Driver-2f-common-2f-container

clean-Drivers-2f-AK_Imported_Driver-2f-common-2f-container:
	-$(RM) ./Drivers/AK_Imported_Driver/common/container/fifo.cyclo ./Drivers/AK_Imported_Driver/common/container/fifo.d ./Drivers/AK_Imported_Driver/common/container/fifo.o ./Drivers/AK_Imported_Driver/common/container/fifo.su ./Drivers/AK_Imported_Driver/common/container/log_queue.cyclo ./Drivers/AK_Imported_Driver/common/container/log_queue.d ./Drivers/AK_Imported_Driver/common/container/log_queue.o ./Drivers/AK_Imported_Driver/common/container/log_queue.su ./Drivers/AK_Imported_Driver/common/container/ring_buffer.cyclo ./Drivers/AK_Imported_Driver/common/container/ring_buffer.d ./Drivers/AK_Imported_Driver/common/container/ring_buffer.o ./Drivers/AK_Imported_Driver/common/container/ring_buffer.su

.PHONY: clean-Drivers-2f-AK_Imported_Driver-2f-common-2f-container

