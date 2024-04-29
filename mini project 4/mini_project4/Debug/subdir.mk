################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Measure\ the\ distance.c \
../gpio.c \
../icu.c \
../lcd.c \
../ultrasonic.c 

OBJS += \
./Measure\ the\ distance.o \
./gpio.o \
./icu.o \
./lcd.o \
./ultrasonic.o 

C_DEPS += \
./Measure\ the\ distance.d \
./gpio.d \
./icu.d \
./lcd.d \
./ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
Measure\ the\ distance.o: ../Measure\ the\ distance.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"Measure the distance.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


