/*
FreeRTOS+TCP V2.2.2
Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 http://aws.amazon.com/freertos
 http://www.FreeRTOS.org
*/


/*********************************************************************************************************************
 * @file     NetworkInterface.c
 * @brief    Network interface port source file for Infineon XMC4500 Relax Kit
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

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_DNS.h"
#include "NetworkBufferManagement.h"
#include "NetworkInterface.h"


/* Include XMC library headers */
#include "xmc_eth_mac.h"
#include "xmc_eth_phy.h"


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/

/* If ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES is set to 1, then the Ethernet
driver will filter incoming packets and only pass the stack those packets it
considers need processing. */
#if( ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES == 0 )
#define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer ) eProcessBuffer
#else
#define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer ) eConsiderFrameForProcessing( ( pucEthernetBuffer ) )
#endif


/* Default the size of the stack used by the EMAC deferred handler task to twice
the size of the stack used by the idle task - but allow this to be overridden in
FreeRTOSConfig.h as configMINIMAL_STACK_SIZE is a user definable constant. */
#ifndef configEMAC_TASK_STACK_SIZE
	#define configEMAC_TASK_STACK_SIZE ( 2 * configMINIMAL_STACK_SIZE )
#endif

#ifndef configEMAC_TASK_PRIORITY
	#define configEMAC_TASK_PRIORITY			( ipconfigIP_TASK_PRIORITY + 1)
#endif

/* Data configuration constants for ETH */
#define ETH_0_PHY_ADDR   (0)
#define ETH_0_NUM_RX_BUF (4U)
#define ETH_0_NUM_TX_BUF (4U)

/* MAC address*/
#define MAC_ADDR0 (0x00U)
#define MAC_ADDR1 (0x03U)
#define MAC_ADDR2 (0x19U)
#define MAC_ADDR3 (0x45U)
#define MAC_ADDR4 (0x00U)
#define MAC_ADDR5 (0x00U)


/******************************************************************************
* Macros
*******************************************************************************/

/* Macros for Ethernet Configuration */
#define MAC_ADDR    ((uint64_t)MAC_ADDR0 | \
        ((uint64_t)MAC_ADDR1 << 8) | \
        ((uint64_t)MAC_ADDR2 << 16) | \
        ((uint64_t)MAC_ADDR3 << 24) | \
        ((uint64_t)MAC_ADDR4 << 32) | \
        ((uint64_t)MAC_ADDR5 << 40))


/******************************************************************************
* Variables
*******************************************************************************/

/**
 * @brief	MAC buffers
 */
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_0_rx_desc[ETH_0_NUM_RX_BUF] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) XMC_ETH_MAC_DMA_DESC_t ETH_0_tx_desc[ETH_0_NUM_TX_BUF] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t ETH_0_rx_buf[ETH_0_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
static __attribute__((aligned(4))) uint8_t ETH_0_tx_buf[ETH_0_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));

/**
 * @brief	xEthRxEventSemaphore for Rx ISR Handling
 */
static SemaphoreHandle_t xEthRxEventSemaphore = NULL;
static SemaphoreHandle_t xEthTxEventMutex = NULL;
static SemaphoreHandle_t xEthRxEventMutex = NULL;

/**
 * @brief	Holds the handle of the task used as a deferred interrupt processor.
 * 			The handle is used so direct notifications can be sent to the task
 * 			for all EMAC/DMA related interrupts
 */
static TaskHandle_t xEMACTaskHandle = NULL;

/**
 * @brief	Ethernet Handle for ethernet mac packets processing and handling
 */
static XMC_ETH_MAC_t ethernet_mac_handle =
{
    .regs = ETH0,
    .address = MAC_ADDR,
    .rx_desc = ETH_0_rx_desc,
    .tx_desc = ETH_0_tx_desc,
    .rx_buf = ETH_0_rx_buf[0],
    .tx_buf = ETH_0_tx_buf[0],
    .num_rx_buf = ETH_0_NUM_RX_BUF,
    .num_tx_buf = ETH_0_NUM_TX_BUF
};


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
 * @brief	A deferred interrupt handler task that processes
 */
static void prvEMACHandlerTask( void *pvParameters );


/******************************************************************************
* Function Definitions
*******************************************************************************/


