################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
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

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SciSonicFirmware.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SciSonicFirmware.list \

OBJCOPY_HEX += \
SciSonicFirmware.hex \

OBJCOPY_BIN += \
SciSonicFirmware.bin \


# All Target
all: SciSonicFirmware.elf secondary-outputs

# Tool invocations
SciSonicFirmware.elf: $(OBJS) $(USER_OBJS) C:\Users\MesutSen\Documents\Mesut_Proj\SciSonic_Ladder\MicroPLC-20200727T155945Z-001\MicroPLC\SciSonicFirmware_Source_Code\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "SciSonicFirmware.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\MesutSen\Documents\Mesut_Proj\SciSonic_Ladder\MicroPLC-20200727T155945Z-001\MicroPLC\SciSonicFirmware_Source_Code\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="SciSonicFirmware.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SciSonicFirmware.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SciSonicFirmware.list"
	@echo 'Finished building: $@'
	@echo ' '

SciSonicFirmware.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "SciSonicFirmware.hex"
	@echo 'Finished building: $@'
	@echo ' '

SciSonicFirmware.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "SciSonicFirmware.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
