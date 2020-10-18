/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		Client Task Source
* Filename:		client_task.cpp
* Author:		HS
* Origin Date:	01/18/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	client_task.cpp
 *  @brief:	This source file contains function definitions for
 *  		client task class.
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <client_task.h>

/* Shall not be included directly into application */
#include <FreeRTOS_IP_Private.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

//#define USING_RAW_UDP_CLIENT
#define USING_RAW_CLIENT

/* Note: I am not using both for testing, you can update the code and do it */
#if defined(USING_RAW_UDP_CLIENT) && defined(USING_RAW_CLIENT)
#error "Cannot use both at same time"
#endif


/******************************************************************************
* Configuration Constants
*******************************************************************************/

#define CLIENT_DATA_LENGTH				( 14 )


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Definitions
*******************************************************************************/


/******* Public Member function for ClientTask__ class *******/

/**
 * @function
 *
 * @brief
 *
 * <i>Imp Note:</i>
 */
void ClientTask__ :: client_body( void *pvParameters )
{
    (void) pvParameters;
	/* Local variables */
	struct freertos_sockaddr xServerAddress;
	Socket_t xClientSocket;
	constexpr TickType_t xSendTimeOut = pdMS_TO_TICKS( 5000 );
    TickType_t xTimeOnShutdown = 0;

#ifdef USING_RAW_CLIENT
    unsigned long data_len = sizeof(struct xIP_PACKET) + CLIENT_DATA_LENGTH;
    char buffer_l[sizeof(struct xIP_PACKET) + CLIENT_DATA_LENGTH] = {};
#elif defined(USING_RAW_UDP_CLIENT)
    unsigned long data_len = sizeof(struct xUDP_PACKET) + CLIENT_DATA_LENGTH;
    char buffer_l[sizeof(struct xUDP_PACKET) + CLIENT_DATA_LENGTH] = {};
#endif

	/* Configure server details */
#ifdef USING_RAW_CLIENT
	xServerAddress.sin_port = FreeRTOS_htons( 0/*5901*/ );
#elif defined(USING_RAW_UDP_CLIENT)
	xServerAddress.sin_port = FreeRTOS_htons( 5901 );
#endif
	xServerAddress.sin_addr = FreeRTOS_inet_addr( "192.168.1.7" );/* For RAW it will become 0.0.0.0, let this be same */

	/* Fill in the packet information
	 * Note: You can fill packet information in any way as you like,
	 * I have filled information using the existing method provided in FreeRTOS+Plus, and quite handy too! */
	char *pxIpSrcAddrOffset = ( char *) ( & (buffer_l[sizeof( MACAddress_t )]) );
#ifdef USING_RAW_CLIENT
	( void ) memcpy( ( void * ) pxIpSrcAddrOffset, ( const void * ) ( xDefaultPartRAWPacketHeader.ucBytes ), sizeof( xDefaultPartRAWPacketHeader ) );
	pxIpSrcAddrOffset = ( char *) ( & (buffer_l[sizeof(xIP_PACKET)]) );
#elif defined(USING_RAW_UDP_CLIENT)
	( void ) memcpy( ( void * ) pxIpSrcAddrOffset, ( const void * ) ( xDefaultPartUDPPacketHeader.ucBytes ), sizeof( xDefaultPartUDPPacketHeader ) );
	pxIpSrcAddrOffset = ( char *) ( & (buffer_l[sizeof(xUDP_PACKET)]) );
#endif
	( void ) memcpy( ( void * ) pxIpSrcAddrOffset, (char *)"Hella Server\r\n", CLIENT_DATA_LENGTH );

	/* Connect to a server and create a task for client */
	for(  ; ; )
	{
		/* Attempt to open the socket. */
#ifdef USING_RAW_CLIENT
		xClientSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_RAW, FREERTOS_IPPROTO_RAW );
#elif defined(USING_RAW_UDP_CLIENT)
		xClientSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_RAW, FREERTOS_IPPROTO_UDP );
#endif
		configASSERT( xClientSocket != FREERTOS_INVALID_SOCKET );

		FreeRTOS_setsockopt( xClientSocket, 0, FREERTOS_SO_SNDTIMEO, &xSendTimeOut, sizeof( xSendTimeOut ) );

		/* Start processing */
		for( ; ; )
		{
            if( 0 == FreeRTOS_sendto( xClientSocket, buffer_l, data_len, FREERTOS_MSG_DONTWAIT, &xServerAddress, sizeof(xServerAddress) ) )
            {
                /* Break loop to recreate the client socket */
                break;
            }
            else
            {
                vTaskDelay( pdMS_TO_TICKS(1000) );
            }
        }

        /* Initiate a shutdown in case it has not already been initiated. */
        FreeRTOS_shutdown( xClientSocket, FREERTOS_SHUT_RDWR );

        /* Wait for the shutdown to take effect, indicated by FreeRTOS_recv()
         * returning an error. */
        xTimeOnShutdown = xTaskGetTickCount();
        do
        {
            if( FreeRTOS_recv( xClientSocket, buffer_l, sizeof(buffer_l), 0 ) < 0 )
            {
                break;
            }
        } while( ( xTaskGetTickCount() - xTimeOnShutdown ) < pdMS_TO_TICKS(1000) );

        /* Close socket */
        FreeRTOS_closesocket( xClientSocket );
	}

	vTaskDelete( NULL );
}


/********************************** End of File *******************************/
