################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/uart/subdir.mk
-include src/timer/subdir.mk
-include src/sdcard/Fatfs/subdir.mk
-include src/sdcard/subdir.mk
-include src/gsm/subdir.mk
-include src/Tcp/subdir.mk
-include src/Led/subdir.mk
-include src/GPS/subdir.mk
-include src/GPIO/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: KHNLPC1764.axf

# Tool invocations
KHNLPC1764.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Long\Documents\LPCXpresso_7.0.0_92\workspace\CMSISv2p00_LPC17xx\Debug" -Xlinker --gc-sections -Xlinker -Map=KHNLPC1764.map -mcpu=cortex-m3 -mthumb -T "KHNLPC1764_Debug.ld" -o "KHNLPC1764.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) KHNLPC1764.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size KHNLPC1764.axf; arm-none-eabi-objcopy -O binary KHNLPC1764.axf KHNLPC1764.bin ; checksum -p LPC1768 -d KHNLPC1764.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
