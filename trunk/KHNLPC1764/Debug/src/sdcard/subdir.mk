################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sdcard/khn_gsht.c \
../src/sdcard/sd.c \
../src/sdcard/spi.c 

OBJS += \
./src/sdcard/khn_gsht.o \
./src/sdcard/sd.o \
./src/sdcard/spi.o 

C_DEPS += \
./src/sdcard/khn_gsht.d \
./src/sdcard/sd.d \
./src/sdcard/spi.d 


# Each subdirectory must supply rules for building sources it contributes
src/sdcard/%.o: ../src/sdcard/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__CODE_RED -D__REDLIB__ -I"C:\Users\Long\Documents\LPCXpresso_7.0.0_92\workspace\CMSISv2p00_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


