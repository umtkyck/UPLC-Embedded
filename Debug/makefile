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
UK1502_UPLC.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
UK1502_UPLC.list \

OBJCOPY_HEX += \
UK1502_UPLC.hex \

OBJCOPY_BIN += \
UK1502_UPLC.bin \


# All Target
all: UK1502_UPLC.elf secondary-outputs

# Tool invocations
UK1502_UPLC.elf: $(OBJS) $(USER_OBJS) C:\Users\Umit\Documents\SourceTree\UPLC-Embedded\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "UK1502_UPLC.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Umit\Documents\SourceTree\UPLC-Embedded\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="UK1502_UPLC.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

UK1502_UPLC.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "UK1502_UPLC.list"
	@echo 'Finished building: $@'
	@echo ' '

UK1502_UPLC.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "UK1502_UPLC.hex"
	@echo 'Finished building: $@'
	@echo ' '

UK1502_UPLC.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "UK1502_UPLC.bin"
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
