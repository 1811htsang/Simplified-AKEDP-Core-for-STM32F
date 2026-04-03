################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.cpp \
../Drivers/AK_Imported_Driver/app/screens/scr_idle.cpp \
../Drivers/AK_Imported_Driver/app/screens/scr_info.cpp \
../Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.cpp \
../Drivers/AK_Imported_Driver/app/screens/scr_noen.cpp \
../Drivers/AK_Imported_Driver/app/screens/scr_startup.cpp 

OBJS += \
./Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.o \
./Drivers/AK_Imported_Driver/app/screens/scr_idle.o \
./Drivers/AK_Imported_Driver/app/screens/scr_info.o \
./Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.o \
./Drivers/AK_Imported_Driver/app/screens/scr_noen.o \
./Drivers/AK_Imported_Driver/app/screens/scr_startup.o 

CPP_DEPS += \
./Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.d \
./Drivers/AK_Imported_Driver/app/screens/scr_idle.d \
./Drivers/AK_Imported_Driver/app/screens/scr_info.d \
./Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.d \
./Drivers/AK_Imported_Driver/app/screens/scr_noen.d \
./Drivers/AK_Imported_Driver/app/screens/scr_startup.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AK_Imported_Driver/app/screens/%.o Drivers/AK_Imported_Driver/app/screens/%.su Drivers/AK_Imported_Driver/app/screens/%.cyclo: ../Drivers/AK_Imported_Driver/app/screens/%.cpp Drivers/AK_Imported_Driver/app/screens/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/CMSIS/Device/ST/STM32F1xx/Source" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/STM32F1xx_HAL_Driver/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Core/Src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/inc" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/ak/src" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/sys" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/common/container" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app" -I"C:/Users/shanghuang/Documents/PERSON-PROJECT-CV/AK-STM32/stm32f_porting/AK Firmware F103 Custom Porting/Drivers/AK_Imported_Driver/app/screens" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fno-exceptions -fno-rtti -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-AK_Imported_Driver-2f-app-2f-screens

clean-Drivers-2f-AK_Imported_Driver-2f-app-2f-screens:
	-$(RM) ./Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.cyclo ./Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.d ./Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.o ./Drivers/AK_Imported_Driver/app/screens/scr_es35sw_th_sensor.su ./Drivers/AK_Imported_Driver/app/screens/scr_idle.cyclo ./Drivers/AK_Imported_Driver/app/screens/scr_idle.d ./Drivers/AK_Imported_Driver/app/screens/scr_idle.o ./Drivers/AK_Imported_Driver/app/screens/scr_idle.su ./Drivers/AK_Imported_Driver/app/screens/scr_info.cyclo ./Drivers/AK_Imported_Driver/app/screens/scr_info.d ./Drivers/AK_Imported_Driver/app/screens/scr_info.o ./Drivers/AK_Imported_Driver/app/screens/scr_info.su ./Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.cyclo ./Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.d ./Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.o ./Drivers/AK_Imported_Driver/app/screens/scr_lhio404_io_device.su ./Drivers/AK_Imported_Driver/app/screens/scr_noen.cyclo ./Drivers/AK_Imported_Driver/app/screens/scr_noen.d ./Drivers/AK_Imported_Driver/app/screens/scr_noen.o ./Drivers/AK_Imported_Driver/app/screens/scr_noen.su ./Drivers/AK_Imported_Driver/app/screens/scr_startup.cyclo ./Drivers/AK_Imported_Driver/app/screens/scr_startup.d ./Drivers/AK_Imported_Driver/app/screens/scr_startup.o ./Drivers/AK_Imported_Driver/app/screens/scr_startup.su

.PHONY: clean-Drivers-2f-AK_Imported_Driver-2f-app-2f-screens

