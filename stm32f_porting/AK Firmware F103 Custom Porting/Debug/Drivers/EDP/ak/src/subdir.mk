################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/EDP/ak/src/fsm.c \
../Drivers/EDP/ak/src/message.c \
../Drivers/EDP/ak/src/task.c \
../Drivers/EDP/ak/src/timer.c \
../Drivers/EDP/ak/src/tsm.c 

C_DEPS += \
./Drivers/EDP/ak/src/fsm.d \
./Drivers/EDP/ak/src/message.d \
./Drivers/EDP/ak/src/task.d \
./Drivers/EDP/ak/src/timer.d \
./Drivers/EDP/ak/src/tsm.d 

OBJS += \
./Drivers/EDP/ak/src/fsm.o \
./Drivers/EDP/ak/src/message.o \
./Drivers/EDP/ak/src/task.o \
./Drivers/EDP/ak/src/timer.o \
./Drivers/EDP/ak/src/tsm.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/EDP/ak/src/%.o Drivers/EDP/ak/src/%.su Drivers/EDP/ak/src/%.cyclo: ../Drivers/EDP/ak/src/%.c Drivers/EDP/ak/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/sys" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-EDP-2f-ak-2f-src

clean-Drivers-2f-EDP-2f-ak-2f-src:
	-$(RM) ./Drivers/EDP/ak/src/fsm.cyclo ./Drivers/EDP/ak/src/fsm.d ./Drivers/EDP/ak/src/fsm.o ./Drivers/EDP/ak/src/fsm.su ./Drivers/EDP/ak/src/message.cyclo ./Drivers/EDP/ak/src/message.d ./Drivers/EDP/ak/src/message.o ./Drivers/EDP/ak/src/message.su ./Drivers/EDP/ak/src/task.cyclo ./Drivers/EDP/ak/src/task.d ./Drivers/EDP/ak/src/task.o ./Drivers/EDP/ak/src/task.su ./Drivers/EDP/ak/src/timer.cyclo ./Drivers/EDP/ak/src/timer.d ./Drivers/EDP/ak/src/timer.o ./Drivers/EDP/ak/src/timer.su ./Drivers/EDP/ak/src/tsm.cyclo ./Drivers/EDP/ak/src/tsm.d ./Drivers/EDP/ak/src/tsm.o ./Drivers/EDP/ak/src/tsm.su

.PHONY: clean-Drivers-2f-EDP-2f-ak-2f-src