/**
 * @function
 *
 * @brief		Initializes the MAC driver task
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
BaseType_t xNetworkInterfaceInitialise( void )
{
    /* Local Variables */
    BaseType_t xResult = pdPASS;

    /* Check for PHY Link status */
    while( XMC_ETH_PHY_GetLinkStatus(&ethernet_mac_handle, ETH_0_PHY_ADDR) != XMC_ETH_LINK_STATUS_UP );

	/* Enter critical section */
	vPortEnterCritical();
	/* Set network speed and set it up */
	XMC_ETH_MAC_SetLink( &ethernet_mac_handle,
			XMC_ETH_PHY_GetLinkSpeed( &ethernet_mac_handle, ETH_0_PHY_ADDR ),
			XMC_ETH_PHY_GetLinkDuplex( &ethernet_mac_handle, ETH_0_PHY_ADDR ) );
	/* Enable Ethernet interrupts */
	XMC_ETH_MAC_EnableEvent( &ethernet_mac_handle, (uint32_t)XMC_ETH_MAC_EVENT_RECEIVE );
	XMC_ETH_MAC_EnableEvent( &ethernet_mac_handle, (uint32_t)XMC_ETH_MAC_EVENT_PMT );
	NVIC_SetPriority( (IRQn_Type)ETH0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 48U, 0U) );
	NVIC_ClearPendingIRQ( (IRQn_Type)ETH0_0_IRQn );
	NVIC_EnableIRQ( (IRQn_Type)ETH0_0_IRQn );

	/* Enable transmission and reception */
	XMC_ETH_MAC_EnableTx( &ethernet_mac_handle );
	XMC_ETH_MAC_EnableRx( &ethernet_mac_handle );
	/* Exit critical section */
	vPortExitCritical();

    /* Return status */
	return xResult;
}


/**
 * @function
 *
 * @brief		Sends data received from the embedded TCP/IP stack to the
 * 				Ethernet MAC driver for transmission
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxNetworkBuffer, BaseType_t xReleaseAfterSend )
{
    /* Local variables */
	NetworkBufferDescriptor_t * const pxDescriptor_l = pxNetworkBuffer;
	uint8_t * p_buffer_l = NULL;
    BaseType_t xReturn = pdFAIL;
    size_t frame_lenght_l = pxDescriptor_l->xDataLength;
    BaseType_t xMutex = pdFALSE;
    XMC_ETH_LINK_STATUS_t link_status_l = XMC_ETH_LINK_STATUS_DOWN;

	/* Open a do {} while ( 0 ) loop to be able to call break. */
	do
	{
        /* Check link status */
        link_status_l = XMC_ETH_PHY_GetLinkStatus( &ethernet_mac_handle, ETH_0_PHY_ADDR );
		if( XMC_ETH_LINK_STATUS_UP == link_status_l )
		{/* Process data to be sent to Ethernet */
			/* Check the data size to be sent and decide transmission state */
			if( (frame_lenght_l > (size_t) XMC_ETH_MAC_BUF_SIZE) ||
					(0 == frame_lenght_l) )
			{
				/* Can not proceed with the request because of larger data size than available frame size */
				xReturn = pdFAIL;
			}
			else
			{
				/* Take Mutex */
				xMutex = xSemaphoreTake( xEthTxEventMutex, portMAX_DELAY );
				if( pdTRUE == xMutex )
				{
					/* Check the data size and get the DMA descriptor to send data */
					if( pdTRUE == XMC_ETH_MAC_IsTxDescriptorOwnedByDma( &ethernet_mac_handle ) )
					{
						vReleaseNetworkBuffer( pxDescriptor_l->pucEthernetBuffer );
						XMC_ETH_MAC_ResumeTx( &ethernet_mac_handle );
					}
					else
					{
						/* Get the buffer for transmission */
						p_buffer_l = XMC_ETH_MAC_GetTxBuffer( &ethernet_mac_handle );
						XMC_ETH_MAC_SetTxBufferSize( &ethernet_mac_handle, frame_lenght_l );
						/* Copy Network layer data into buffer and trigger transmission */
						memcpy( p_buffer_l, pxDescriptor_l->pucEthernetBuffer, frame_lenght_l );
						XMC_ETH_MAC_ResumeTx( &ethernet_mac_handle );
						XMC_ETH_MAC_ReturnTxDescriptor( &ethernet_mac_handle );

						/* Call the standard trace macro to log the send event */
						iptraceNETWORK_INTERFACE_TRANSMIT();

						/* Update return status */
						xReturn = pdTRUE;
					}
				}
				else
				{
					/* Could not take Mutex */
					xReturn = pdFAIL;
				}
			}
		}
		else
		{
			/* The PHY has no Link Status, packet shall be dropped. */
			xReturn = pdFAIL;

			/* Call the user handler to handle the link status */
		}
	} while( 0 );

	/* Give mutex back */
    if( pdTRUE == xMutex )
    {
    	xSemaphoreGive( xEthTxEventMutex );
    }

	/* The buffer has been sent so can be released. */
	if( xReleaseAfterSend != pdFALSE )
	{
		pxDescriptor_l->pucEthernetBuffer = NULL;
		vReleaseNetworkBufferAndDescriptor( pxDescriptor_l );
	}

    /* Return status */
	return xReturn;
}


