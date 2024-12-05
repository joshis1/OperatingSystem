################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mw/Third_Party/LwIP/src/api/api_lib.c \
../mw/Third_Party/LwIP/src/api/api_msg.c \
../mw/Third_Party/LwIP/src/api/err.c \
../mw/Third_Party/LwIP/src/api/if_api.c \
../mw/Third_Party/LwIP/src/api/netbuf.c \
../mw/Third_Party/LwIP/src/api/netdb.c \
../mw/Third_Party/LwIP/src/api/netifapi.c \
../mw/Third_Party/LwIP/src/api/sockets.c \
../mw/Third_Party/LwIP/src/api/tcpip.c 

OBJS += \
./mw/Third_Party/LwIP/src/api/api_lib.o \
./mw/Third_Party/LwIP/src/api/api_msg.o \
./mw/Third_Party/LwIP/src/api/err.o \
./mw/Third_Party/LwIP/src/api/if_api.o \
./mw/Third_Party/LwIP/src/api/netbuf.o \
./mw/Third_Party/LwIP/src/api/netdb.o \
./mw/Third_Party/LwIP/src/api/netifapi.o \
./mw/Third_Party/LwIP/src/api/sockets.o \
./mw/Third_Party/LwIP/src/api/tcpip.o 

C_DEPS += \
./mw/Third_Party/LwIP/src/api/api_lib.d \
./mw/Third_Party/LwIP/src/api/api_msg.d \
./mw/Third_Party/LwIP/src/api/err.d \
./mw/Third_Party/LwIP/src/api/if_api.d \
./mw/Third_Party/LwIP/src/api/netbuf.d \
./mw/Third_Party/LwIP/src/api/netdb.d \
./mw/Third_Party/LwIP/src/api/netifapi.d \
./mw/Third_Party/LwIP/src/api/sockets.d \
./mw/Third_Party/LwIP/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
mw/Third_Party/LwIP/src/api/%.o mw/Third_Party/LwIP/src/api/%.su mw/Third_Party/LwIP/src/api/%.cyclo: ../mw/Third_Party/LwIP/src/api/%.c mw/Third_Party/LwIP/src/api/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -c -I../Inc -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/App" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/LWIP/Target" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/system/arch" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/prot" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/priv" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/lwip/apps" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/Core/Inc" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/netif/ppp/polarssl" -I"C:/Users/shrey/Downloads/OperatingSystem/resources/STM32_Nucleo_F429ZI/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -I"/EthernetDriver/mw/Third_Party/LwIP/src/include/compat/stdc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-api

clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-api:
	-$(RM) ./mw/Third_Party/LwIP/src/api/api_lib.cyclo ./mw/Third_Party/LwIP/src/api/api_lib.d ./mw/Third_Party/LwIP/src/api/api_lib.o ./mw/Third_Party/LwIP/src/api/api_lib.su ./mw/Third_Party/LwIP/src/api/api_msg.cyclo ./mw/Third_Party/LwIP/src/api/api_msg.d ./mw/Third_Party/LwIP/src/api/api_msg.o ./mw/Third_Party/LwIP/src/api/api_msg.su ./mw/Third_Party/LwIP/src/api/err.cyclo ./mw/Third_Party/LwIP/src/api/err.d ./mw/Third_Party/LwIP/src/api/err.o ./mw/Third_Party/LwIP/src/api/err.su ./mw/Third_Party/LwIP/src/api/if_api.cyclo ./mw/Third_Party/LwIP/src/api/if_api.d ./mw/Third_Party/LwIP/src/api/if_api.o ./mw/Third_Party/LwIP/src/api/if_api.su ./mw/Third_Party/LwIP/src/api/netbuf.cyclo ./mw/Third_Party/LwIP/src/api/netbuf.d ./mw/Third_Party/LwIP/src/api/netbuf.o ./mw/Third_Party/LwIP/src/api/netbuf.su ./mw/Third_Party/LwIP/src/api/netdb.cyclo ./mw/Third_Party/LwIP/src/api/netdb.d ./mw/Third_Party/LwIP/src/api/netdb.o ./mw/Third_Party/LwIP/src/api/netdb.su ./mw/Third_Party/LwIP/src/api/netifapi.cyclo ./mw/Third_Party/LwIP/src/api/netifapi.d ./mw/Third_Party/LwIP/src/api/netifapi.o ./mw/Third_Party/LwIP/src/api/netifapi.su ./mw/Third_Party/LwIP/src/api/sockets.cyclo ./mw/Third_Party/LwIP/src/api/sockets.d ./mw/Third_Party/LwIP/src/api/sockets.o ./mw/Third_Party/LwIP/src/api/sockets.su ./mw/Third_Party/LwIP/src/api/tcpip.cyclo ./mw/Third_Party/LwIP/src/api/tcpip.d ./mw/Third_Party/LwIP/src/api/tcpip.o ./mw/Third_Party/LwIP/src/api/tcpip.su

.PHONY: clean-mw-2f-Third_Party-2f-LwIP-2f-src-2f-api

