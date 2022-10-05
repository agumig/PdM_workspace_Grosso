################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/hw_hx711.c \
../Src/hw_hx711_port.c \
../Src/hw_timer_us.c \
../Src/hw_timer_us_port.c \
../Src/main.c 

OBJS += \
./Src/hw_hx711.o \
./Src/hw_hx711_port.o \
./Src/hw_timer_us.o \
./Src/hw_timer_us_port.o \
./Src/main.o 

C_DEPS += \
./Src/hw_hx711.d \
./Src/hw_hx711_port.d \
./Src/hw_timer_us.d \
./Src/hw_timer_us_port.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/CMSIS/Include" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/Core/Inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/API" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/API/inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Drivers/API/src" -I"/home/agustin/CESE/PdM_workspace_Grosso/TP PdM - PCSE/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/hw_hx711.d ./Src/hw_hx711.o ./Src/hw_hx711.su ./Src/hw_hx711_port.d ./Src/hw_hx711_port.o ./Src/hw_hx711_port.su ./Src/hw_timer_us.d ./Src/hw_timer_us.o ./Src/hw_timer_us.su ./Src/hw_timer_us_port.d ./Src/hw_timer_us_port.o ./Src/hw_timer_us_port.su ./Src/main.d ./Src/main.o ./Src/main.su

.PHONY: clean-Src