/* Uncomment this in case BufferAllocation_1.c is used. */
#ifdef BUFFER_ALLOCATION_1_USED

#define niBUFFER_1_PACKET_SIZE		1536

/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
void vNetworkInterfaceAllocateRAMToBuffers( NetworkBufferDescriptor_t pxNetworkBuffers[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ] )
{
	static __attribute__ ((section(".first_data"))) uint8_t ucNetworkPackets[ ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS * niBUFFER_1_PACKET_SIZE ] __attribute__ ( ( aligned( 32 ) ) );
	uint8_t *ucRAMBuffer = ucNetworkPackets;
	uint32_t ul;

		for( ul = 0; ul < ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS; ul++ )
		{
			pxNetworkBuffers[ ul ].pucEthernetBuffer = ucRAMBuffer + ipBUFFER_PADDING;
			*( ( unsigned * ) ucRAMBuffer ) = ( unsigned ) ( &( pxNetworkBuffers[ ul ] ) );
			ucRAMBuffer += niBUFFER_1_PACKET_SIZE;
		}
}

#endif	/* BUFFER_ALLOCATION_1_USED */



/**
 * @function
 *
 * @brief		Get PHY link status
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
BaseType_t xGetPhyLinkStatus( void )
{
	return (XMC_ETH_LINK_STATUS_UP == XMC_ETH_PHY_GetLinkStatus( &ethernet_mac_handle, ETH_0_PHY_ADDR ));
}


/**
 * @function
 *
 * @brief		Sends data received from Ethernet MAC driver to the
 * 				embedded TCP/IP stack to the for reception
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
void prvEMACHandlerTask( void *pvParameters )
{
    /* Remove compiler warnings about unused parameters. */
	( void ) pvParameters;

    /* Local variables */
    NetworkBufferDescriptor_t *pxNetworkBufferDescriptor = NULL;
    uint8_t * p_buffer_l = NULL;
    /* Used to indicate that xSendEventStructToIPTask() is being called because
     * of an Ethernet receive event. */
    IPStackEvent_t xRxEvent = { eNetworkRxEvent, NULL };
    BaseType_t xMutex = pdFALSE;
    size_t length_l = 0;

    /* Loop processing for task */
	for( ; ; )
	{
		/* Wait for the EMAC interrupt to indicate that another packet has been received. */
        if( xSemaphoreTake( xEthRxEventSemaphore, portMAX_DELAY ) == pdPASS )
        {
        	/* Disable Ethernet interrupt */
        	NVIC_DisableIRQ( (IRQn_Type)ETH0_0_IRQn );

        	/* Take Rx mutex */
        	xMutex = xSemaphoreTake( xEthRxEventMutex, portMAX_DELAY );
        	if( pdTRUE == xMutex )
        	{
            	/* Check descriptor */
            	if( XMC_ETH_MAC_IsRxDescriptorOwnedByDma( &ethernet_mac_handle ) )
            	{
            		XMC_ETH_MAC_ResumeRx( &ethernet_mac_handle );
            	}
            	else/* Descriptor available, Check for data size received */
            	{
            		/* Check data using HAL */
            		length_l = XMC_ETH_MAC_GetRxFrameSize( &ethernet_mac_handle );
                    /* If data length is valid, enter to get a network descriptor */
                    if( length_l > 0 )
                    {
                        /* Obtain a network buffer to pass this data into the
                           stack.  No storage is required as the network buffer
                           will point directly to the buffer that already holds
                           the received data. */
                        pxNetworkBufferDescriptor = pxGetNetworkBufferWithDescriptor( length_l, ( TickType_t ) 0 );
                        /* Check Network Descriptor validity */
                        if( pxNetworkBufferDescriptor != NULL )
                        {
                            /* Read descriptor buffer and assign to pcBuffer */
                        	p_buffer_l = XMC_ETH_MAC_GetRxBuffer( &ethernet_mac_handle );
                        	/* Copy the descriptor buffer data into network buffer */
                            memcpy( pxNetworkBufferDescriptor->pucEthernetBuffer, p_buffer_l, length_l );
                            pxNetworkBufferDescriptor->xDataLength = length_l;
                            /* Return RX descriptor */
                            XMC_ETH_MAC_ReturnRxDescriptor( &ethernet_mac_handle );
                            /* Resume Rx operation */
                            XMC_ETH_MAC_ResumeRx( &ethernet_mac_handle );

                            /* Can check for Receive data using Zero copy */
                            xRxEvent.pvData = ( void * ) pxNetworkBufferDescriptor;

                            /* Data was received and stored.  Send a message to the IP
                               task to let it know. */
                            if( xSendEventStructToIPTask( &xRxEvent, ( TickType_t ) 0 ) == pdFAIL )
                            {
                                vReleaseNetworkBufferAndDescriptor( pxNetworkBufferDescriptor );
                                iptraceETHERNET_RX_EVENT_LOST();
                            }
                        }/* if Network Buffer Descriptor != NULL */
                        else
                        {
                            iptraceETHERNET_RX_EVENT_LOST();
                        }
                    }/* if data length valid */
            	}
        	}
        	else
        	{
        		/* Could not take Mutex */
        	}
        }/* End of semaphore if block */

        /* Give mutex back */
        if( pdTRUE == xMutex )
        {
        	xSemaphoreGive( xEthRxEventMutex );
        }

        /* Enable Ethernet Interrupt */
        NVIC_ClearPendingIRQ( (IRQn_Type)ETH0_0_IRQn );
        NVIC_EnableIRQ( (IRQn_Type)ETH0_0_IRQn );
	}
}


