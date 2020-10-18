/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		Peripherals Hooks Source
* Filename:		peripherals_hooks.c
* Author:		HS
* Origin Date:
* Version:
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	peripherals_hooks.c
 *  @brief:	This source file contains callback functions for peripherals
 */


/******************************************************************************
* Includes
*******************************************************************************/

/* Include peripherals files*/
#include <dri_uart_extern.h>
#include <dri_can_extern.h>
#include <dri_rtc_extern.h>
#include <dri_scu_extern.h>


/* Include Standard Headers */
#include <dri_mem.h>



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
* Functions
*******************************************************************************/

#if (UART_CALLBACK_HANDLER_USED == 1U)
/* UART Callback handler */
void UART_callback_handler( const eUART_Channel channel, unsigned char *p_data, const eUART_Event event );
void UART_callback_handler( const eUART_Channel channel, unsigned char *p_data, const eUART_Event event )
{
	/* Local variables */
	unsigned char * p_data_l = p_data;

	/* Check events */
	switch( event )
	{
		default:
		case UART_EVENT_TX_COMPLETE:
			if( UART_CHANNEL_0 == channel )
			{
				/* Transmission successful! */
				__asm("NOP");
			}
			break;

		case UART_EVENT_RX_COMPLETE:
			if( UART_CHANNEL_0 == channel )
			{
				/* Reception successful! */
				__asm("NOP");
			}
			break;
		case UART_EVENT_RX_CHAR:
			switch( channel )
			{
			default:
			case UART_CHANNEL_0:
				/* Reception successfull, transmit received data */
				UART_Api_Functions.fp_transmit( &UART_Channel_0, p_data_l, 1 );
				break;
			case UART_CHANNEL_2:
				/* Reception successfull, transmit received data */
				break;
			}

			break;
	}
}
#endif	/* UART_CALLBACK_HANDLER_USED */


/* CAN Callback Handler */
#if (CAN_CALLBACK_HANDLER_USED == 1U)
void CAN_callback_handler( void * p_channel, const eCAN_EventType event_type, const eCAN_Event event );
void CAN_callback_handler( void * p_channel, const eCAN_EventType event_type, const eCAN_Event event )
{
	DRIVER_ASSERT_NRET( NULL == p_channel );
	/* Local Variables */
	tStCAN_NodeConfiguartion * const p_config_l = ((tStCAN_Node *)p_channel)->p_config;
	tStCAN_Control * const p_control_l = ((tStCAN_Node *)p_channel)->p_control;

	/* Check Event Type */
	switch( event_type )
	{
	default:
		case CAN_EVENT_TYPE_NODE_TRANSFER:
		/* Check Node Transfer event */
		switch( event )
		{
		default:
			case CAN_EVENT_NODE_TRANSFER_TRANSMIT:
				if( (1 == p_control_l->state.init) && (1 == p_control_l->state.tx_busy) )
				{
					p_control_l->state.tx_busy = 0;
				}
			break;
			case CAN_EVENT_NODE_TRANSFER_RECEIVE:
				if( (1 == p_control_l->state.init) && (0 == p_control_l->state.rx_busy) )
				{
					XMC_CAN_MO_ReceiveData( p_config_l->p_message_rx );
					p_control_l->state.rx_busy = 0;
				}
				else/* Direct Read Data from CAN Node in a common Mo */
				{
				}
			break;
		}
		break;	/* CAN_EVENT_TYPE_NODE_TRANSFER */
	}
}
#endif


/* SCU Callback Handler */
#if (SCU_CALLBACK_HANDLER_USED == 1U)
void SCU_callback_handler( void );
void SCU_callback_handler( void )
{
	__asm("NOP");
}
#endif


/* RTC Callback Handler */
#if (RTC_CALLBACK_HANDLER_USED == 1U)
void RTC_callback_handler( void * p_channel, const eRTC_Events event );
void RTC_callback_handler( void * p_channel, const eRTC_Events event )
{
	(void) p_channel;
	(void) event;
}
#endif


/********************************** End of File *******************************/
