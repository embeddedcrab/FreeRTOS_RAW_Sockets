/*********************************************************************************************************************
 * @file	server_task.cpp
 * @brief	Server Task Source
 * @version
 * @date	01/18/2020
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
#include <server_task.h>
#include <client_task.h>

#include <FreeRTOS_IP_Private.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

#define USING_RAW_UDP_SERVER


/******************************************************************************
* Configuration Constants
*******************************************************************************/

#define MAX_CLIENTS		2


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/

/* Declare static data member of class
 *
 * Note: Tell linker to take variable from this source file
 * */


static const char * const gserver_handler_name = "ServerHandler";
static const UBaseType_t gserver_handler_priority = ipconfigIP_TASK_PRIORITY + 1;
static const configSTACK_DEPTH_TYPE gserver_handler_stack_depth = configMINIMAL_STACK_SIZE * 2;


/******************************************************************************
* Function Declarations
*******************************************************************************/

extern "C"
{
#ifdef USING_RAW_UDP_SERVER

#define configECHO_SERVER_ADDR0		192
#define configECHO_SERVER_ADDR1		168
#define configECHO_SERVER_ADDR2		1
#define configECHO_SERVER_ADDR3		7

#define RAW_UDP_SERVER_DATA_LENGTH	20

const char * rus_task_name = "RAW UDP Server";
void raw_udp_server( void * pvParameters );

#endif
}


/******************************************************************************
* Function Definitions
*******************************************************************************/


/******* Public Member function for ServerTask__ class *******/

/**
 * @function
 *
 * @brief
 *
 * <i>Imp Note:</i>
 * 				Example taken from FreeRTOS-Plus TCP demo
 */
void ServerTask__ :: server_body( void *pvParameters )
{
	/* Local variables */
	struct freertos_sockaddr xClient, xBindAddress;
	Socket_t xListeningSocket = NULL;
	Socket_t xConnectedSocket = NULL;

	socklen_t xSize = sizeof( xClient );
	TickType_t xReceiveTimeOut = portMAX_DELAY;
	const BaseType_t xBacklog = MAX_CLIENTS;

	while( FreeRTOS_IsNetworkUp() != pdTRUE )
	{
		/* Sleep, then check again */
		vTaskDelay( NETWORK_RETRY_DELAY );
	}

	/* Create Test Client Task */
	ClientTask__ client( ipconfigIP_TASK_PRIORITY - 1, configMINIMAL_STACK_SIZE<<2 );

	#ifdef USING_RAW_UDP_SERVER
	/* Create another server task */
	(void) RTOS_WRAPPER__::create( raw_udp_server, rus_task_name,
									configMINIMAL_STACK_SIZE<<1, NULL,
									ipconfigIP_TASK_PRIORITY - 1, NULL );
	#endif

	/* Attempt to open the socket. */
	xListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );
	configASSERT( xListeningSocket != FREERTOS_INVALID_SOCKET );

	/* Set a time out so accept() will just wait for a connection. */
	FreeRTOS_setsockopt( xListeningSocket, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof( xReceiveTimeOut ) );

	/* Bind the socket to the port that the client task will send to,
	 * then listen for incoming connections. */
	xBindAddress.sin_port = INFINEON_HOST_SERVER_PORT;
	xBindAddress.sin_port = FreeRTOS_htons( xBindAddress.sin_port );
	FreeRTOS_bind( xListeningSocket, &xBindAddress, sizeof( xBindAddress ) );
	FreeRTOS_listen( xListeningSocket, xBacklog );

	/* Create task to receive data from client */
	for( ; ; )
	{
		/* Wait for a client to connect. */
		xConnectedSocket = FreeRTOS_accept( xListeningSocket, &xClient, &xSize );
		configASSERT( xConnectedSocket != FREERTOS_INVALID_SOCKET );

		(void) RTOS_WRAPPER__::create( ServerTask__::server_handler_entry, gserver_handler_name, gserver_handler_stack_depth,
										xConnectedSocket, gserver_handler_priority, NULL );
	}

	/* Should never reach here */
	vTaskDelete( NULL );
}


/**
 * @function
 *
 * @brief		Simple Echo server
 *
 * <i>Imp Note:</i>
 */
