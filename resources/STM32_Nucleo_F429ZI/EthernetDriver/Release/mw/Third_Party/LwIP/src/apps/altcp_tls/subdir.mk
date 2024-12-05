################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.c \
../mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.c 

OBJS += \
./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.o \
./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.o 

C_DEPS += \
./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.d \
./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.d 


# Each subdirectory must supply rules for building sources it contributes
mw/Third_Party/LwIP/src/apps/altcp_tls/%.o mw/Third_Party/LwIP/src/apps/altcp_tls/%.su mw/Third_Party/LwIP/src/apps/altcp_tls/%.cyclo: ../mw/Third_Party/LwIP/src/apps/altcp_tls/%.c mw/Third_Party/LwIP/src/apps/altcp_tls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DDEBUG -c -I../Inc -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/system/arch" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/prot" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Core/Inc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/App" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/Target" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-apps-2f-altcp_tls

clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-apps-2f-altcp_tls:
	-$(RM) ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.cyclo ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.d ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.o ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls.su ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.cyclo ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.d ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.o ./mw/Third_Party/LwIP/src/apps/altcp_tls/altcp_tls_mbedtls_mem.su

.PHONY: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-apps-2f-altcp_tls

