################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Libraries/XMCLib/src/subdir.mk
-include Libraries/Newlib/subdir.mk
-include FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include FreeRTOS/Source/portable/Common/subdir.mk
-include FreeRTOS/Source/subdir.mk
-include FreeRTOS/MemMang/subdir.mk
-include FreeRTOS-Plus-TCP/tools/subdir.mk
-include FreeRTOS-Plus-TCP/portable/NetworkInterface/board_family/subdir.mk
-include FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/subdir.mk
-include FreeRTOS-Plus-TCP/portable/BufferManagement/subdir.mk
-include FreeRTOS-Plus-TCP/subdir.mk
-include Driver/UART/src/subdir.mk
-include Driver/UART/cfg/src/subdir.mk
-include Driver/SCU/src/subdir.mk
-include Driver/SCU/cfg/src/subdir.mk
-include Driver/RTC/src/subdir.mk
-include Driver/RTC/cfg/src/subdir.mk
-include Driver/MEM/src/subdir.mk
-include Driver/GPIO/src/subdir.mk
-include Driver/CAN/src/subdir.mk
-include Driver/CAN/cfg/src/subdir.mk
-include CMSIS_RTOS_V2/subdir.mk
-include App/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Ethernet_CAN_XMC4500.hex \

SECONDARY_LIST += \
Ethernet_CAN_XMC4500.lst \

SECONDARY_SIZE += \
Ethernet_CAN_XMC4500.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = "Ethernet_CAN_XMC4500.map"

TOOLCHAIN_ROOT=E:/Embedded/DAVE_4-4-2_64bit_2018-02-23/DAVE-IDE-4.4.2-64Bit/eclipse/ARM-GCC-49
PROJECT_LOC=E:/Embedded/Projects/XMC4500/Ethernet_CAN_XMC4500

# All Target
all: Ethernet_CAN_XMC4500.elf secondary-outputs

# Tool invocations
Ethernet_CAN_XMC4500.elf: $(OBJS) E:\Embedded\Projects\XMC4500\Ethernet_CAN_XMC4500\linker\FreeRTOS_XMC4500.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C++ Linker'
	$(file >objects.rsp,$(OBJS))
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-g++" -T"E:\Embedded\Projects\XMC4500\Ethernet_CAN_XMC4500\linker\FreeRTOS_XMC4500.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,$(MAP_FILE) -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "Ethernet_CAN_XMC4500.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
Ethernet_CAN_XMC4500.hex: Ethernet_CAN_XMC4500.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy" -O ihex "Ethernet_CAN_XMC4500.elf" "Ethernet_CAN_XMC4500.hex"
	@echo 'Finished building: $@'
	@echo.
Ethernet_CAN_XMC4500.lst: Ethernet_CAN_XMC4500.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objdump" -h -S "Ethernet_CAN_XMC4500.elf" > "Ethernet_CAN_XMC4500.lst"
	@echo 'Finished building: $@'
	@echo.
