################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
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
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Prosthetic Hand Prototype
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Prosthetic\ Hand\ Prototype.elf \

MAP_FILES += \
Prosthetic\ Hand\ Prototype.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Prosthetic\ Hand\ Prototype.list \

OBJCOPY_HEX += \
Prosthetic\ Hand\ Prototype.hex \

OBJCOPY_BIN += \
Prosthetic\ Hand\ Prototype.bin \


# All Target
all: main-build

# Main-build Target
main-build: Prosthetic\ Hand\ Prototype.elf secondary-outputs

# Tool invocations
Prosthetic\ Hand\ Prototype.elf Prosthetic\ Hand\ Prototype.map: $(OBJS) $(USER_OBJS) E:\EiT\Magisterka\Prosthetic-Hand-Prototype\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Prosthetic Hand Prototype.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"E:\EiT\Magisterka\Prosthetic-Hand-Prototype\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Prosthetic Hand Prototype.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Prosthetic\ Hand\ Prototype.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Prosthetic Hand Prototype.list"
	@echo 'Finished building: $@'
	@echo ' '

Prosthetic\ Hand\ Prototype.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Prosthetic Hand Prototype.hex"
	@echo 'Finished building: $@'
	@echo ' '

Prosthetic\ Hand\ Prototype.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Prosthetic Hand Prototype.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Prosthetic\ Hand\ Prototype.bin Prosthetic\ Hand\ Prototype.elf Prosthetic\ Hand\ Prototype.hex Prosthetic\ Hand\ Prototype.list Prosthetic\ Hand\ Prototype.map Prosthetic\\ Hand\\ Prototype.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
