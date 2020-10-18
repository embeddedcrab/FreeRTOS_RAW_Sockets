/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART API Source
* Filename:		dri_uart.cpp
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_uart.c
 *  @brief:	This header file contains API function definitions for
 *  		UART on USIC channel. It contains Interrupt handlers for UART.
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_uart.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/

#ifdef UART_PROTOCOL_EVENT_USED

/* Protocol events configurations and flags data */
static const XMC_UART_CH_STATUS_FLAG_t uart_event_status_flags[UART_EVENT_MAX] =
{
  XMC_UART_CH_STATUS_FLAG_SYNCHRONIZATION_BREAK_DETECTED,
  XMC_UART_CH_STATUS_FLAG_RECEIVER_NOISE_DETECTED,
  XMC_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_0,
  XMC_UART_CH_STATUS_FLAG_FORMAT_ERROR_IN_STOP_BIT_1,
  XMC_UART_CH_STATUS_FLAG_COLLISION_DETECTED
};


static const XMC_UART_CH_EVENT_t uart_event_conf_flags[UART_EVENT_MAX] =
{
  XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK,
  XMC_UART_CH_EVENT_RECEIVER_NOISE,
  XMC_UART_CH_EVENT_FORMAT_ERROR,
  XMC_UART_CH_EVENT_FORMAT_ERROR,
  XMC_UART_CH_EVENT_COLLISION
};

#endif	/* UART_PROTOCOL_EVENT_USED */


/******************************************************************************
* Private Function Declarations
*******************************************************************************/

/**
 * @function	find_uart_channel_input_source
 *
 * @brief		find channel input source
 *
 * @param[in]	p_context		-	UART channel context
 * @param[out]	eUART_Channel	-	UART channel number
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
static eUART_Channel UART_get_channel( const void * p_context );

/**
 * @function	calc_fifo
 *
 * @brief		calculate FIFO size according to channel
 *
 * @param[in]	channel		-	UART channel
 * @param[in]	fifo_size	-	fifo size array data reference
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
static void calc_fifo( const void * p_channel, unsigned long fifo_size_arr[2] );

/**
 * @function	reconfigure_rx_fifo
 *
 * @brief		Reconfigure fifo limits for receiver
 *
 * @param[in]	data_size	-	required size of data
 *
 * @param[out]	NA
 *
 * \par<b>Description:</b><br>
 * 				This function is used to reconfigure fifo size according to
 * 				required length of data
 *
 * <i>Imp Note:</i>
 * 				A local function to reconfigure Receive FIFO with the given
 * 				size and trigger limit. Size is needed because the FIFO should
 * 				be disabled before changing the trigger limit by clearing the FIFO size.
 *
 */
static void reconfigure_rx_fifo( void * p_channel, unsigned long data_size );


/** Direct Communication Mode functions **/
static long UART_tx_direct( void * p_channel, const unsigned char * p_src, const unsigned long length );
/* TODO: Might not be needed, rx_isr_direct is better for receiving and processing */
static long UART_rx_direct( void * p_channel, unsigned char * const p_dest, const unsigned long length );


/* ISR Handler functions */
#ifdef UART_TX_INTERRUPT_USED
static void UART_tx_isr_entry( void * p_channel );
#endif

#ifdef UART_RX_INTERRUPT_USED
static void UART_rx_isr_direct( void* p_channel );
static void UART_rx_isr_entry( void* p_channel );
#endif

#ifdef UART_PROTOCOL_EVENT_USED
static void UART_err_isr_entry( void* p_channel );
#endif



/******************************************************************************
* Public Function Definitions
*******************************************************************************/

/**
 * @function	UART_init
 *
 * @brief		Initialization function for UART channel
 *
 * <i>Imp Note:</i>
 *
 */