Ethernet_CAN_XMC4500.siz: Ethernet_CAN_XMC4500.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size" --format=berkeley "Ethernet_CAN_XMC4500.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(C_DEPS)$(CC_DEPS)$(C++_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_UPPER_DEPS)$(CPP_DEPS)
	-$(RM) App/src/client_task.o App/src/led_task.o App/src/server_task.o CMSIS_RTOS_V2/cmsis_os2.o Driver/CAN/cfg/src/dri_can_conf.o Driver/CAN/src/dri_can.o Driver/GPIO/src/dri_gpio.o Driver/GPIO/src/dri_gpio_pins.o Driver/MEM/src/dri_mem.o Driver/RTC/cfg/src/dri_rtc_conf.o Driver/RTC/src/dri_rtc.o Driver/SCU/cfg/src/dri_scu_conf.o Driver/SCU/src/dri_scu.o Driver/UART/cfg/src/dri_uart_conf.o Driver/UART/src/dri_uart.o FreeRTOS/MemMang/heap_4.o FreeRTOS/Source/croutine.o FreeRTOS/Source/event_groups.o FreeRTOS/Source/list.o FreeRTOS/Source/queue.o FreeRTOS/Source/stream_buffer.o FreeRTOS/Source/tasks.o FreeRTOS/Source/timers.o FreeRTOS/Source/portable/Common/mpu_wrappers.o FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o FreeRTOS-Plus-TCP/FreeRTOS_ARP.o FreeRTOS-Plus-TCP/FreeRTOS_DHCP.o FreeRTOS-Plus-TCP/FreeRTOS_DNS.o FreeRTOS-Plus-TCP/FreeRTOS_IP.o FreeRTOS-Plus-TCP/FreeRTOS_RAW_IP.o FreeRTOS-Plus-TCP/FreeRTOS_Sockets.o FreeRTOS-Plus-TCP/FreeRTOS_Stream_Buffer.o FreeRTOS-Plus-TCP/FreeRTOS_TCP_IP.o FreeRTOS-Plus-TCP/FreeRTOS_TCP_WIN.o FreeRTOS-Plus-TCP/FreeRTOS_UDP_IP.o FreeRTOS-Plus-TCP/portable/BufferManagement/BufferAllocation_2.o FreeRTOS-Plus-TCP/portable/NetworkInterface/Common/phyHandling.o FreeRTOS-Plus-TCP/portable/NetworkInterface/board_family/NetworkInterface.o FreeRTOS-Plus-TCP/tools/tcp_dump_packets.o FreeRTOS-Plus-TCP/tools/tcp_mem_stats.o Libraries/Newlib/syscalls.o Libraries/XMCLib/src/xmc4_eru.o Libraries/XMCLib/src/xmc4_flash.o Libraries/XMCLib/src/xmc4_gpio.o Libraries/XMCLib/src/xmc4_rtc.o Libraries/XMCLib/src/xmc4_scu.o Libraries/XMCLib/src/xmc_can.o Libraries/XMCLib/src/xmc_ccu4.o Libraries/XMCLib/src/xmc_ccu8.o Libraries/XMCLib/src/xmc_common.o Libraries/XMCLib/src/xmc_dac.o Libraries/XMCLib/src/xmc_dma.o Libraries/XMCLib/src/xmc_dsd.o Libraries/XMCLib/src/xmc_ebu.o Libraries/XMCLib/src/xmc_ecat.o Libraries/XMCLib/src/xmc_eru.o Libraries/XMCLib/src/xmc_eth_mac.o 
	-$(RM) Libraries/XMCLib/src/xmc_eth_phy_dp83848.o Libraries/XMCLib/src/xmc_eth_phy_ksz8031rnl.o Libraries/XMCLib/src/xmc_eth_phy_ksz8081rnb.o Libraries/XMCLib/src/xmc_fce.o Libraries/XMCLib/src/xmc_gpio.o Libraries/XMCLib/src/xmc_hrpwm.o Libraries/XMCLib/src/xmc_i2c.o Libraries/XMCLib/src/xmc_i2s.o Libraries/XMCLib/src/xmc_ledts.o Libraries/XMCLib/src/xmc_posif.o Libraries/XMCLib/src/xmc_rtc.o Libraries/XMCLib/src/xmc_sdmmc.o Libraries/XMCLib/src/xmc_spi.o Libraries/XMCLib/src/xmc_uart.o Libraries/XMCLib/src/xmc_usbd.o Libraries/XMCLib/src/xmc_usbh.o Libraries/XMCLib/src/xmc_usic.o Libraries/XMCLib/src/xmc_vadc.o Libraries/XMCLib/src/xmc_wdt.o Startup/_boot_init.o Startup/_startup.o Startup/startup_XMC4500.o Startup/system_XMC4500.o interrupts.o main.o peripherals_hooks.o rtos_hooks.o  Ethernet_CAN_XMC4500.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
