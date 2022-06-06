################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Reel-ATMega168.lss \

FLASH_IMAGE += \
Reel-ATMega168.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: Reel-ATMega168.elf secondary-outputs

# Tool invocations
Reel-ATMega168.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Reel-ATMega168.map -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=atmega168p -o "Reel-ATMega168.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Reel-ATMega168.lss: Reel-ATMega168.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Reel-ATMega168.elf  >"Reel-ATMega168.lss"
	@echo 'Finished building: $@'
	@echo ' '

Reel-ATMega168.hex: Reel-ATMega168.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Reel-ATMega168.elf  "Reel-ATMega168.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Reel-ATMega168.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega168p Reel-ATMega168.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) Reel-ATMega168.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
