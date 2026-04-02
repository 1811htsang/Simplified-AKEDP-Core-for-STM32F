################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/AK_Imported_Driver/ak/src/fsm.c \
../Drivers/AK_Imported_Driver/ak/src/message.c \
../Drivers/AK_Imported_Driver/ak/src/task.c \
../Drivers/AK_Imported_Driver/ak/src/timer.c \
../Drivers/AK_Imported_Driver/ak/src/tsm.c 

OBJS += \
./Drivers/AK_Imported_Driver/ak/src/fsm.o \
./Drivers/AK_Imported_Driver/ak/src/message.o \
./Drivers/AK_Imported_Driver/ak/src/task.o \
./Drivers/AK_Imported_Driver/ak/src/timer.o \
./Drivers/AK_Imported_Driver/ak/src/tsm.o 

C_DEPS += \
./Drivers/AK_Imported_Driver/ak/src/fsm.d \
./Drivers/AK_Imported_Driver/ak/src/message.d \
./Drivers/AK_Imported_Driver/ak/src/task.d \
./Drivers/AK_Imported_Driver/ak/src/timer.d \
./Drivers/AK_Imported_Driver/ak/src/tsm.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AK_Imported_Driver/ak/src/%.o Drivers/AK_Imported_Driver/ak/src/%.su Drivers/AK_Imported_Driver/ak/src/%.cyclo: ../Drivers/AK_Imported_Driver/ak/src/%.c Drivers/AK_Imported_Driver/ak/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AK_Imported_Driver-2f-ak-2f-src

clean-Drivers-2f-AK_Imported_Driver-2f-ak-2f-src:
	-$(RM) ./Drivers/AK_Imported_Driver/ak/src/fsm.cyclo ./Drivers/AK_Imported_Driver/ak/src/fsm.d ./Drivers/AK_Imported_Driver/ak/src/fsm.o ./Drivers/AK_Imported_Driver/ak/src/fsm.su ./Drivers/AK_Imported_Driver/ak/src/message.cyclo ./Drivers/AK_Imported_Driver/ak/src/message.d ./Drivers/AK_Imported_Driver/ak/src/message.o ./Drivers/AK_Imported_Driver/ak/src/message.su ./Drivers/AK_Imported_Driver/ak/src/task.cyclo ./Drivers/AK_Imported_Driver/ak/src/task.d ./Drivers/AK_Imported_Driver/ak/src/task.o ./Drivers/AK_Imported_Driver/ak/src/task.su ./Drivers/AK_Imported_Driver/ak/src/timer.cyclo ./Drivers/AK_Imported_Driver/ak/src/timer.d ./Drivers/AK_Imported_Driver/ak/src/timer.o ./Drivers/AK_Imported_Driver/ak/src/timer.su ./Drivers/AK_Imported_Driver/ak/src/tsm.cyclo ./Drivers/AK_Imported_Driver/ak/src/tsm.d ./Drivers/AK_Imported_Driver/ak/src/tsm.o ./Drivers/AK_Imported_Driver/ak/src/tsm.su

.PHONY: clean-Drivers-2f-AK_Imported_Driver-2f-ak-2f-src