/**
 * @function
 *
 * @brief		Initializes the MAC driver w.r.t Hardware
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
BaseType_t initialize_ethernet_mac( void )
{
    /* Local Variables */
    BaseType_t xResult = pdFAIL;
    /** Local Variables **/
	unsigned char count_outer_l = 0;

    /* Ethernet PHY configuration */
    const XMC_ETH_PHY_CONFIG_t eth_phy_config_l =
    {
        .interface = XMC_ETH_LINK_INTERFACE_RMII,
        .speed = XMC_ETH_LINK_SPEED_100M,
        .duplex = XMC_ETH_LINK_DUPLEX_FULL,
		.enable_auto_negotiate = 1,
		.enable_loop_back = 0
    };
    /* Ethernet port protocol configuration */
    const XMC_ETH_MAC_PORT_CTRL_t port_control_l =
    {
        .mode = XMC_ETH_MAC_PORT_CTRL_MODE_RMII,
        .rxd0 = XMC_ETH_MAC_PORT_CTRL_RXD0_P2_2,
        .rxd1 = XMC_ETH_MAC_PORT_CTRL_RXD1_P2_3,
        .clk_rmii = XMC_ETH_MAC_PORT_CTRL_CLK_RMII_P15_8,
        .crs_dv = XMC_ETH_MAC_PORT_CTRL_CRS_DV_P15_9,
        .rxer= XMC_ETH_MAC_PORT_CTRL_RXER_P2_4,
		.mdio = XMC_ETH_MAC_PORT_CTRL_MDIO_P2_0
    };

    /* Set port control for EMAC */
    XMC_ETH_MAC_SetPortControl( &ethernet_mac_handle, port_control_l );

    XMC_ETH_MAC_Init( &ethernet_mac_handle );
    XMC_ETH_MAC_DisableJumboFrame( &ethernet_mac_handle );
    XMC_ETH_MAC_EnableReceptionBroadcastFrames( &ethernet_mac_handle );

    /* Initialize PHY */
    if( XMC_ETH_PHY_STATUS_OK == XMC_ETH_PHY_Init( &ethernet_mac_handle, ETH_0_PHY_ADDR, &eth_phy_config_l ) )
    {
        /* Initialize Network descriptor buffers */
        for( count_outer_l = 0; count_outer_l < ETH_0_NUM_TX_BUF; ++count_outer_l )
        {
        	memset( (uint8_t *) (ETH_0_tx_buf + count_outer_l), '\0', XMC_ETH_MAC_BUF_SIZE );
        }
        for( count_outer_l = 0; count_outer_l < ETH_0_NUM_RX_BUF; ++count_outer_l )
        {
        	memset( (uint8_t *) (ETH_0_rx_buf + count_outer_l), '\0', XMC_ETH_MAC_BUF_SIZE );
        }
        while( XMC_ETH_PHY_GetLinkStatus(&ethernet_mac_handle, ETH_0_PHY_ADDR) != XMC_ETH_LINK_STATUS_UP );
    	/* PHY successfully initialized */

        /* Create Semaphore for EMAC Handler task */
        vSemaphoreCreateBinary( xEthRxEventSemaphore );
        xEthTxEventMutex = xSemaphoreCreateMutex();
        xEthRxEventMutex = xSemaphoreCreateMutex();
        configASSERT( xEthRxEventSemaphore );
        configASSERT( xEthTxEventMutex );
        configASSERT( xEthRxEventMutex );

        /* Create EMAC handler Task to process received frames */
        xResult = xTaskCreate( prvEMACHandlerTask, "EMAC Task", configEMAC_TASK_STACK_SIZE,
        		NULL, configEMAC_TASK_PRIORITY, &xEMACTaskHandle );
        /* Check return status */
        if( xResult != pdTRUE )
        {
        	vSemaphoreDelete( xEthRxEventSemaphore );
        	vSemaphoreDelete( xEthTxEventMutex );
			vSemaphoreDelete( xEthRxEventMutex );
        	/* Free Resources */
        	vTaskDelete( NULL );
        }
    }

    return xResult;
}


