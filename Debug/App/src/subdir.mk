################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../App/src/client_task.cpp \
../App/src/led_task.cpp \
../App/src/server_task.cpp 

OBJS += \
./App/src/client_task.o \
./App/src/led_task.o \
./App/src/server_task.o 

CPP_DEPS += \
./App/src/client_task.d \
./App/src/led_task.d \
./App/src/server_task.d 


# Each subdirectory must supply rules for building sources it contributes
App/src/%.o: ../App/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C++ Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-g++" -MMD -MT "$@" -DXMC4500_F100x1024 -DXMC_ETH_PHY_KSZ8081RNB -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/FreeRTOS-SocketCAN/config" -I"$(PROJECT_LOC)/FreeRTOS-SocketCAN/include" -I"$(PROJECT_LOC)/Driver/CAN/cfg/inc" -I"$(PROJECT_LOC)/Driver/CAN/inc" -I"$(PROJECT_LOC)/App/inc" -I"$(PROJECT_LOC)/FreeRTOS-Plus-TCP/config" -I"$(PROJECT_LOC)/Driver/GPIO/inc" -I"$(PROJECT_LOC)/Driver/inc" -I"$(PROJECT_LOC)/Driver/MEM/inc" -I"$(PROJECT_LOC)/Driver/RTC/cfg/inc" -I"$(PROJECT_LOC)/Driver/RTC/inc" -I"$(PROJECT_LOC)/Driver/SCU/cfg/inc" -I"$(PROJECT_LOC)/Driver/SCU/inc" -I"$(PROJECT_LOC)/Driver/UART/cfg/inc" -I"$(PROJECT_LOC)/Driver/UART/inc" -I"$(PROJECT_LOC)/FreeRTOS-Plus-TCP/include" -I"$(PROJECT_LOC)/FreeRTOS-Plus-TCP/portable/Compiler/GCC" -I"$(PROJECT_LOC)/FreeRTOS-Plus-TCP/portable/NetworkInterface/include" -I"$(PROJECT_LOC)/CMSIS_RTOS_V2" -I"$(PROJECT_LOC)/FreeRTOS/config" -I"$(PROJECT_LOC)/FreeRTOS/Source/include" -I"$(PROJECT_LOC)/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4500_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Libraries" -O1 -ffunction-sections -fdata-sections -Wall -Weffc++ -std=gnu++14 -mfloat-abi=hard -ffast-math -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -funsigned-bitfields -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -Wunused -Wmissing-declarations -Wpointer-arith -Wfloat-equal -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g3 -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

