################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Drivers/AK_Imported_Driver/sys/subdir.mk
-include Drivers/AK_Imported_Driver/common/container/subdir.mk
-include Drivers/AK_Imported_Driver/common/subdir.mk
-include Drivers/AK_Imported_Driver/app/screens/subdir.mk
-include Drivers/AK_Imported_Driver/app/subdir.mk
-include Drivers/AK_Imported_Driver/ak/src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := AK Firmware F103 Custom Porting
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
AK\ Firmware\ F103\ Custom\ Porting.elf \

MAP_FILES += \
AK\ Firmware\ F103\ Custom\ Porting.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
AK\ Firmware\ F103\ Custom\ Porting.list \


# All Target
all: main-build

# Main-build Target
main-build: AK\ Firmware\ F103\ Custom\ Porting.elf secondary-outputs

# Tool invocations
AK\ Firmware\ F103\ Custom\ Porting.elf AK\ Firmware\ F103\ Custom\ Porting.map: $(OBJS) $(USER_OBJS) C:\Users\shanghuang\Documents\PERSON-PROJECT-CV\AK-STM32\stm32f_porting\AK\ Firmware\ F103\ Custom\ Porting\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "AK Firmware F103 Custom Porting.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\shanghuang\Documents\PERSON-PROJECT-CV\AK-STM32\stm32f_porting\AK Firmware F103 Custom Porting\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="AK Firmware F103 Custom Porting.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  "$(BUILD_ARTIFACT)"
	@echo 'Finished building: $@'
	@echo ' '

AK\ Firmware\ F103\ Custom\ Porting.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S "$(BUILD_ARTIFACT)" > "AK Firmware F103 Custom Porting.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) AK\ Firmware\ F103\ Custom\ Porting.elf AK\ Firmware\ F103\ Custom\ Porting.list AK\ Firmware\ F103\ Custom\ Porting.map AK\\ Firmware\\ F103\\ Custom\\ Porting.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