long UART_init( void * p_channel, const unsigned char init )
{
	DRIVER_ASSERT( NULL == p_channel, UART_STATUS_INVALID_ARGUMENT );
	/* Local Variables */
	const tStUART_Configuartion * p_config_l = ((const tStUART_Channel *) p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;

	long ret_status = UART_STATUS_FAILED;
	unsigned long fifo_size[2] = {0U};

	/* calculate FIFO size */
	calc_fifo( p_channel, fifo_size );

	/* Check input argument */
	switch( init )
	{
		/* Open UART Channel */
		case UART_INIT_OPEN:
			/* Check for initialization */
			if( p_control_l->state.state_value > 0 ){ break;	}
			else{	/* Not required */	}

			/* Flush RX FIFO */
			XMC_USIC_CH_RXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
			/* Initialize UART channel for communication */
			XMC_UART_CH_Init( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
								&(p_config_l->channel_cfg) );
			/* Set input source path, decides the mode of communication as well */
			XMC_USIC_CH_SetInputSource( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
							p_config_l->channel_input, p_config_l->channel_input_source );
			/* Configure transmit FIFO */
			XMC_USIC_CH_TXFIFO_Configure( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
											fifo_size[0], p_config_l->fifo_cfg.tx_fifo_size, 1U );
			/* Configure receive FIFO */
			XMC_USIC_CH_RXFIFO_Configure( (XMC_USIC_CH_t *)p_config_l->p_channel_context, fifo_size[1],
											p_config_l->fifo_cfg.rx_fifo_size, 1U );
			/* Start UART channel */
			XMC_UART_CH_Start( (XMC_USIC_CH_t *)p_config_l->p_channel_context );

			#ifdef UART_PROTOCOL_EVENT_USED
			/* Set service request for UART protocol events */
			XMC_USIC_CH_SetInterruptNodePointer( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
								XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL
								#ifdef UART_PROTOCOL_EVENT_USED
								,( p_config_l->isr_config.protocol_events > 0 ? p_config_l->isr_config.event_sr : 0U )
								#else
								,2U
								#endif
								);
			#endif

			XMC_USIC_CH_TXFIFO_SetInterruptNodePointer( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
												XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
												p_config_l->isr_config.tx_sr );

			/* Set service request for rx FIFO receive interrupt */
			XMC_USIC_CH_RXFIFO_SetInterruptNodePointer( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
												XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
												(p_config_l->isr_config.rx_sr) );
			XMC_USIC_CH_RXFIFO_SetInterruptNodePointer( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
												XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE,
												(p_config_l->isr_config.rx_sr) );

			#ifdef UART_PROTOCOL_EVENT_USED
			/* Check for event handling functionality */
			if( p_config_l->isr_config.protocol_events > 0 )
			{
				XMC_UART_CH_EnableEvent( (XMC_USIC_CH_t *)p_config_l->p_channel_context,
											p_config_l->isr_config.protocol_events );
				/* Initialize ISR for events */
				/* Set priority and enable NVIC node for event interrupts */
				NVIC_SetPriority( (IRQn_Type)p_config_l->isr_config.event_irq.irq_num,
									NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
									p_config_l->isr_config.event_irq.irq_priority,
									0U)
				);

				NVIC_EnableIRQ( (IRQn_Type)p_config_l->isr_config.event_irq.irq_num );
			} else{ /* Not required */ }
			#endif	/* UART_PROTOCOL_EVENT_USED */

			/* Set priority and enable NVIC node for transmit interrupt */
			NVIC_SetPriority( (IRQn_Type)(p_config_l->isr_config.tx_irq.irq_num),
								NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
								p_config_l->isr_config.tx_irq.irq_priority,
								0U) );
			NVIC_EnableIRQ( (IRQn_Type)(p_config_l->isr_config.tx_irq.irq_num) );
			/* Set priority and enable NVIC node for receive interrupt */
			NVIC_SetPriority( (IRQn_Type)(p_config_l->isr_config.rx_irq.irq_num),
								NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
								p_config_l->isr_config.rx_irq.irq_priority,
								0U) );
			NVIC_EnableIRQ( (IRQn_Type)(p_config_l->isr_config.rx_irq.irq_num) );

			/* Enable receive buffer event */
			reconfigure_rx_fifo( p_channel, 1 );
			XMC_USIC_CH_RXFIFO_EnableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
					(unsigned long) ((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
					(unsigned long)XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE) );

			/* Set initialize state */
			p_control_l->state.state_value = 1;
			p_control_l->state.mode = 1;	/* Default mode is interrupt */
			/* Update return status */
			ret_status = UART_STATUS_SUCCESS;
			break;
		/* Close UART Channel */
		case UART_INIT_CLOSE:
			if( XMC_UART_CH_STATUS_BUSY == XMC_UART_CH_Stop( (XMC_USIC_CH_t *)p_config_l->p_channel_context )
				)
			{
				/* Update return status */
				ret_status = UART_STATUS_BUSY;
			}
			else
			{
				/* Reset control state */
				p_control_l->state.state_value = 0;
				/* Update return status */
				ret_status = UART_STATUS_SUCCESS;
			}
			break;
		/* Default case */
		default:
			/* Update return status */
			ret_status = UART_STATUS_INVALID_ARGUMENT;
			break;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	UART_set_mode
 *
 * @brief		Mode set function for UART channel
 *
 * <i>Imp Note:</i>
 *
 */