void ServerTask__ :: server_handler_body( void * pvParameters )
{
	/* Local variables */
	Socket_t xConnectedSocket = NULL;
	uint8_t *pucRxBuffer = NULL;
	BaseType_t lBytes = 0;
	BaseType_t lSent = 0;
	BaseType_t lTotalSent = 0;
	constexpr TickType_t xReceiveTimeOut = pdMS_TO_TICKS( portMAX_DELAY );
	constexpr TickType_t xSendTimeOut = pdMS_TO_TICKS( 5000 );
	TickType_t xTimeOnShutdown = 0;

	/* Get the client details */
	xConnectedSocket = (Socket_t) pvParameters;

	/* Send Accept confirmation to client */
	FreeRTOS_send( xConnectedSocket, "Hello\r\n", sizeof("Hello\r\n"), 0 );

	/* Attempt to create the buffer used to receive the string to be echoed
	 * back.  This could be avoided using a zero copy interface that just
	 * returned the same buffer. */
	pucRxBuffer = new uint8_t[ipconfigTCP_MSS];
	/* Check memory allocation for buffer validity */
	if( pucRxBuffer != NULL )
	{
		/* Set socket options for client */
		FreeRTOS_setsockopt( xConnectedSocket, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof( xReceiveTimeOut ) );
		FreeRTOS_setsockopt( xConnectedSocket, 0, FREERTOS_SO_SNDTIMEO, &xSendTimeOut, sizeof( xReceiveTimeOut ) );

		/* Receive data from client */
		for( ; ; )
		{
			/* Zero out the receive array so there is NULL at the end of the string
				when it is printed out. */
			memset( pucRxBuffer, 0x00, ipconfigTCP_MSS );

			/* Receive data on the socket. */
			lBytes = FreeRTOS_recv( xConnectedSocket, pucRxBuffer, ipconfigTCP_MSS, 0 );

			/* If data was received, echo it back. */
			if( lBytes >= 0 )
			{
				lSent = 0;
				lTotalSent = 0;

				/* Call send() until all the data has been sent. */
				while( ( lSent >= 0 ) && ( lTotalSent < lBytes ) )
				{
					lSent = FreeRTOS_send( xConnectedSocket, pucRxBuffer, lBytes - lTotalSent, 0 );
					lTotalSent += lSent;
				}

				if( lSent < 0 )
				{
					/* Socket closed? */
					break;
				}
			}
			else
			{
				/* Socket closed? */
				break;
			}
		}
	}

	/* Initiate a shutdown in case it has not already been initiated. */
	FreeRTOS_shutdown( xConnectedSocket, FREERTOS_SHUT_RDWR );

	/* Wait for the shutdown to take effect, indicated by FreeRTOS_recv()
	 * returning an error. */
	xTimeOnShutdown = xTaskGetTickCount();
	do
	{
		if( FreeRTOS_recv( xConnectedSocket, pucRxBuffer, ipconfigTCP_MSS, 0 ) < 0 )
		{
			break;
		}
	} while( ( xTaskGetTickCount() - xTimeOnShutdown ) < tcpechoSHUTDOWN_DELAY );

	/* Finished with the socket, buffer, the task. */
	delete[] pucRxBuffer;
	FreeRTOS_closesocket( xConnectedSocket );

	vTaskDelete( NULL );
}
/*---------------------------------------------------------------------------*/


#ifdef USING_RAW_UDP_SERVER
/**
 * @function
 *
 * @brief		Simple UDP Server
 *
 * <i>Imp Note:</i>
 */
void raw_udp_server( void * pvParameters )
{
	/* Local variables */
	Socket_t xListeningSocket = NULL;
	struct freertos_sockaddr xClient, xServer;
	socklen_t xAddressLength;

	unsigned long data_len = sizeof(struct xUDP_PACKET) + RAW_UDP_SERVER_DATA_LENGTH;
	char data[sizeof(struct xUDP_PACKET) + RAW_UDP_SERVER_DATA_LENGTH] = {};

	/* Attempt to open the socket. */
	xListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_RAW, FREERTOS_IPPROTO_UDP );
	configASSERT( xListeningSocket != FREERTOS_INVALID_SOCKET );

	/* Bind the socket to the port that the client task will send to,
	 * then listen for incoming connections. */
	xServer.sin_port = 6000;
	xServer.sin_port = FreeRTOS_htons( xServer.sin_port );
	if( -1 == FreeRTOS_bind( xListeningSocket, &xServer, sizeof( xServer ) ) )
	{
		FreeRTOS_closesocket( xListeningSocket );
		/* For debugging only */
		for( ; ; );
	}

	/* Create task to receive data from client */
	for( ; ; )
	{
		/* Receive data from Client */
		FreeRTOS_recvfrom(	xListeningSocket, (data + sizeof(struct xUDP_PACKET)), RAW_UDP_SERVER_DATA_LENGTH, 0, &xClient, &xAddressLength );

		/* Update UDP data */
		char *pxIpSrcAddrOffset = ( char *) ( & (data[sizeof( MACAddress_t )]) );
		( void ) memcpy( ( void * ) pxIpSrcAddrOffset, ( const void * ) ( xDefaultPartUDPPacketHeader.ucBytes ), sizeof( xDefaultPartUDPPacketHeader ) );

		FreeRTOS_sendto( xListeningSocket, data, data_len, 0, &xClient, xAddressLength );
		vTaskDelay(500);
	}

	FreeRTOS_closesocket( xListeningSocket );

	/* Should never reach here */
	vTaskDelete( NULL );
}

#endif

/********************************** End of File *******************************/
