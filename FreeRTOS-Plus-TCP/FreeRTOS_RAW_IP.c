/*********************************************************************************************************************
 * @file     FreeRTOS_RAW_IP.cpp
 * @brief    FreeRTOS RAW Sockets source file
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

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_ARP.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "NetworkBufferManagement.h"

#if( ipconfigUSE_DNS == 1 )
	#include "FreeRTOS_DNS.h"
#endif


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Declarations
*******************************************************************************/

/*
 * This function process raw packets using IPPROTO_RAW
 */
static void prvProcessRAWPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer );

/*
 * This function process raw packets using IPPROTO_UDP
 */
static void prvProcessUDPPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer );


/******************************************************************************
* Function Definitions
*******************************************************************************/

void vProcessGeneratedRAWPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
const char *pxSrcAddrOffset = ( char *) ( &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( MACAddress_t ) ] ) );

	/* Check protocol being used for RAW Sockets */
	switch( pxSrcAddrOffset[17] )
	{
	case ipPROTOCOL_RAW:
		prvProcessRAWPacket( pxNetworkBuffer );
		break;

	case ipPROTOCOL_UDP:
		prvProcessUDPPacket( pxNetworkBuffer );
		break;

	default:
		/* TODO: Handle Error */
		break;
	}
}
/*-----------------------------------------------------------*/

void prvProcessRAWPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
IPPacket_t *pxIPPacket = NULL;
IPHeader_t *pxIPHeader = NULL;
eARPLookupResult_t eReturned;
uint32_t ulIPAddress = pxNetworkBuffer->ulIPAddress;
size_t uxPayloadSize;

	/* Map the IP Packet on start of frame */
	pxIPPacket = ipCAST_PTR_TO_TYPE_PTR(IPPacket_t, pxNetworkBuffer->pucEthernetBuffer);
	uxPayloadSize = pxNetworkBuffer->xDataLength - sizeof( IPPacket_t );

	/* Determine the ARP cache status for the requested IP address. */
	eReturned = eARPGetCacheEntry( &( ulIPAddress ), &( pxIPPacket->xEthernetHeader.xDestinationAddress ) );

	if( eReturned != eCantSendPacket )
	{
		if( eReturned == eARPCacheHit )
		{
			iptraceSENDING_RAW_PACKET( pxNetworkBuffer->ulIPAddress );

			/* Create short cuts to the data within the packet. */
			pxIPHeader = &( pxIPPacket->xIPHeader );

			/* Save options now, as they will be overwritten by memcpy */
			/*
			 * Offset the memcpy by the size of a MAC address to start at the packet's
			 * Ethernet header 'source' MAC address; the preceding 'destination' should not be altered.
			 */
			/* The Ethernet source address is at offset 6. */
			/* Note: Overwrite the existing data including protocol as set in UDP structure,
			 * reusing the existing one for now */
//			char *pxIpSrcAddrOffset = ( char *) ( &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( MACAddress_t ) ] ) );
//			( void ) memcpy( ( void * ) pxIpSrcAddrOffset, ( const void * ) ( xDefaultPartRAWPacketHeader.ucBytes ), sizeof( xDefaultPartRAWPacketHeader ) );

			pxIPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( IPHeader_t ) );

			pxIPHeader->usLength = FreeRTOS_htons( pxIPHeader->usLength );
			pxIPHeader->ulDestinationIPAddress = pxNetworkBuffer->ulIPAddress;

			#if( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
			{
				pxIPHeader->usHeaderChecksum = 0U;
				pxIPHeader->usHeaderChecksum = usGenerateChecksum( 0U, ( uint8_t * ) &( pxIPHeader->ucVersionHeaderLength ), ipSIZE_OF_IPv4_HEADER );
				pxIPHeader->usHeaderChecksum = ~FreeRTOS_htons( pxIPHeader->usHeaderChecksum );
			}
			#endif
		}
		else if( eReturned == eARPCacheMiss )
		{
			/* Add an entry to the ARP table with a null hardware address.
				This allows the ARP timer to know that an ARP reply is
				outstanding, and perform retransmissions if necessary. */
			vARPRefreshCacheEntry( NULL, ulIPAddress );

			/* Generate an ARP for the required IP address. */
			iptracePACKET_DROPPED_TO_GENERATE_ARP( pxNetworkBuffer->ulIPAddress );
			pxNetworkBuffer->ulIPAddress = ulIPAddress;
			vARPGenerateRequestPacket( pxNetworkBuffer );
		}
		else
		{
			/* The lookup indicated that an ARP request has already been
				sent out for the queried IP address. */
			eReturned = eCantSendPacket;
		}
	}

	if( eReturned != eCantSendPacket )
	{
		/* The network driver is responsible for freeing the network buffer
			after the packet has been sent. */
		( void ) xNetworkInterfaceOutput( pxNetworkBuffer, pdTRUE );
	}
	else
	{
		/* The packet can't be sent (DHCP not completed?).  Just drop the
			packet. */
		vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
	}
}
/*-----------------------------------------------------------*/

