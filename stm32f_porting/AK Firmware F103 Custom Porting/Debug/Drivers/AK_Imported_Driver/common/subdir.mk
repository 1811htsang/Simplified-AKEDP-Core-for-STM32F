################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/AK_Imported_Driver/common/screen_manager.cpp \
../Drivers/AK_Imported_Driver/common/view_item.cpp \
../Drivers/AK_Imported_Driver/common/view_render.cpp 

C_SRCS += \
../Drivers/AK_Imported_Driver/common/cmd_line.c \
../Drivers/AK_Imported_Driver/common/utils.c \
../Drivers/AK_Imported_Driver/common/xprintf.c 

C_DEPS += \
./Drivers/AK_Imported_Driver/common/cmd_line.d \
./Drivers/AK_Imported_Driver/common/utils.d \
./Drivers/AK_Imported_Driver/common/xprintf.d 

OBJS += \
./Drivers/AK_Imported_Driver/common/cmd_line.o \
./Drivers/AK_Imported_Driver/common/screen_manager.o \
./Drivers/AK_Imported_Driver/common/utils.o \
./Drivers/AK_Imported_Driver/common/view_item.o \
./Drivers/AK_Imported_Driver/common/view_render.o \
./Drivers/AK_Imported_Driver/common/xprintf.o 

CPP_DEPS += \
./Drivers/AK_Imported_Driver/common/screen_manager.d \
./Drivers/AK_Imported_Driver/common/view_item.d \
./Drivers/AK_Imported_Driver/common/view_render.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AK_Imported_Driver/common/%.o Drivers/AK_Imported_Driver/common/%.su Drivers/AK_Imported_Driver/common/%.cyclo: ../Drivers/AK_Imported_Driver/common/%.c Drivers/AK_Imported_Driver/common/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/AK_Imported_Driver/common/%.o Drivers/AK_Imported_Driver/common/%.su Drivers/AK_Imported_Driver/common/%.cyclo: ../Drivers/AK_Imported_Driver/common/%.cpp Drivers/AK_Imported_Driver/common/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Source" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fno-exceptions -fno-rtti -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AK_Imported_Driver-2f-common

clean-Drivers-2f-AK_Imported_Driver-2f-common:
	-$(RM) ./Drivers/AK_Imported_Driver/common/cmd_line.cyclo ./Drivers/AK_Imported_Driver/common/cmd_line.d ./Drivers/AK_Imported_Driver/common/cmd_line.o ./Drivers/AK_Imported_Driver/common/cmd_line.su ./Drivers/AK_Imported_Driver/common/screen_manager.cyclo ./Drivers/AK_Imported_Driver/common/screen_manager.d ./Drivers/AK_Imported_Driver/common/screen_manager.o ./Drivers/AK_Imported_Driver/common/screen_manager.su ./Drivers/AK_Imported_Driver/common/utils.cyclo ./Drivers/AK_Imported_Driver/common/utils.d ./Drivers/AK_Imported_Driver/common/utils.o ./Drivers/AK_Imported_Driver/common/utils.su ./Drivers/AK_Imported_Driver/common/view_item.cyclo ./Drivers/AK_Imported_Driver/common/view_item.d ./Drivers/AK_Imported_Driver/common/view_item.o ./Drivers/AK_Imported_Driver/common/view_item.su ./Drivers/AK_Imported_Driver/common/view_render.cyclo ./Drivers/AK_Imported_Driver/common/view_render.d ./Drivers/AK_Imported_Driver/common/view_render.o ./Drivers/AK_Imported_Driver/common/view_render.su ./Drivers/AK_Imported_Driver/common/xprintf.cyclo ./Drivers/AK_Imported_Driver/common/xprintf.d ./Drivers/AK_Imported_Driver/common/xprintf.o ./Drivers/AK_Imported_Driver/common/xprintf.su

.PHONY: clean-Drivers-2f-AK_Imported_Driver-2f-common

