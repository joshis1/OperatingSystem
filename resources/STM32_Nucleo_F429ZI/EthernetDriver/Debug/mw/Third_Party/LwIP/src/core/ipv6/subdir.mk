################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mw/Third_Party/LwIP/src/core/ipv6/dhcp6.c \
../mw/Third_Party/LwIP/src/core/ipv6/ethip6.c \
../mw/Third_Party/LwIP/src/core/ipv6/icmp6.c \
../mw/Third_Party/LwIP/src/core/ipv6/inet6.c \
../mw/Third_Party/LwIP/src/core/ipv6/ip6.c \
../mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.c \
../mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.c \
../mw/Third_Party/LwIP/src/core/ipv6/mld6.c \
../mw/Third_Party/LwIP/src/core/ipv6/nd6.c 

OBJS += \
./mw/Third_Party/LwIP/src/core/ipv6/dhcp6.o \
./mw/Third_Party/LwIP/src/core/ipv6/ethip6.o \
./mw/Third_Party/LwIP/src/core/ipv6/icmp6.o \
./mw/Third_Party/LwIP/src/core/ipv6/inet6.o \
./mw/Third_Party/LwIP/src/core/ipv6/ip6.o \
./mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.o \
./mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.o \
./mw/Third_Party/LwIP/src/core/ipv6/mld6.o \
./mw/Third_Party/LwIP/src/core/ipv6/nd6.o 

C_DEPS += \
./mw/Third_Party/LwIP/src/core/ipv6/dhcp6.d \
./mw/Third_Party/LwIP/src/core/ipv6/ethip6.d \
./mw/Third_Party/LwIP/src/core/ipv6/icmp6.d \
./mw/Third_Party/LwIP/src/core/ipv6/inet6.d \
./mw/Third_Party/LwIP/src/core/ipv6/ip6.d \
./mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.d \
./mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.d \
./mw/Third_Party/LwIP/src/core/ipv6/mld6.d \
./mw/Third_Party/LwIP/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
mw/Third_Party/LwIP/src/core/ipv6/%.o mw/Third_Party/LwIP/src/core/ipv6/%.su mw/Third_Party/LwIP/src/core/ipv6/%.cyclo: ../mw/Third_Party/LwIP/src/core/ipv6/%.c mw/Third_Party/LwIP/src/core/ipv6/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -c -I../Inc -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/App" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/Target" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/system/arch" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/prot" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Core/Inc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -I"/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv6

clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv6:
	-$(RM) ./mw/Third_Party/LwIP/src/core/ipv6/dhcp6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/dhcp6.d ./mw/Third_Party/LwIP/src/core/ipv6/dhcp6.o ./mw/Third_Party/LwIP/src/core/ipv6/dhcp6.su ./mw/Third_Party/LwIP/src/core/ipv6/ethip6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/ethip6.d ./mw/Third_Party/LwIP/src/core/ipv6/ethip6.o ./mw/Third_Party/LwIP/src/core/ipv6/ethip6.su ./mw/Third_Party/LwIP/src/core/ipv6/icmp6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/icmp6.d ./mw/Third_Party/LwIP/src/core/ipv6/icmp6.o ./mw/Third_Party/LwIP/src/core/ipv6/icmp6.su ./mw/Third_Party/LwIP/src/core/ipv6/inet6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/inet6.d ./mw/Third_Party/LwIP/src/core/ipv6/inet6.o ./mw/Third_Party/LwIP/src/core/ipv6/inet6.su ./mw/Third_Party/LwIP/src/core/ipv6/ip6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/ip6.d ./mw/Third_Party/LwIP/src/core/ipv6/ip6.o ./mw/Third_Party/LwIP/src/core/ipv6/ip6.su ./mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.d ./mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.o ./mw/Third_Party/LwIP/src/core/ipv6/ip6_addr.su ./mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.d ./mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.o ./mw/Third_Party/LwIP/src/core/ipv6/ip6_frag.su ./mw/Third_Party/LwIP/src/core/ipv6/mld6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/mld6.d ./mw/Third_Party/LwIP/src/core/ipv6/mld6.o ./mw/Third_Party/LwIP/src/core/ipv6/mld6.su ./mw/Third_Party/LwIP/src/core/ipv6/nd6.cyclo ./mw/Third_Party/LwIP/src/core/ipv6/nd6.d ./mw/Third_Party/LwIP/src/core/ipv6/nd6.o ./mw/Third_Party/LwIP/src/core/ipv6/nd6.su

.PHONY: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv6

