################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include mw/Third_Party/LwIP/system/OS/subdir.mk
-include mw/Third_Party/LwIP/src/netif/ppp/polarssl/subdir.mk
-include mw/Third_Party/LwIP/src/netif/ppp/subdir.mk
-include mw/Third_Party/LwIP/src/netif/subdir.mk
-include mw/Third_Party/LwIP/src/core/ipv6/subdir.mk
-include mw/Third_Party/LwIP/src/core/ipv4/subdir.mk
-include mw/Third_Party/LwIP/src/core/subdir.mk
-include mw/Third_Party/LwIP/src/apps/tftp/subdir.mk
-include mw/Third_Party/LwIP/src/apps/sntp/subdir.mk
-include mw/Third_Party/LwIP/src/apps/snmp/subdir.mk
-include mw/Third_Party/LwIP/src/apps/smtp/subdir.mk
-include mw/Third_Party/LwIP/src/apps/netbiosns/subdir.mk
-include mw/Third_Party/LwIP/src/apps/mqtt/subdir.mk
-include mw/Third_Party/LwIP/src/apps/mdns/subdir.mk
-include mw/Third_Party/LwIP/src/apps/lwiperf/subdir.mk
-include mw/Third_Party/LwIP/src/apps/altcp_tls/subdir.mk
-include mw/Third_Party/LwIP/src/api/subdir.mk
-include LWIP/Target/subdir.mk
-include LWIP/App/subdir.mk
-include Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/subdir.mk
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


BUILD_ARTIFACT_NAME := EthernetDriver
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EthernetDriver.elf \

MAP_FILES += \
EthernetDriver.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EthernetDriver.list \


# All Target
all: main-build

# Main-build Target
main-build: EthernetDriver.elf secondary-outputs

# Tool invocations
EthernetDriver.elf EthernetDriver.map: $(OBJS) $(USER_OBJS) C:\Users\shrey\Downloads\OperatingSystem\resources\STM32_Nucleo_F429ZI\EthernetDriver\STM32F429ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "EthernetDriver.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\shrey\Downloads\OperatingSystem\resources\STM32_Nucleo_F429ZI\EthernetDriver\STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="EthernetDriver.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EthernetDriver.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EthernetDriver.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) EthernetDriver.elf EthernetDriver.list EthernetDriver.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
