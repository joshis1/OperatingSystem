################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mw/Third_Party/LwIP/src/core/ipv4/autoip.c \
../mw/Third_Party/LwIP/src/core/ipv4/dhcp.c \
../mw/Third_Party/LwIP/src/core/ipv4/etharp.c \
../mw/Third_Party/LwIP/src/core/ipv4/icmp.c \
../mw/Third_Party/LwIP/src/core/ipv4/igmp.c \
../mw/Third_Party/LwIP/src/core/ipv4/ip4.c \
../mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.c \
../mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.c 

OBJS += \
./mw/Third_Party/LwIP/src/core/ipv4/autoip.o \
./mw/Third_Party/LwIP/src/core/ipv4/dhcp.o \
./mw/Third_Party/LwIP/src/core/ipv4/etharp.o \
./mw/Third_Party/LwIP/src/core/ipv4/icmp.o \
./mw/Third_Party/LwIP/src/core/ipv4/igmp.o \
./mw/Third_Party/LwIP/src/core/ipv4/ip4.o \
./mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.o \
./mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./mw/Third_Party/LwIP/src/core/ipv4/autoip.d \
./mw/Third_Party/LwIP/src/core/ipv4/dhcp.d \
./mw/Third_Party/LwIP/src/core/ipv4/etharp.d \
./mw/Third_Party/LwIP/src/core/ipv4/icmp.d \
./mw/Third_Party/LwIP/src/core/ipv4/igmp.d \
./mw/Third_Party/LwIP/src/core/ipv4/ip4.d \
./mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.d \
./mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
mw/Third_Party/LwIP/src/core/ipv4/%.o mw/Third_Party/LwIP/src/core/ipv4/%.su mw/Third_Party/LwIP/src/core/ipv4/%.cyclo: ../mw/Third_Party/LwIP/src/core/ipv4/%.c mw/Third_Party/LwIP/src/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -c -I../Inc -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/App" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/Target" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/system/arch" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/prot" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Core/Inc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -I"/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4

clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4:
	-$(RM) ./mw/Third_Party/LwIP/src/core/ipv4/autoip.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/autoip.d ./mw/Third_Party/LwIP/src/core/ipv4/autoip.o ./mw/Third_Party/LwIP/src/core/ipv4/autoip.su ./mw/Third_Party/LwIP/src/core/ipv4/dhcp.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/dhcp.d ./mw/Third_Party/LwIP/src/core/ipv4/dhcp.o ./mw/Third_Party/LwIP/src/core/ipv4/dhcp.su ./mw/Third_Party/LwIP/src/core/ipv4/etharp.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/etharp.d ./mw/Third_Party/LwIP/src/core/ipv4/etharp.o ./mw/Third_Party/LwIP/src/core/ipv4/etharp.su ./mw/Third_Party/LwIP/src/core/ipv4/icmp.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/icmp.d ./mw/Third_Party/LwIP/src/core/ipv4/icmp.o ./mw/Third_Party/LwIP/src/core/ipv4/icmp.su ./mw/Third_Party/LwIP/src/core/ipv4/igmp.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/igmp.d ./mw/Third_Party/LwIP/src/core/ipv4/igmp.o ./mw/Third_Party/LwIP/src/core/ipv4/igmp.su ./mw/Third_Party/LwIP/src/core/ipv4/ip4.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/ip4.d ./mw/Third_Party/LwIP/src/core/ipv4/ip4.o ./mw/Third_Party/LwIP/src/core/ipv4/ip4.su ./mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.d ./mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.o ./mw/Third_Party/LwIP/src/core/ipv4/ip4_addr.su ./mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.cyclo ./mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.d ./mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.o ./mw/Third_Party/LwIP/src/core/ipv4/ip4_frag.su

.PHONY: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4

