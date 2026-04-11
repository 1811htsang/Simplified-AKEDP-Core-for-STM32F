################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/EDP/app/app.cpp \
../Drivers/EDP/app/task_if.cpp \
../Drivers/EDP/app/task_list.cpp 

OBJS += \
./Drivers/EDP/app/app.o \
./Drivers/EDP/app/task_if.o \
./Drivers/EDP/app/task_list.o 

CPP_DEPS += \
./Drivers/EDP/app/app.d \
./Drivers/EDP/app/task_if.d \
./Drivers/EDP/app/task_list.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/EDP/app/%.o Drivers/EDP/app/%.su Drivers/EDP/app/%.cyclo: ../Drivers/EDP/app/%.cpp Drivers/EDP/app/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Source" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/EDP/sys" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Test/Sample" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fno-exceptions -fno-rtti -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-EDP-2f-app

clean-Drivers-2f-EDP-2f-app:
	-$(RM) ./Drivers/EDP/app/app.cyclo ./Drivers/EDP/app/app.d ./Drivers/EDP/app/app.o ./Drivers/EDP/app/app.su ./Drivers/EDP/app/task_if.cyclo ./Drivers/EDP/app/task_if.d ./Drivers/EDP/app/task_if.o ./Drivers/EDP/app/task_if.su ./Drivers/EDP/app/task_list.cyclo ./Drivers/EDP/app/task_list.d ./Drivers/EDP/app/task_list.o ./Drivers/EDP/app/task_list.su

.PHONY: clean-Drivers-2f-EDP-2f-app

