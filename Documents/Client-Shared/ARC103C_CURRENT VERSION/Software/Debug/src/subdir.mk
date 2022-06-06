################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/hardware.c \
../src/interrupts.c \
../src/lcd.c \
../src/main.c \
../src/pwm.c \
../src/sys.c \
../src/timer.c \
../src/uart.c \
../src/utils.c 

OBJS += \
./src/adc.o \
./src/hardware.o \
./src/interrupts.o \
./src/lcd.o \
./src/main.o \
./src/pwm.o \
./src/sys.o \
./src/timer.o \
./src/uart.o \
./src/utils.o 

C_DEPS += \
./src/adc.d \
./src/hardware.d \
./src/interrupts.d \
./src/lcd.d \
./src/main.d \
./src/pwm.d \
./src/sys.d \
./src/timer.d \
./src/uart.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=atmega168p -DF_CPU=12288000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


