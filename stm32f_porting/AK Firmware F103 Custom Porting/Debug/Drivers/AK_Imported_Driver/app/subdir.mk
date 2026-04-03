################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/AK_Imported_Driver/app/app.cpp \
../Drivers/AK_Imported_Driver/app/app_bsp.cpp \
../Drivers/AK_Imported_Driver/app/app_data.cpp \
../Drivers/AK_Imported_Driver/app/app_modbus_pull.cpp \
../Drivers/AK_Imported_Driver/app/app_non_clear_ram.cpp \
../Drivers/AK_Imported_Driver/app/shell.cpp \
../Drivers/AK_Imported_Driver/app/task_dbg.cpp \
../Drivers/AK_Imported_Driver/app/task_display.cpp \
../Drivers/AK_Imported_Driver/app/task_fw.cpp \
../Drivers/AK_Imported_Driver/app/task_if.cpp \
../Drivers/AK_Imported_Driver/app/task_life.cpp \
../Drivers/AK_Imported_Driver/app/task_list.cpp \
../Drivers/AK_Imported_Driver/app/task_rf24_if.cpp \
../Drivers/AK_Imported_Driver/app/task_shell.cpp \
../Drivers/AK_Imported_Driver/app/task_system.cpp \
../Drivers/AK_Imported_Driver/app/task_uart_if.cpp \
../Drivers/AK_Imported_Driver/app/task_zigbee.cpp 

OBJS += \
./Drivers/AK_Imported_Driver/app/app.o \
./Drivers/AK_Imported_Driver/app/app_bsp.o \
./Drivers/AK_Imported_Driver/app/app_data.o \
./Drivers/AK_Imported_Driver/app/app_modbus_pull.o \
./Drivers/AK_Imported_Driver/app/app_non_clear_ram.o \
./Drivers/AK_Imported_Driver/app/shell.o \
./Drivers/AK_Imported_Driver/app/task_dbg.o \
./Drivers/AK_Imported_Driver/app/task_display.o \
./Drivers/AK_Imported_Driver/app/task_fw.o \
./Drivers/AK_Imported_Driver/app/task_if.o \
./Drivers/AK_Imported_Driver/app/task_life.o \
./Drivers/AK_Imported_Driver/app/task_list.o \
./Drivers/AK_Imported_Driver/app/task_rf24_if.o \
./Drivers/AK_Imported_Driver/app/task_shell.o \
./Drivers/AK_Imported_Driver/app/task_system.o \
./Drivers/AK_Imported_Driver/app/task_uart_if.o \
./Drivers/AK_Imported_Driver/app/task_zigbee.o 

CPP_DEPS += \
./Drivers/AK_Imported_Driver/app/app.d \
./Drivers/AK_Imported_Driver/app/app_bsp.d \
./Drivers/AK_Imported_Driver/app/app_data.d \
./Drivers/AK_Imported_Driver/app/app_modbus_pull.d \
./Drivers/AK_Imported_Driver/app/app_non_clear_ram.d \
./Drivers/AK_Imported_Driver/app/shell.d \
./Drivers/AK_Imported_Driver/app/task_dbg.d \
./Drivers/AK_Imported_Driver/app/task_display.d \
./Drivers/AK_Imported_Driver/app/task_fw.d \
./Drivers/AK_Imported_Driver/app/task_if.d \
./Drivers/AK_Imported_Driver/app/task_life.d \
./Drivers/AK_Imported_Driver/app/task_list.d \
./Drivers/AK_Imported_Driver/app/task_rf24_if.d \
./Drivers/AK_Imported_Driver/app/task_shell.d \
./Drivers/AK_Imported_Driver/app/task_system.d \
./Drivers/AK_Imported_Driver/app/task_uart_if.d \
./Drivers/AK_Imported_Driver/app/task_zigbee.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AK_Imported_Driver/app/%.o Drivers/AK_Imported_Driver/app/%.su Drivers/AK_Imported_Driver/app/%.cyclo: ../Drivers/AK_Imported_Driver/app/%.cpp Drivers/AK_Imported_Driver/app/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Source" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fno-exceptions -fno-rtti -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AK_Imported_Driver-2f-app

