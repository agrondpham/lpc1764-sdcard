################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/uart/rs232.c \
../src/uart/uart.c 

OBJS += \
./src/uart/rs232.o \
./src/uart/uart.o 

C_DEPS += \
./src/uart/rs232.d \
./src/uart/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/uart/%.o: ../src/uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__CODE_RED -D__REDLIB__ -I"C:\Users\Long\Documents\LPCXpresso_7.0.0_92\workspace\CMSISv2p00_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


