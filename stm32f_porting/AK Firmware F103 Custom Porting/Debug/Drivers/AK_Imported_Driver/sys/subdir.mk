################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/AK_Imported_Driver/sys/porting_compat.cpp 

C_SRCS += \
../Drivers/AK_Imported_Driver/sys/sys_boot.c \
../Drivers/AK_Imported_Driver/sys/sys_dbg.c 

C_DEPS += \
./Drivers/AK_Imported_Driver/sys/sys_boot.d \
./Drivers/AK_Imported_Driver/sys/sys_dbg.d 

OBJS += \
./Drivers/AK_Imported_Driver/sys/porting_compat.o \
./Drivers/AK_Imported_Driver/sys/sys_boot.o \
./Drivers/AK_Imported_Driver/sys/sys_dbg.o 

CPP_DEPS += \
./Drivers/AK_Imported_Driver/sys/porting_compat.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AK_Imported_Driver/sys/%.o Drivers/AK_Imported_Driver/sys/%.su Drivers/AK_Imported_Driver/sys/%.cyclo: ../Drivers/AK_Imported_Driver/sys/%.cpp Drivers/AK_Imported_Driver/sys/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Source" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fno-exceptions -fno-rtti -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/AK_Imported_Driver/sys/%.o Drivers/AK_Imported_Driver/sys/%.su Drivers/AK_Imported_Driver/sys/%.cyclo: ../Drivers/AK_Imported_Driver/sys/%.c Drivers/AK_Imported_Driver/sys/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AK_Imported_Driver-2f-sys

clean-Drivers-2f-AK_Imported_Driver-2f-sys:
	-$(RM) ./Drivers/AK_Imported_Driver/sys/porting_compat.cyclo ./Drivers/AK_Imported_Driver/sys/porting_compat.d ./Drivers/AK_Imported_Driver/sys/porting_compat.o ./Drivers/AK_Imported_Driver/sys/porting_compat.su ./Drivers/AK_Imported_Driver/sys/sys_boot.cyclo ./Drivers/AK_Imported_Driver/sys/sys_boot.d ./Drivers/AK_Imported_Driver/sys/sys_boot.o ./Drivers/AK_Imported_Driver/sys/sys_boot.su ./Drivers/AK_Imported_Driver/sys/sys_dbg.cyclo ./Drivers/AK_Imported_Driver/sys/sys_dbg.d ./Drivers/AK_Imported_Driver/sys/sys_dbg.o ./Drivers/AK_Imported_Driver/sys/sys_dbg.su

.PHONY: clean-Drivers-2f-AK_Imported_Driver-2f-sys