clean-Drivers-2f-AK_Imported_Driver-2f-app:
	-$(RM) ./Drivers/AK_Imported_Driver/app/app.cyclo ./Drivers/AK_Imported_Driver/app/app.d ./Drivers/AK_Imported_Driver/app/app.o ./Drivers/AK_Imported_Driver/app/app.su ./Drivers/AK_Imported_Driver/app/app_bsp.cyclo ./Drivers/AK_Imported_Driver/app/app_bsp.d ./Drivers/AK_Imported_Driver/app/app_bsp.o ./Drivers/AK_Imported_Driver/app/app_bsp.su ./Drivers/AK_Imported_Driver/app/app_data.cyclo ./Drivers/AK_Imported_Driver/app/app_data.d ./Drivers/AK_Imported_Driver/app/app_data.o ./Drivers/AK_Imported_Driver/app/app_data.su ./Drivers/AK_Imported_Driver/app/app_modbus_pull.cyclo ./Drivers/AK_Imported_Driver/app/app_modbus_pull.d ./Drivers/AK_Imported_Driver/app/app_modbus_pull.o ./Drivers/AK_Imported_Driver/app/app_modbus_pull.su ./Drivers/AK_Imported_Driver/app/app_non_clear_ram.cyclo ./Drivers/AK_Imported_Driver/app/app_non_clear_ram.d ./Drivers/AK_Imported_Driver/app/app_non_clear_ram.o ./Drivers/AK_Imported_Driver/app/app_non_clear_ram.su ./Drivers/AK_Imported_Driver/app/shell.cyclo ./Drivers/AK_Imported_Driver/app/shell.d ./Drivers/AK_Imported_Driver/app/shell.o ./Drivers/AK_Imported_Driver/app/shell.su ./Drivers/AK_Imported_Driver/app/task_dbg.cyclo ./Drivers/AK_Imported_Driver/app/task_dbg.d ./Drivers/AK_Imported_Driver/app/task_dbg.o ./Drivers/AK_Imported_Driver/app/task_dbg.su ./Drivers/AK_Imported_Driver/app/task_display.cyclo ./Drivers/AK_Imported_Driver/app/task_display.d ./Drivers/AK_Imported_Driver/app/task_display.o ./Drivers/AK_Imported_Driver/app/task_display.su ./Drivers/AK_Imported_Driver/app/task_fw.cyclo ./Drivers/AK_Imported_Driver/app/task_fw.d ./Drivers/AK_Imported_Driver/app/task_fw.o ./Drivers/AK_Imported_Driver/app/task_fw.su ./Drivers/AK_Imported_Driver/app/task_if.cyclo ./Drivers/AK_Imported_Driver/app/task_if.d ./Drivers/AK_Imported_Driver/app/task_if.o ./Drivers/AK_Imported_Driver/app/task_if.su ./Drivers/AK_Imported_Driver/app/task_life.cyclo ./Drivers/AK_Imported_Driver/app/task_life.d ./Drivers/AK_Imported_Driver/app/task_life.o ./Drivers/AK_Imported_Driver/app/task_life.su ./Drivers/AK_Imported_Driver/app/task_list.cyclo ./Drivers/AK_Imported_Driver/app/task_list.d ./Drivers/AK_Imported_Driver/app/task_list.o ./Drivers/AK_Imported_Driver/app/task_list.su ./Drivers/AK_Imported_Driver/app/task_rf24_if.cyclo ./Drivers/AK_Imported_Driver/app/task_rf24_if.d ./Drivers/AK_Imported_Driver/app/task_rf24_if.o ./Drivers/AK_Imported_Driver/app/task_rf24_if.su ./Drivers/AK_Imported_Driver/app/task_shell.cyclo ./Drivers/AK_Imported_Driver/app/task_shell.d ./Drivers/AK_Imported_Driver/app/task_shell.o ./Drivers/AK_Imported_Driver/app/task_shell.su ./Drivers/AK_Imported_Driver/app/task_system.cyclo ./Drivers/AK_Imported_Driver/app/task_system.d ./Drivers/AK_Imported_Driver/app/task_system.o ./Drivers/AK_Imported_Driver/app/task_system.su ./Drivers/AK_Imported_Driver/app/task_uart_if.cyclo ./Drivers/AK_Imported_Driver/app/task_uart_if.d ./Drivers/AK_Imported_Driver/app/task_uart_if.o ./Drivers/AK_Imported_Driver/app/task_uart_if.su ./Drivers/AK_Imported_Driver/app/task_zigbee.cyclo ./Drivers/AK_Imported_Driver/app/task_zigbee.d ./Drivers/AK_Imported_Driver/app/task_zigbee.o ./Drivers/AK_Imported_Driver/app/task_zigbee.su

.PHONY: clean-Drivers-2f-AK_Imported_Driver-2f-app

