################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src\ (copy\ 20180514\ \ )/main.c 

OBJS += \
./src\ (copy\ 20180514\ \ )/main.o 

C_DEPS += \
./src\ (copy\ 20180514\ \ )/main.d 


# Each subdirectory must supply rules for building sources it contributes
src\ (copy\ 20180514\ \ )/main.o: ../src\ (copy\ 20180514\ \ )/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega168p -DF_CPU=12288000UL -MMD -MP -MF"src (copy 20180514  )/main.d" -MT"src\ (copy\ 20180514\ \ )/main.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