long UART_set_mode( void * p_channel, const unsigned char mode )
{
	DRIVER_ASSERT( NULL == p_channel, UART_STATUS_INVALID_ARGUMENT );
	/* Local Arguments */
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;
	eUART_Status ret_status = UART_STATUS_FAILED;

	/* Check state of Tx and Rx locks */
	if( (1 == p_control_l->state.tx_lock) || (1 == p_control_l->state.rx_lock) ||
			(1 == p_control_l->state.tx_busy) || (1 == p_control_l->state.rx_busy)
			)
	{
		ret_status = UART_STATUS_BUSY;
	}
	else
	{
		/* Update mode */
		p_control_l->state.mode = (UART_MODE_DIRECT == mode) ? 0 :
									(UART_MODE_INTERRUPT == mode) ? 1 : 1;
		ret_status = UART_STATUS_SUCCESS;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	UART_set_baud
 *
 * @brief		Set baud rate for UART channel
 *
 * <i>Imp Note:</i>
 *
 */
long UART_set_baud( void * p_channel, const unsigned long speed )
{
	DRIVER_ASSERT( NULL == p_channel, UART_STATUS_INVALID_ARGUMENT );
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;
	unsigned char status = 1;
	eUART_Status ret_status = UART_STATUS_FAILED;

	/* Check input argument validity */
	switch( speed )
	{
		/* Valid arguments, can add more valid baud rates */
		case 1200:
		case 4800:
		case 9600:
		case 19200:
		case 38400:
		case 115200:
			/* Good to go for baud rate update */
			break;

		/* Invalid arguments */
		default:
			ret_status = UART_STATUS_INVALID_ARGUMENT;
			/* Return status */
			return ret_status;
	}

	/* Check state of Tx and Rx locks */
	if( (1 == p_control_l->state.tx_lock) || (1 == p_control_l->state.rx_lock) ||
			(1 == p_control_l->state.tx_busy) || (1 == p_control_l->state.rx_busy)
			)
	{
		ret_status = UART_STATUS_BUSY;
	}
	else
	{
		/* Update speed */
		status = XMC_UART_CH_SetBaudrate( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
											speed, p_config_l->channel_cfg.oversampling );
		/* Check operation status */
		if( XMC_UART_CH_STATUS_OK == status )
		{
			p_config_l->channel_cfg.baudrate  =speed;
			ret_status = UART_STATUS_SUCCESS;
		}
		else
		{
			ret_status = UART_STATUS_FAILED;
		}
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	UART_transmit
 *
 * @brief		Registers a request for data transmission
 *
 * <i>Imp Note:</i>
 *
 */
long UART_transmit( void * p_channel, const unsigned char * p_src, const unsigned long length )
{
	/* Input argument validity */
	DRIVER_ASSERT( NULL == p_src, UART_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( 0U == length, UART_STATUS_INVALID_ARGUMENT );
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;

	/* Local Arguments */
	eUART_Status ret_status = UART_STATUS_FAILED;
	DRIVER_ASSERT( 0U == p_control_l->state.init, UART_STATUS_NOT_INITIALIZED );


	/* Check mode of UART, Direct or Interrupt */
	switch( p_control_l->state.mode )
	{
	default:
		case UART_MODE_DIRECT:
			ret_status = UART_tx_direct( p_channel, p_src, length );
		break;

		case UART_MODE_INTERRUPT:
			/* Check transmit busy status and perform operation */
			if( 0 == p_control_l->state.tx_busy )
			{
				/* Update required parameters for transmission interrupt */
				p_control_l->p_tx_data = (unsigned char *)p_src;
				p_control_l->tx_data_count = length;
				p_control_l->tx_data_index = 0;
				p_control_l->state.tx_busy = 1;

				/* Trigger transmission interrupt */
				if( p_config_l->fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
				{
					/* Clear transmit FIFO */
					XMC_USIC_CH_TXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
					/* Enable transmit buffer event */
					XMC_USIC_CH_TXFIFO_EnableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
												(unsigned long) XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD );
				}
				else
				{
					/* Enable transmit buffer event */
					XMC_USIC_CH_EnableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
											(unsigned long) XMC_USIC_CH_EVENT_TRANSMIT_BUFFER );
				}
				/* Trigger the transmit buffer interrupt */
				XMC_USIC_CH_TriggerServiceRequest( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
												(unsigned long)p_config_l->isr_config.tx_sr );
				/* Update status */
				ret_status = UART_STATUS_SUCCESS;
			}
			else
			{
				ret_status = UART_STATUS_BUSY;
			}
		break;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	UART_receive
 *
 * @brief		Registers a request for data reception
 *
 * <i>Imp Note:</i>
 *
 */
long UART_receive( void * p_channel, unsigned char * const p_dest, const unsigned long length )
{
	/* Input argument validity */
	DRIVER_ASSERT( NULL == p_dest, UART_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( 0U == length, UART_STATUS_INVALID_ARGUMENT );
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;

	/* Local Arguments */
	eUART_Status ret_status = UART_STATUS_FAILED;
	DRIVER_ASSERT( 0U == p_control_l->state.init, UART_STATUS_NOT_INITIALIZED );

	switch( p_control_l->state.mode )
	{
	default:
		case UART_MODE_DIRECT:
			ret_status = UART_rx_direct( p_channel, p_dest, length );
		break;

		case UART_MODE_INTERRUPT:
			/* Check receive busy status and perform operation */
			if( (0 == p_control_l->state.rx_busy) && (0 == p_control_l->state.rx_lock) )/* 0x18 == (p_control_l->state.state_value & 0x18) */
			{
				/* Update receiver parameters */
				p_control_l->p_rx_data = p_dest;
				p_control_l->rx_data_count = length;
				p_control_l->rx_data_index = 0;
				p_control_l->state.rx_busy = 1;
				p_control_l->state.rx_lock = 1;

				/* Check FIFO configuration and trigger event */
				if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
				{
					/* Clear receiver FIFO */
					XMC_USIC_CH_RXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );

					/* Reconfigure FIFO for current data size */
					reconfigure_rx_fifo( p_channel, p_control_l->rx_data_count );

					/* Enable receive buffer event */
					XMC_USIC_CH_RXFIFO_EnableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
								(unsigned long) ((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
								(unsigned long)XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE) );
				}
				else
				{/* Enable receive buffer event */
					XMC_USIC_CH_RXFIFO_EnableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
								(unsigned long) ((unsigned long) XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
								(unsigned long) XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE) );
				}
				/* Update status */
				ret_status = UART_STATUS_SUCCESS;
			}
			else
			{
				ret_status = UART_STATUS_BUSY;
			}
		break;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function	UART_abort
 *
 * @brief		Abort transmission or reception
 *
 * <i>Imp Note:</i>
 *
 */
long UART_abort( void * p_channel, const unsigned char tx_rx )
{
	/* Input argument validity */
	DRIVER_ASSERT( NULL == p_channel, UART_STATUS_INVALID_ARGUMENT );
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;
	eUART_Status ret_status = UART_STATUS_FAILED;
	DRIVER_ASSERT( 0U == p_control_l->state.init, UART_STATUS_NOT_INITIALIZED );

	/* Check the abort request */
	switch( tx_rx )
	{
		case UART_ABORT_TX:
			/* Reset Transmitter Parameters */
			p_control_l->state.tx_busy = 0;
			p_control_l->p_tx_data = NULL;

			/* Disable the transmitter events */
			if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
			      /* Disable the transmit FIFO event */
			      XMC_USIC_CH_TXFIFO_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
			    		  	  	  	  	  (unsigned long) XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD );
			      XMC_USIC_CH_TXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
			}
			else
			{
			      /*Disable the standard transmit event*/
			      XMC_USIC_CH_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
			    		  	  	  	  (unsigned long) XMC_USIC_CH_EVENT_TRANSMIT_BUFFER );
			}
			XMC_USIC_CH_SetTransmitBufferStatus( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
													XMC_USIC_CH_TBUF_STATUS_SET_IDLE );
		break;

		case UART_ABORT_RX:
			/* Reset Receiver Parameters */
			p_control_l->state.rx_busy = 0;
			p_control_l->p_rx_data = NULL;
			/* Disable the transmitter events */
			if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Disable the receiver event */
				XMC_USIC_CH_TXFIFO_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
						(unsigned long) ((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
										(unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE) );
			}
			else
			{
				XMC_UART_CH_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
						(unsigned long) ((unsigned long)XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
								(unsigned long)XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE) );
			}
		break;

		case UART_ABORT_TX_RX:
			/* Reset Transmitter and Receiver parameters */
			p_control_l->state.tx_busy = 0;
			p_control_l->p_tx_data = NULL;
			p_control_l->state.rx_busy = 0;
			p_control_l->p_rx_data = NULL;

			/* Disable the transmitter events */
			if( p_config_l->fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Disable the transmit FIFO event */
				XMC_USIC_CH_TXFIFO_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
									(unsigned long) XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD );
				XMC_USIC_CH_TXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
			}
			else
			{
				/*Disable the standard transmit event*/
				XMC_USIC_CH_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
									(unsigned long) XMC_USIC_CH_EVENT_TRANSMIT_BUFFER );
			}
			XMC_USIC_CH_SetTransmitBufferStatus( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
													XMC_USIC_CH_TBUF_STATUS_SET_IDLE );

			/* Disable the receiver events */
			if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Disable the receiver event */
				XMC_USIC_CH_TXFIFO_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context), (unsigned long)
									((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
									(unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE)
									);
			}
			else
			{
				XMC_UART_CH_DisableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
					(unsigned long) ((unsigned long)XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
								(unsigned long)XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE) );
			}
		break;
		default:
			/* Return with Failed status */
			break;
	}

	/* Return status */
	return ret_status;
}