#if (ipconfigUSE_NETWORK_EVENT_HOOK == 1 )
/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 *
 */
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
	switch( eNetworkEvent )
	{
	default:
		case eNetworkUp:
		break;

		case eNetworkDown:
			/* Get link status, TODO: Note: Time Consuming for hooks */
			if( 1 == xGetPhyLinkStatus() )
			{
    			/* Set network speed and set it up */
    			XMC_ETH_MAC_SetLink( &ethernet_mac_handle,
    					XMC_ETH_PHY_GetLinkSpeed( &ethernet_mac_handle, ETH_0_PHY_ADDR ),
    					XMC_ETH_PHY_GetLinkDuplex( &ethernet_mac_handle, ETH_0_PHY_ADDR ) );
			}
		break;
	}
}

#endif	/* ipconfigUSE_NETWORK_EVENT_HOOK == 1 */


#if( ipconfigUSE_LLMNR == 1 )
BaseType_t xApplicationDNSQueryHook( const char *pcName )
{

}
#endif

#if (ipconfigSUPPORT_OUTGOING_PINGS == 1)

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
	(void) eStatus;
	(void) usIdentifier;
}

#endif	/* ipconfigSUPPORT_OUTGOING_PINGS == 1 */


/******************************************************************************/


/**
 * @function
 *
 * @brief		XMC4500 Ethernet 0 IRQ Handler
 *
 * @param[in]
 *
 * <i>Imp Note:</i>
 * 				can be named as IRQ_Hdlr_108 also
 *
 */
void ETH0_0_IRQHandler( void )
{
	/* Local variables */
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	static unsigned long status_l = 0;

	if( xEMACTaskHandle != NULL )
	{
		/* Get Event status */
		status_l = XMC_ETH_MAC_GetEventStatus( &ethernet_mac_handle );
		/* Check for receive status */
		if( status_l & XMC_ETH_MAC_EVENT_RECEIVE )
		{
			/* Trigger semaphore indicating that some data has been received using FromISR */
			xSemaphoreGiveFromISR( xEthRxEventSemaphore, &xHigherPriorityTaskWoken );
		}

		/* Clear Interrupt Flag */
		XMC_ETH_MAC_ClearEventStatus( &ethernet_mac_handle, XMC_ETH_MAC_EVENT_RECEIVE );

	    /* Can call user defined callback function from here to process some other things */

		/* Leave ISR */
	    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}


/********************************** End of File *******************************/
