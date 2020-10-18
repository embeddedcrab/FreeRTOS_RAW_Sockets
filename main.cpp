/*********************************************************************************************************************
 * @file     main.cpp
 * @brief    Main source file
 * @version
 * @date
 *
 * @cond
 *********************************************************************************************************************
 * Copyright (c) 2020, Hemant Sharma - All Rights Reserved
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************
 *
 * *************************** Change history ********************************
 *
 * @endcond
 */


/******************************************************************************
* Includes
*******************************************************************************/

/* Include peripheral device files */
#include <dri_gpio_pins.h>
#include <dri_uart_extern.h>
#include <dri_can_extern.h>
#include <dri_scu_extern.h>
#include <dri_rtc_extern.h>

/* Include memory Headers */
#include <dri_mem.h>

/* Include RTOS Headers */
#include <FreeRTOS.h>
#include <semphr.h>

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "NetworkInterface.h"

/* Include Application Headers */
#include <led_task.h>
#include <server_task.h>
#include <client_task.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/

/**
 * Extern "C" block for C variables
 * C Linkage in CPP Application
 */
#ifdef __cplusplus
extern "C"
{
/* Nothing present */
}
#endif	/* __cplusplus */



/******************************************************************************
* Functions
*******************************************************************************/

/**
 * Extern "C" block for C functions
 * C Linkage in CPP Application
 */
#ifdef __cplusplus
extern "C"
{
/******* User defined initialization functions *******/
void init_user_before_main( void );
}
#endif	/* __cplusplus */


/**
 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 */
int main(void)
{
	/* Task Parameters Variable */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wwrite-strings"
	TaskCreationParams TaskParams = {"LED_TASK", configMINIMAL_STACK_SIZE, 1 };
	#pragma GCC diagnostic pop

	/* The MAC address array is not declared const as the MAC address will
	 * normally be read from an some Memory(like Flash OTP region) and not hard coded
	 * (in real deployed applications) or read using HAL API from ETH MAC */
	static uint8_t ucMACAddress[ 6 ] = { 0x00U, 0x03U, 0x19U, 0x45U, 0x00, 0x00 };

	/* Define the network addressing.  These parameters will be used if either
	 * ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration failed. */
	static const uint8_t ucIPAddress[ 4 ] =
	{
		INFINEON_HOST_SERVER_IP_ADDRESS0, INFINEON_HOST_SERVER_IP_ADDRESS1,
		INFINEON_HOST_SERVER_IP_ADDRESS2, INFINEON_HOST_SERVER_IP_ADDRESS3
	};

	static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
	static const uint8_t ucGatewayAddress[ 4 ] = { 192, 168, 1, 1 };
	/* The following is the address of an OpenDNS server. */
	static const uint8_t ucDNSServerAddress[ 4 ] = { 208, 67, 222, 222 };

	/* Create LED tasks
	 *
	 * <i>Imp Note:</i>
	 * Application task creation shall be done using new while creating objects
	 * otherwise Bus Fault Exception will come!!
	 * */
	LedTask__ * LedTask = new LedTask__( XMC_GPIO_PORT1 , 0, &TaskParams );
	LedTask->xvinit();
	
	/* Initialize the RTOS’s TCP/IP stack.  The tasks that use the network
	 * are created in the vApplicationIPNetworkEventHook() hook function below.
	 * The hook function is called when the network connects. */
	BaseType_t xReturn = FreeRTOS_IPInit( ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress );

	/* Check IP Init status */
	if( xReturn == pdPASS )
	{
		/* Create Server Handler task */
		ServerTask__ server( ipconfigIP_TASK_PRIORITY, configMINIMAL_STACK_SIZE );
		/* Start Task Scheduler */
		vTaskStartScheduler();
		/* Processor should never cross from here */
		GPIO__::fp_ctrl( XMC_GPIO_PORT1 , 0, XMC_GPIO_OUTPUT_LEVEL_HIGH );
		for( ; ; )
		{
			__asm("NOP");
		}
	}
	else
	{
		GPIO__::fp_ctrl( XMC_GPIO_PORT1 , 0, XMC_GPIO_OUTPUT_LEVEL_HIGH );
		/* Could not initialize Network, shall hang N/W related tasks */
		for( ; ; )
		{
			__asm("NOP");
		}
	}

	return 0;
}


/******************************************************************************/

/**
 * @function
 *
 * @brief		User defined initialization before main entry
 *
 * @param[in]
 *
 * @param[out]	NA
 *
 * @return
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
extern "C" void init_user_before_main( void )
{
	extern void init_system_before_main( void );
	init_system_before_main();

	/* Initialize GPIO Pin functionalities */
	extern void gpio_init_c( void );
	gpio_init_c();

	/*----------------- Initialize Peripherals --------------------*/

	/* Initialize UART Channels */
	UART_init( &UART_Channel_0, 1 );	/* Used for Application Logging */

	/* Initialize CAN Module */
	CAN_module_init( &CAN_Global, 1 );
	/* Initialize CAN Nodes */
	CAN_node_init( &CAN1, 1, 1 );
	CAN_node_init( &CAN2, 2, 1 );

	/* Initialize Ethernet MAC */
	extern void initialize_ethernet_mac( void );	/* MAC Initialization fucntion */
	initialize_ethernet_mac();

	/* Initialize SCU and WDT */
	SCU_init( &SCU, 1 );
	/* Initialize RTC */
	RTC_init( &RTC_, 1 );
}

/********************************** End of File *******************************/