/******************************************************************************
* Private Function Definitions
*******************************************************************************/

eUART_Channel UART_get_channel( const void * p_context )
{
	/* Local variable */
	XMC_USIC_CH_t * context = (XMC_USIC_CH_t *)p_context;
	eUART_Channel channel;

	/* Check for UART channel number */
	if( XMC_UART0_CH0 == context )
	{
		channel = UART_CHANNEL_0;
	}
	else if( XMC_UART0_CH1 == context )
	{
		channel = UART_CHANNEL_1;
	}
	else if( XMC_UART1_CH0 == context )
	{
		channel = UART_CHANNEL_2;
	}
	else if( XMC_UART1_CH1 == context )
	{
		channel = UART_CHANNEL_3;
	}
	else if( XMC_UART2_CH0 == context )
	{
		channel = UART_CHANNEL_4;
	}
	else if( XMC_UART2_CH1 == context )
	{
		channel = UART_CHANNEL_5;
	}
	else{ /* else not required */ }
	return channel;
}


void calc_fifo( const void * p_channel, unsigned long fifo_size_arr[2] )
{
	/* Local variables */
	const tStUART_Configuartion * p_config_l = ((const tStUART_Channel *) p_channel)->p_config;
	eUART_Channel channel = UART_get_channel( p_config_l->p_channel_context );

	/* Check channel */
	switch( channel )
	{
		default:/* Considering all in default */
		case UART_CHANNEL_0:
		case UART_CHANNEL_2:
		case UART_CHANNEL_4:
			/* Update fifo_size */
			fifo_size_arr[0] = 48U;
			fifo_size_arr[1] = 32U;
			break;
		case UART_CHANNEL_1:
		case UART_CHANNEL_3:
		case UART_CHANNEL_5:
			/* Update fifo_size */
			fifo_size_arr[0] = 16U;
			fifo_size_arr[1] = 0U;
			break;
	}
}


