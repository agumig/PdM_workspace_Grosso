################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f429xx.s 

OBJS += \
./Startup/startup_stm32f429xx.o 

S_DEPS += \
./Startup/startup_stm32f429xx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/CMSIS/Include" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/Core/Inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/API" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/API/inc" -I"/home/agustin/CESE/PdM_workspace_Grosso/Practica3/Drivers/API/src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f429xx.d ./Startup/startup_stm32f429xx.o

.PHONY: clean-Startup