/*
 * Note: This function is same as 'vProcessGeneratedUDPPacket' present in
 * UDP_IP.c file currently. Has been made local so that Customizations can
 * easily be done without considering the effects on on other functions.
 */
void prvProcessUDPPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
UDPPacket_t *pxUDPPacket;
IPHeader_t *pxIPHeader;
eARPLookupResult_t eReturned;
uint32_t ulIPAddress = pxNetworkBuffer->ulIPAddress;
size_t uxPayloadSize;

	/* Map the UDP packet onto the start of the frame. */
	pxUDPPacket = ipCAST_PTR_TO_TYPE_PTR( UDPPacket_t, pxNetworkBuffer->pucEthernetBuffer );

#if ipconfigSUPPORT_OUTGOING_PINGS == 1
	if( pxNetworkBuffer->usPort == ( uint16_t ) ipPACKET_CONTAINS_ICMP_DATA )
	{
		uxPayloadSize = pxNetworkBuffer->xDataLength - sizeof( ICMPPacket_t );
	}
	else
#endif
	{
		uxPayloadSize = pxNetworkBuffer->xDataLength - sizeof( UDPPacket_t );
	}

	/* Determine the ARP cache status for the requested IP address. */
	eReturned = eARPGetCacheEntry( &( ulIPAddress ), &( pxUDPPacket->xEthernetHeader.xDestinationAddress ) );

	if( eReturned != eCantSendPacket )
	{
		if( eReturned == eARPCacheHit )
		{
			#if( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
				uint8_t ucSocketOptions;
			#endif
			iptraceSENDING_RAW_PACKET( pxNetworkBuffer->ulIPAddress );

			/* Create short cuts to the data within the packet. */
			pxIPHeader = &( pxUDPPacket->xIPHeader );

			#if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )
			/* Is it possible that the packet is not actually a UDP packet
				after all, but an ICMP packet. */
			if( pxNetworkBuffer->usPort != ( uint16_t ) ipPACKET_CONTAINS_ICMP_DATA )
			#endif /* ipconfigSUPPORT_OUTGOING_PINGS */
			{
				UDPHeader_t *pxUDPHeader;

				pxUDPHeader = &( pxUDPPacket->xUDPHeader );

				pxUDPHeader->usDestinationPort = pxNetworkBuffer->usPort;
				pxUDPHeader->usSourcePort = pxNetworkBuffer->usBoundPort;
				pxUDPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( UDPHeader_t ) );
				pxUDPHeader->usLength = FreeRTOS_htons( pxUDPHeader->usLength );
				pxUDPHeader->usChecksum = 0U;
			}
			/* Save options now, as they will be overwritten by memcpy */
			#if( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
			{
				ucSocketOptions = pxNetworkBuffer->pucEthernetBuffer[ ipSOCKET_OPTIONS_OFFSET ];
			}
			#endif
			/*
			 * Offset the memcpy by the size of a MAC address to start at the packet's
			 * Ethernet header 'source' MAC address; the preceding 'destination' should not be altered.
			 */
			/* The Ethernet source address is at offset 6. */
			/* Note: Overwrite the existing data including protocol as set in UDP structure,
			 * reusing the existing one for now */
//			char *pxUdpSrcAddrOffset = ( char *) ( &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( MACAddress_t ) ] ) );
//			( void ) memcpy( ( void * ) pxUdpSrcAddrOffset, ( const void * ) ( xDefaultPartUDPPacketHeader.ucBytes ), sizeof( xDefaultPartUDPPacketHeader ) );

			#if ipconfigSUPPORT_OUTGOING_PINGS == 1
			if( pxNetworkBuffer->usPort == ( uint16_t ) ipPACKET_CONTAINS_ICMP_DATA )
			{
				pxIPHeader->ucProtocol = ipPROTOCOL_ICMP;
				pxIPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( IPHeader_t ) + sizeof( ICMPHeader_t ) );
			}
			else
			#endif /* ipconfigSUPPORT_OUTGOING_PINGS */
			{
				pxIPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( IPHeader_t ) + sizeof( UDPHeader_t ) );
			}

			pxIPHeader->usLength = FreeRTOS_htons( pxIPHeader->usLength );
			pxIPHeader->ulDestinationIPAddress = pxNetworkBuffer->ulIPAddress;

			#if( ipconfigUSE_LLMNR == 1 )
			{
				/* LLMNR messages are typically used on a LAN and they're
				 * not supposed to cross routers */
				if( pxNetworkBuffer->ulIPAddress == ipLLMNR_IP_ADDR )
				{
					pxIPHeader->ucTimeToLive = 0x01;
				}
			}
			#endif

			#if( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
			{
				pxIPHeader->usHeaderChecksum = 0U;
				pxIPHeader->usHeaderChecksum = usGenerateChecksum( 0U, ( uint8_t * ) &( pxIPHeader->ucVersionHeaderLength ), ipSIZE_OF_IPv4_HEADER );
				pxIPHeader->usHeaderChecksum = ~FreeRTOS_htons( pxIPHeader->usHeaderChecksum );

				if( ( ucSocketOptions & ( uint8_t ) FREERTOS_SO_UDPCKSUM_OUT ) != 0U )
				{
					( void ) usGenerateProtocolChecksum( ( uint8_t * ) pxUDPPacket, pxNetworkBuffer->xDataLength, pdTRUE );
				}
				else
				{
					pxUDPPacket->xUDPHeader.usChecksum = 0U;
				}
			}
		#endif
		}
		else if( eReturned == eARPCacheMiss )
		{
			/* Add an entry to the ARP table with a null hardware address.
				This allows the ARP timer to know that an ARP reply is
				outstanding, and perform retransmissions if necessary. */
			vARPRefreshCacheEntry( NULL, ulIPAddress );

			/* Generate an ARP for the required IP address. */
			iptracePACKET_DROPPED_TO_GENERATE_ARP( pxNetworkBuffer->ulIPAddress );
			pxNetworkBuffer->ulIPAddress = ulIPAddress;
			vARPGenerateRequestPacket( pxNetworkBuffer );
		}
		else
		{
			/* The lookup indicated that an ARP request has already been
				sent out for the queried IP address. */
			eReturned = eCantSendPacket;
		}
	}

	if( eReturned != eCantSendPacket )
	{
		/* The network driver is responsible for freeing the network buffer
			after the packet has been sent. */

		#if defined( ipconfigETHERNET_MINIMUM_PACKET_BYTES )
		{
			if( pxNetworkBuffer->xDataLength < ( size_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES )
			{
				BaseType_t xIndex;

				for( xIndex = ( BaseType_t ) pxNetworkBuffer->xDataLength; xIndex < ( BaseType_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES; xIndex++ )
				{
					pxNetworkBuffer->pucEthernetBuffer[ xIndex ] = 0U;
				}
				pxNetworkBuffer->xDataLength = ( size_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES;
			}
		}
		#endif

		( void ) xNetworkInterfaceOutput( pxNetworkBuffer, pdTRUE );
	}
	else
	{
		/* The packet can't be sent (DHCP not completed?).  Just drop the
			packet. */
		vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
	}
}
/*-----------------------------------------------------------*/


/********************************** End of File *******************************/