void reconfigure_rx_fifo( void * p_channel, unsigned long data_size )
{
	/* Local Variables */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	unsigned long fifo_size = 0U;
	unsigned long ret_limit_val = 0U;

	/* Get FIFO size in bytes */
	fifo_size = (unsigned long) ( 0x01UL << (unsigned char) (p_config_l->fifo_cfg.rx_fifo_size) );
	/* If data size is more than FIFO size, configure the limit to the FIFO size */
	ret_limit_val = ( data_size < fifo_size ) ? data_size - 1U : fifo_size - 1U;
	/* Set the limit value */
	XMC_USIC_CH_RXFIFO_SetSizeTriggerLimit( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
											p_config_l->fifo_cfg.rx_fifo_size, ret_limit_val );
}


static long UART_tx_direct( void * p_channel, const unsigned char * p_src, const unsigned long length )
{
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;
	unsigned long count_l = 0;
	eUART_Status ret_status = UART_STATUS_SUCCESS;

	/* Wait for current transfer to complete */
	while( 1 == p_control_l->state.tx_busy );
	p_control_l->state.tx_busy = 1;

	/* Check FIFO usage */
	if( p_config_l->fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
	{
		XMC_USIC_CH_TXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
		for( count_l = 0; count_l < length; ++count_l )
		{
			XMC_UART_CH_Transmit( (XMC_USIC_CH_t *)(p_config_l->p_channel_context), p_src[count_l] );
			if( 1 == XMC_USIC_CH_TXFIFO_IsFull( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
			{
				XMC_USIC_CH_TXFIFO_Flush( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
			}
		}
	}
	else
	{
		for( count_l = 0; count_l < length; ++count_l )
		{
			XMC_UART_CH_Transmit( (XMC_USIC_CH_t *)(p_config_l->p_channel_context), p_src[count_l] );
		}
	}
	/* Update transfer parameters */
	p_control_l->state.tx_busy = 0;

	return ret_status;
}


static long UART_rx_direct( void * p_channel, unsigned char * const p_dest, const unsigned long length )
{
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;
	unsigned long count_l = 0;
	eUART_Status ret_status = UART_STATUS_SUCCESS;

	/* Wait for current transfer to complete */
	while( (1 == p_control_l->state.rx_busy) && (1 == p_control_l->state.rx_lock) );
	p_control_l->state.rx_busy = 1;
	p_control_l->state.rx_lock = 1;

	/* Receive data directly */
	if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
	{
		for( count_l = 0; count_l < length; ++count_l )
		{
			/* Check if FIFO is empty or not */
			if( 0 == XMC_USIC_CH_RXFIFO_IsEmpty( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
			{
				p_dest[count_l] = (unsigned char)XMC_UART_CH_GetReceivedData( (XMC_USIC_CH_t *const)p_config_l->p_channel_context );
			}
			else if( 1 == XMC_USIC_CH_RXFIFO_IsEmpty( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
			{
				/* Dynamically setup the trigger limit for FIFO */
				if( count_l < length )
				{
					reconfigure_rx_fifo( p_channel, (length - count_l) );
				}
			} else { /* Not needed */ }
		}
	}
	else
	{
		for( count_l = 0; count_l < length; ++count_l )
		{
			/* Wait to receive data */
			while( 0 == (XMC_UART_CH_GetStatusFlag( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) ) & ((1<<14) | (1<<15)) ) );
			p_dest[count_l] = (unsigned char)XMC_UART_CH_GetReceivedData( (XMC_USIC_CH_t *)(p_config_l->p_channel_context) );
		}
	}

	/* Update transfer parameters */
	p_control_l->state.rx_busy = 0;
	p_control_l->state.rx_lock = 0;
	return ret_status;
}


#ifdef UART_TX_INTERRUPT_USED
/**
 * @function	UART_tx_isr_entry
 *
 * @brief		UART Transmission ISR body
 *
 * <i>Imp Note:</i>
 *
 */
void UART_tx_isr_entry( void * p_channel )
{
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;

	if( 1 == p_control_l->state.tx_busy )
	{
		/* Check data sent */
		if( p_control_l->tx_data_index < p_control_l->tx_data_count )
		{
			/* Check FIFO used or not */
			if( p_config_l->fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
			{
				/* Fill FIFO and send data */
				while( 0 == XMC_USIC_CH_TXFIFO_IsFull( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
				{
					if( p_control_l->tx_data_index < p_control_l->tx_data_count )
					{
						/* Load the FIFO byte by byte till either FIFO is full or all data is loaded */
						XMC_UART_CH_Transmit( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
								(unsigned short) p_control_l->p_tx_data[p_control_l->tx_data_index] );
						/* Increment data index */
						++(p_control_l->tx_data_index);
					}
					else
					{
						/* FIFO has been filled completely or data has been loaded into it */
						break;
					}
				}
			}
			else/* Send directly */
			{
				XMC_UART_CH_Transmit( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
						p_control_l->p_tx_data[p_control_l->tx_data_index] );
				++(p_control_l->tx_data_index);
			}
		}
		else/* Data is fully sent, ensure it and complete the transfer */
		{
			/* Check FIFO empty status now */
			if( 1 == XMC_USIC_CH_TXFIFO_IsEmpty( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
			{
				if( p_config_l->fifo_cfg.tx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
				{
					/* Disable fifo transmit event event */
					XMC_USIC_CH_TXFIFO_DisableEvent( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
												(unsigned long) XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD );
				}
				else
				{
					/* Disable standard transmit event */
					XMC_USIC_CH_DisableEvent( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
												(unsigned long) XMC_USIC_CH_EVENT_TRANSMIT_BUFFER );
				}

				/* Wait for transmit buffer to get free to ensure data has been sent */
				while( XMC_USIC_CH_TBUF_STATUS_BUSY ==
						XMC_USIC_CH_GetTransmitBufferStatus( (XMC_USIC_CH_t *const)p_config_l->p_channel_context )
						);

				/* Release resources for transmission */
				p_control_l->p_tx_data = NULL;
				p_control_l->state.tx_busy = 0;

				/* Call callback if available */
				#if( UART_CALLBACK_HANDLER_USED == 1U )
				((tStUART_Channel*)p_channel)->p_callback->fp_callback( UART_get_channel( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ),
																		NULL, UART_EVENT_TX_COMPLETE );
				#endif
			}
		}
	}
}

#endif


#ifdef UART_RX_INTERRUPT_USED
/**
 * @function	UART_rx_isr_direct
 *
 * @brief		UART Reception direct ISR body
 *
 * <i>Imp Note:</i>
 *
 */
void UART_rx_isr_direct( void * p_channel )
{
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	/* Receive data directly and pass it to callback,
	 * for this operation events for receiver should always be enabled
	 *
	 * Note: If need this functionality then, enable receiver events for direct use
	 *  */
	unsigned char data_l = 255;

	/* Clear receive event */
	XMC_USIC_CH_DisableEvent( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
			(unsigned long) ((unsigned long) XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
			(unsigned long) XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE) );

	/* Check for FIFO usage */
	if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
	{
		/* Check if FIFO is empty or not */
		if( 0 == XMC_USIC_CH_RXFIFO_IsEmpty( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
		{
			data_l = (unsigned char)XMC_UART_CH_GetReceivedData( (XMC_USIC_CH_t *const)p_config_l->p_channel_context );
		}
	}
	else
	{
		/* Receive data */
		data_l = (unsigned char)XMC_UART_CH_GetReceivedData( (XMC_USIC_CH_t *const)p_config_l->p_channel_context );
	}

	/* Call callback */
	#if( UART_CALLBACK_HANDLER_USED == 1U )
	((tStUART_Channel*)p_channel)->p_callback->fp_callback( UART_get_channel( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ),
															&data_l, UART_EVENT_RX_CHAR );
	#endif
	/* Enable Rx Event */
	XMC_USIC_CH_RXFIFO_EnableEvent( (XMC_USIC_CH_t *)(p_config_l->p_channel_context),
				(unsigned long) ((unsigned long) XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
				(unsigned long) XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE) );
}


/**
 * @function	UART_rx_isr_entry
 *
 * @brief		UART Reception ISR body
 *
 * <i>Imp Note:</i>
 *
 */
void UART_rx_isr_entry( void * p_channel )
{
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	tStUART_Control * p_control_l = ((tStUART_Channel*) p_channel)->p_control;
	if( (1 == p_control_l->state.rx_lock) && (1 == p_control_l->state.rx_busy) )
	{
		/* Check for FIFO usage */
		if( p_config_l->fifo_cfg.rx_fifo_size != XMC_USIC_CH_FIFO_DISABLED )
		{
			/* Check FIFO empty status */
			while( 0 == XMC_USIC_CH_RXFIFO_IsEmpty( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ) )
			{
				/* Receive data */
				if( p_control_l->rx_data_index < p_control_l->rx_data_count )
				{
					p_control_l->p_rx_data[p_control_l->rx_data_index] =
						(unsigned char)XMC_UART_CH_GetReceivedData( (XMC_USIC_CH_t *const)p_config_l->p_channel_context );
					/* Increment data index */
					++(p_control_l->rx_data_index);
				}
				/* Check for data count and index equality */
				if( p_control_l->rx_data_index == p_control_l->rx_data_count )
				{
					/* Release FIFO and reset parameters*/
					p_control_l->state.rx_busy = 0;
					p_control_l->state.rx_lock = 0;
					XMC_USIC_CH_RXFIFO_DisableEvent( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
									(unsigned long) ((unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
									(unsigned long) XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE) );

					/* Call callback if available */
					#if( UART_CALLBACK_HANDLER_USED == 1U )
					((tStUART_Channel*)p_channel)->p_callback->fp_callback( UART_get_channel( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ),
																			NULL, UART_EVENT_RX_COMPLETE );
					#endif
					/* Break operation */
					break;
				}
			}
			/* Dynamically setup the trigger limit for FIFO */
			if( p_control_l->rx_data_index < p_control_l->rx_data_count )
			{
				reconfigure_rx_fifo( p_channel, p_control_l->rx_data_count - p_control_l->rx_data_index );
			}
		}
		else
		{
			/* Check data index */
			if( p_control_l->rx_data_index < p_control_l->rx_data_count )
			{
				/* receive data and put it in buffer */
				p_control_l->p_rx_data[p_control_l->rx_data_index] = (unsigned char)XMC_UART_CH_GetReceivedData(
															(XMC_USIC_CH_t *const)p_config_l->p_channel_context );
				/* Update data index */
				++(p_control_l->rx_data_index);
			}
			else
			{
				/* Release rx control */
				p_control_l->state.rx_busy = 0;
				p_control_l->state.rx_lock = 0;
				/* Disable events */
				XMC_USIC_CH_DisableEvent( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
							(unsigned long) ((unsigned long) XMC_USIC_CH_EVENT_STANDARD_RECEIVE |
							(unsigned long) XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE) );
				/* Call callback */
				#if( UART_CALLBACK_HANDLER_USED == 1U )
				((tStUART_Channel*)p_channel)->p_callback->fp_callback( UART_get_channel( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ),
																		NULL, UART_EVENT_RX_COMPLETE );
				#endif
			}
		}
	}
	else
	{
		/* Receive data directly from ISR without explicitly asking for it */
		UART_rx_isr_direct( p_channel );
	}
}

#endif


#ifdef UART_PROTOCOL_EVENT_USED
/**
 * @function	UART_err_isr_entry
 *
 * @brief		UART Error ISR body
 *
 * <i>Imp Note:</i>
 *
 */
void UART_err_isr_entry( void * p_channel )
{
	/* Local Arguments */
	tStUART_Configuartion * p_config_l = ((tStUART_Channel*)p_channel)->p_config;
	/* Local variables */
	unsigned long count_l = 0;
	unsigned long max_val_l = UART_EVENT_MAX;
	/* Get event status */
	unsigned long psr_status_l = XMC_UART_CH_GetStatusFlag( (XMC_USIC_CH_t *const)p_config_l->p_channel_context );
	unsigned long event_cfg_l = ((XMC_USIC_CH_t *const)p_config_l->p_channel_context)->PSR_ASCMode;

	/* Check events, if event available then call particular callback one by one */
	for( count_l = 0; count_l < max_val_l; ++count_l )
	{
		if( (event_cfg_l & (unsigned long) uart_event_conf_flags[count_l]) &&
				(psr_status_l & (unsigned long) uart_event_status_flags[count_l]) )
		{
			/* Clear status flag */
			XMC_UART_CH_ClearStatusFlag( (XMC_USIC_CH_t *const)p_config_l->p_channel_context,
					(unsigned long) uart_event_status_flags[count_l] );
			/* Call Callback */
			#if( UART_CALLBACK_HANDLER_USED == 1U )
			((tStUART_Channel*)p_channel)->p_callback->fp_callback( UART_get_channel( (XMC_USIC_CH_t *const)p_config_l->p_channel_context ),
																	NULL, count_l );
			#endif
			break;
		}
	}
}

#endif


/**
 * @function	UART_isr_entry
 *
 * @brief		Common ISR entry function for UART Channels
 *
 * <i>Imp Note:</i>
 *
 */
#if (defined(UART_TX_INTERRUPT_USED) ||	\
	defined(UART_RX_INTERRUPT_USED) ||	\
	defined(UART_PROTOCOL_EVENT_USED))
void UART_isr_entry( void * p_channel, const eUART_InterruptOperation type );
void UART_isr_entry( void * p_channel, const eUART_InterruptOperation type )
{
	DRIVER_ASSERT_NRET( NULL == p_channel );
	/* Check for UART channel and type of ISR request */
	switch( type )
	{
	default:/* Considering all cases in default */
	#ifdef UART_TX_INTERRUPT_USED
		case UART_INTERRUPT_OPERATION_TX:
			UART_tx_isr_entry( p_channel );
		break;
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		case UART_INTERRUPT_OPERATION_RX:
			UART_rx_isr_entry( p_channel );
		break;
	#endif
	#ifdef UART_PROTOCOL_EVENT_USED
		case UART_INTERRUPT_OPERATION_ERROR:
			UART_err_isr_entry( p_channel );
		break;
	#endif
	}
}

#endif


/********************************** End of File *******************************/
