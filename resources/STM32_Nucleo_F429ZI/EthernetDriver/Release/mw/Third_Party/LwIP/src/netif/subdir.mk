################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mw/Third_Party/LwIP/src/netif/bridgeif.c \
../mw/Third_Party/LwIP/src/netif/bridgeif_fdb.c \
../mw/Third_Party/LwIP/src/netif/ethernet.c \
../mw/Third_Party/LwIP/src/netif/lowpan6.c \
../mw/Third_Party/LwIP/src/netif/lowpan6_ble.c \
../mw/Third_Party/LwIP/src/netif/lowpan6_common.c \
../mw/Third_Party/LwIP/src/netif/slipif.c \
../mw/Third_Party/LwIP/src/netif/zepif.c 

OBJS += \
./mw/Third_Party/LwIP/src/netif/bridgeif.o \
./mw/Third_Party/LwIP/src/netif/bridgeif_fdb.o \
./mw/Third_Party/LwIP/src/netif/ethernet.o \
./mw/Third_Party/LwIP/src/netif/lowpan6.o \
./mw/Third_Party/LwIP/src/netif/lowpan6_ble.o \
./mw/Third_Party/LwIP/src/netif/lowpan6_common.o \
./mw/Third_Party/LwIP/src/netif/slipif.o \
./mw/Third_Party/LwIP/src/netif/zepif.o 

C_DEPS += \
./mw/Third_Party/LwIP/src/netif/bridgeif.d \
./mw/Third_Party/LwIP/src/netif/bridgeif_fdb.d \
./mw/Third_Party/LwIP/src/netif/ethernet.d \
./mw/Third_Party/LwIP/src/netif/lowpan6.d \
./mw/Third_Party/LwIP/src/netif/lowpan6_ble.d \
./mw/Third_Party/LwIP/src/netif/lowpan6_common.d \
./mw/Third_Party/LwIP/src/netif/slipif.d \
./mw/Third_Party/LwIP/src/netif/zepif.d 


# Each subdirectory must supply rules for building sources it contributes
mw/Third_Party/LwIP/src/netif/%.o mw/Third_Party/LwIP/src/netif/%.su mw/Third_Party/LwIP/src/netif/%.cyclo: ../mw/Third_Party/LwIP/src/netif/%.c mw/Third_Party/LwIP/src/netif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DDEBUG -c -I../Inc -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/system/arch" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/prot" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Core/Inc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/App" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/Target" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-netif

clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-netif:
	-$(RM) ./mw/Third_Party/LwIP/src/netif/bridgeif.cyclo ./mw/Third_Party/LwIP/src/netif/bridgeif.d ./mw/Third_Party/LwIP/src/netif/bridgeif.o ./mw/Third_Party/LwIP/src/netif/bridgeif.su ./mw/Third_Party/LwIP/src/netif/bridgeif_fdb.cyclo ./mw/Third_Party/LwIP/src/netif/bridgeif_fdb.d ./mw/Third_Party/LwIP/src/netif/bridgeif_fdb.o ./mw/Third_Party/LwIP/src/netif/bridgeif_fdb.su ./mw/Third_Party/LwIP/src/netif/ethernet.cyclo ./mw/Third_Party/LwIP/src/netif/ethernet.d ./mw/Third_Party/LwIP/src/netif/ethernet.o ./mw/Third_Party/LwIP/src/netif/ethernet.su ./mw/Third_Party/LwIP/src/netif/lowpan6.cyclo ./mw/Third_Party/LwIP/src/netif/lowpan6.d ./mw/Third_Party/LwIP/src/netif/lowpan6.o ./mw/Third_Party/LwIP/src/netif/lowpan6.su ./mw/Third_Party/LwIP/src/netif/lowpan6_ble.cyclo ./mw/Third_Party/LwIP/src/netif/lowpan6_ble.d ./mw/Third_Party/LwIP/src/netif/lowpan6_ble.o ./mw/Third_Party/LwIP/src/netif/lowpan6_ble.su ./mw/Third_Party/LwIP/src/netif/lowpan6_common.cyclo ./mw/Third_Party/LwIP/src/netif/lowpan6_common.d ./mw/Third_Party/LwIP/src/netif/lowpan6_common.o ./mw/Third_Party/LwIP/src/netif/lowpan6_common.su ./mw/Third_Party/LwIP/src/netif/slipif.cyclo ./mw/Third_Party/LwIP/src/netif/slipif.d ./mw/Third_Party/LwIP/src/netif/slipif.o ./mw/Third_Party/LwIP/src/netif/slipif.su ./mw/Third_Party/LwIP/src/netif/zepif.cyclo ./mw/Third_Party/LwIP/src/netif/zepif.d ./mw/Third_Party/LwIP/src/netif/zepif.o ./mw/Third_Party/LwIP/src/netif/zepif.su

.PHONY: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-netif

