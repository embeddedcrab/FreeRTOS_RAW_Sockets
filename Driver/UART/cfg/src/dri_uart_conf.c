/*****************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART Configuration Source
* Filename:		dri_uart_conf.cpp
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_uart_conf.c
 *  @brief:	This source file contains configuration function definitions for
 *  		UART on USIC channel
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

/*
 * @brief	UART API functions
 *
 * \par<b>Description:</b><br>
 * 			This is data structure for UART API functions to be used by Application
 */
tStUART_Api UART_Api_Functions =
{
	.fp_init = &UART_init,
	.fp_set_baud = &UART_set_baud,
	.fp_transmit = &UART_transmit,
	.fp_receive = &UART_receive,
	.fp_abort = &UART_abort
};


/* Callback handler for user */
#if ( UART_CALLBACK_HANDLER_USED == 1U )
extern void UART_callback_handler( const eUART_Channel channel, unsigned char *p_data, const eUART_Event event );
#endif


/** Configurations for UART Channels **/
#if (UART_CHANNELS_USED > 0)

/* UART Channel 0 usage */
#ifdef UART_CHANNEL_0_USED
/* UART Channel 0 Configurations Data Structure */
tStUART_Configuartion UART_Channel_0_Configuration =
{
	.p_channel_context = (XMC_USIC_CH_t *) XMC_UART0_CH0,
	.channel_cfg =
	{
		.baudrate = 115200,
		.data_bits = 8U,
		.frame_length = 8U,
		.stop_bits = 1U,
		.oversampling = 16U,
		.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
	},
	#if( UART_INTERRUPT_USED == 1U )
	.isr_config =
	{
		.tx_sr = UART_0_TX_SR,
		.rx_sr = UART_0_RX_SR,
	#ifdef UART_PROTOCOL_EVENT_USED
		.protocol_events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							),
		.event_sr = UART_0_TX_SR - 2,
		.event_irq = { .irq_num = USIC0_0_IRQn, .irq_priority = 50 },
	#endif
	#ifdef UART_TX_INTERRUPT_USED
		.tx_irq = { .irq_num = USIC0_2_IRQn, .irq_priority = 50 },
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		.rx_irq = { .irq_num = USIC0_1_IRQn, .irq_priority = 49 }
	#endif
	},
	#endif
	.fifo_cfg =
	{
		.tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_64WORDS,
		.rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_2WORDS
	},
	.channel_input = XMC_USIC_CH_INPUT_DX0,
	.channel_input_source = 1
};
/* UART Channel 0 Control Data Structure */
tStUART_Control UART_Channel_0_Control =
{
	.p_tx_data = NULL,
	.p_rx_data = NULL,
	.tx_data_index = 0,
	.tx_data_count = 0,
	.rx_data_index = 0,
	.rx_data_count = 0,
	.state = { .state_value = 0 }
};


#if( UART_CALLBACK_HANDLER_USED == 1U )
tStUART_Callbacks UART_Channel_0_Callbacks = { .fp_callback = &UART_callback_handler };
#endif	/* UART_CALLBACK_HANDLER_USED */

/* UART Channel 0 Instance Data Structure */
tStUART_Channel UART_Channel_0 =
{
	.p_config = &UART_Channel_0_Configuration,
	.p_control = &UART_Channel_0_Control
#if( UART_CALLBACK_HANDLER_USED == 1U )
	,.p_callback = &UART_Channel_0_Callbacks
#endif
};
#endif	/* UART_CHANNEL_0_USED */

/* UART Channel 1 usage */
#ifdef UART_CHANNEL_1_USED
/* UART Channel 0 Configurations Data Structure */
tStUART_Configuartion UART_Channel_1_Configuration =
{
	.p_channel_context = (XMC_USIC_CH_t *) XMC_UART0_CH1,
	.channel_cfg =
	{
		.baudrate = 115200,
		.data_bits = 8U,
		.frame_length = 8U,
		.stop_bits = 1U,
		.oversampling = 16U,
		.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
	},
	#if( UART_INTERRUPT_USED == 1U )
	.isr_config =
	{
		.tx_sr = UART_1_TX_SR,
		.rx_sr = UART_1_RX_SR,
	#ifdef UART_PROTOCOL_EVENT_USED
		.protocol_events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							),
		.event_sr = UART_1_TX_SR - 2,
		.event_irq = { .irq_num = USIC0_3_IRQn, .irq_priority = 58 },
	#endif
	#ifdef UART_TX_INTERRUPT_USED
		.tx_irq = { .irq_num = USIC0_5_IRQn, .irq_priority = 60 },
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		.rx_irq = { .irq_num = USIC0_4_IRQn, .irq_priority = 59 }
	#endif
	},
	#endif
	.fifo_cfg =
	{
		.tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_64WORDS,
		.rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_2WORDS
	},
	.channel_input = ,/* Update according to pins */
	.channel_input_source =
};
/* UART Channel 1 Control Data Structure */
tStUART_Control UART_Channel_1_Control =
{
	.p_tx_data = NULL,
	.p_rx_data = NULL,
	.tx_data_index = 0,
	.tx_data_count = 0,
	.rx_data_index = 0,
	.rx_data_count = 0,
	.state = { .state_value = 0 }
};


#if( UART_CALLBACK_HANDLER_USED == 1U )
tStUART_Callbacks UART_Channel_1_Callbacks = { .fp_callback = &UART_callback_handler };
#endif	/* UART_CALLBACK_HANDLER_USED */

/* UART Channel 1 Instance Data Structure */
tStUART_Channel UART_Channel_1 =
{
	.p_config = &UART_Channel_1_Configuration,
	.p_control = &UART_Channel_1_Control
#if( UART_CALLBACK_HANDLER_USED == 1U )
	,.p_callback = &UART_Channel_1_Callbacks
#endif
};
#endif	/* UART_CHANNEL_1_USED */


/* UART Channel 2 usage */
#ifdef UART_CHANNEL_2_USED
/* UART Channel 2 Configurations Data Structure */
tStUART_Configuartion UART_Channel_2_Configuration =
{
	.p_channel_context = (XMC_USIC_CH_t *) XMC_UART1_CH0,
	.channel_cfg =
	{
		.baudrate = 115200,
		.data_bits = 8U,
		.frame_length = 8U,
		.stop_bits = 1U,
		.oversampling = 16U,
		.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
	},
	#if( UART_INTERRUPT_USED == 1U )
	.isr_config =
	{
		.tx_sr = UART_2_TX_SR,
		.rx_sr = UART_2_RX_SR,
	#ifdef UART_PROTOCOL_EVENT_USED
		.protocol_events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							),
		.event_sr = UART_2_TX_SR - 2,
		.event_irq = { .irq_num = USIC1_0_IRQn, .irq_priority = 50 },
	#endif
	#ifdef UART_TX_INTERRUPT_USED
		.tx_irq = { .irq_num = USIC1_2_IRQn, .irq_priority = 48 },
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		.rx_irq = { .irq_num = USIC1_1_IRQn, .irq_priority = 47 }
	#endif
	},
	#endif
	.fifo_cfg =
	{
		.tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_64WORDS,
		.rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_2WORDS
	},
	.channel_input = XMC_USIC_CH_INPUT_DX0,
	.channel_input_source = 2
};

#if( UART_CALLBACK_HANDLER_USED == 1U )
tStUART_Callbacks UART_Channel_2_Callbacks = { .fp_callback = &UART_callback_handler };
#endif	/* UART_CALLBACK_HANDLER_USED */

/* UART Channel 2 Control Data Structure */
tStUART_Control UART_Channel_2_Control =
{
	.p_tx_data = NULL,
	.p_rx_data = NULL,
	.tx_data_index = 0,
	.tx_data_count = 0,
	.rx_data_index = 0,
	.rx_data_count = 0,
	.state = { .state_value = 0 }
};
/* UART Channel 2 Instance Data Structure */
tStUART_Channel UART_Channel_2 =
{
	.p_config = &UART_Channel_2_Configuration,
	.p_control = &UART_Channel_2_Control
#if( UART_CALLBACK_HANDLER_USED == 1U )
	,.p_callback = &UART_Channel_2_Callbacks
#endif
};
#endif	/* UART_CHANNEL_2_USED */


/* UART Channel 3 usage */
#ifdef UART_CHANNEL_3_USED
/* UART Channel 3 Configurations Data Structure */
tStUART_Configuartion UART_Channel_3_Configuration =
{
	.p_channel_context = (XMC_USIC_CH_t *) XMC_UART1_CH1,
	.channel_cfg =
	{
		.baudrate = 115200,
		.data_bits = 8U,
		.frame_length = 8U,
		.stop_bits = 1U,
		.oversampling = 16U,
		.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
	},
	#if( UART_INTERRUPT_USED == 1U )
	.isr_config =
	{
		.tx_sr = UART_3_TX_SR,
		.rx_sr = UART_3_RX_SR,
	#ifdef UART_PROTOCOL_EVENT_USED
		.protocol_events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							),
		.event_sr = UART_3_TX_SR - 2,
		.event_irq = { .irq_num = USIC1_3_IRQn, .irq_priority = 58 },
	#endif
	#ifdef UART_TX_INTERRUPT_USED
		.tx_irq = { .irq_num = USIC1_5_IRQn, .irq_priority = 60 },
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		.rx_irq = { .irq_num = USIC1_4_IRQn, .irq_priority = 59 }
	#endif
	},
	#endif
	.fifo_cfg =
	{
		.tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_64WORDS,
		.rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_2WORDS
	},
	.channel_input = XMC_USIC_CH_INPUT_DX0,
	.channel_input_source = 3
};

#if( UART_CALLBACK_HANDLER_USED == 1U )
tStUART_Callbacks UART_Channel_3_Callbacks = { .fp_callback = &UART_callback_handler };
#endif	/* UART_CALLBACK_HANDLER_USED */

/* UART Channel 3 Control Data Structure */
tStUART_Control UART_Channel_3_Control =
{
	.p_tx_data = NULL,
	.p_rx_data = NULL,
	.tx_data_index = 0,
	.tx_data_count = 0,
	.rx_data_index = 0,
	.rx_data_count = 0,
	.state = { .state_value = 0 }
};
/* UART Channel 3 Instance Data Structure */
tStUART_Channel UART_Channel_3 =
{
	.p_config = &UART_Channel_3_Configuration,
	.p_control = &UART_Channel_3_Control
#if( UART_CALLBACK_HANDLER_USED == 1U )
	,.p_callback = &UART_Channel_3_Callbacks
#endif
};
#endif	/* UART_CHANNEL_3_USED */


/* UART Channel 4 usage */
#ifdef UART_CHANNEL_4_USED
/* UART Channel 4 Configurations Data Structure */
tStUART_Configuartion UART_Channel_4_Configuration =
{
	.p_channel_context = (XMC_USIC_CH_t *) XMC_UART2_CH0,
	.channel_cfg =
	{
		.baudrate = 115200,
		.data_bits = 8U,
		.frame_length = 8U,
		.stop_bits = 1U,
		.oversampling = 16U,
		.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
	},
	#if( UART_INTERRUPT_USED == 1U )
	.isr_config =
	{
		.tx_sr = UART_4_TX_SR,
		.rx_sr = UART_4_RX_SR,
	#ifdef UART_PROTOCOL_EVENT_USED
		.protocol_events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							),
		.event_sr = UART_4_TX_SR - 2,
		.event_irq = { .irq_num = USIC2_0_IRQn, .irq_priority = 58 },
	#endif
	#ifdef UART_TX_INTERRUPT_USED
		.tx_irq = { .irq_num = USIC2_2_IRQn, .irq_priority = 60 },
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		.rx_irq = { .irq_num = USIC2_1_IRQn, .irq_priority = 59 }
	#endif
	},
	#endif
	.fifo_cfg =
	{
		.tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_64WORDS,
		.rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_2WORDS
	},
	.channel_input = XMC_USIC_CH_INPUT_DX0,
	.channel_input_source = 0
};

#if( UART_CALLBACK_HANDLER_USED == 1U )
tStUART_Callbacks UART_Channel_4_Callbacks = { .fp_callback = &UART_callback_handler };
#endif	/* UART_CALLBACK_HANDLER_USED */

/* UART Channel 4 Control Data Structure */
tStUART_Control UART_Channel_4_Control =
{
	.p_tx_data = NULL,
	.p_rx_data = NULL,
	.tx_data_index = 0,
	.tx_data_count = 0,
	.rx_data_index = 0,
	.rx_data_count = 0,
	.state = { .state_value = 0 }
};
/* UART Channel 4 Instance Data Structure */
tStUART_Channel UART_Channel_4 =
{
	.p_config = &UART_Channel_4_Configuration,
	.p_control = &UART_Channel_4_Control
#if( UART_CALLBACK_HANDLER_USED == 1U )
	,.p_callback = &UART_Channel_4_Callbacks
#endif
};
#endif	/* UART_CHANNEL_4_USED */


/* UART Channel 5 usage */
#ifdef UART_CHANNEL_5_USED
/* UART Channel 5 Configurations Data Structure */
tStUART_Configuartion UART_Channel_5_Configuration =
{
	.p_channel_context = (XMC_USIC_CH_t *) XMC_UART2_CH1,
	.channel_cfg =
	{
		.baudrate = 115200,
		.data_bits = 8U,
		.frame_length = 8U,
		.stop_bits = 1U,
		.oversampling = 16U,
		.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
	},
	#if( UART_INTERRUPT_USED == 1U )
	.isr_config =
	{
		.tx_sr = UART_5_TX_SR,
		.rx_sr = UART_5_RX_SR,
	#ifdef UART_PROTOCOL_EVENT_USED
		.protocol_events = ((unsigned long)XMC_UART_CH_EVENT_RECEIVER_NOISE |
							(unsigned long)XMC_UART_CH_EVENT_COLLISION |
							(unsigned long)XMC_UART_CH_EVENT_FORMAT_ERROR |
							(unsigned long)XMC_UART_CH_EVENT_SYNCHRONIZATION_BREAK
							),
		.event_sr = UART_5_TX_SR - 2,
		.event_irq = { .irq_num = USIC2_3_IRQn, .irq_priority = 58 },
	#endif
	#ifdef UART_TX_INTERRUPT_USED
		.tx_irq = { .irq_num = USIC2_5_IRQn, .irq_priority = 60 },
	#endif
	#ifdef UART_RX_INTERRUPT_USED
		.rx_irq = { .irq_num = USIC2_4_IRQn, .irq_priority = 59 }
	#endif
	},
	#endif
	.fifo_cfg =
	{
		.tx_fifo_size = XMC_USIC_CH_FIFO_SIZE_64WORDS,
		.rx_fifo_size = XMC_USIC_CH_FIFO_SIZE_2WORDS
	},
	.channel_input = ,	/* According to pins */
	.channel_input_source =
};

#if( UART_CALLBACK_HANDLER_USED == 1U )
tStUART_Callbacks UART_Channel_5_Callbacks = { .fp_callback = &UART_callback_handler };
#endif	/* UART_CALLBACK_HANDLER_USED */

/* UART Channel 4 Control Data Structure */
tStUART_Control UART_Channel_5_Control =
{
	.p_tx_data = NULL,
	.p_rx_data = NULL,
	.tx_data_index = 0,
	.tx_data_count = 0,
	.rx_data_index = 0,
	.rx_data_count = 0,
	.state = { .state_value = 0 }
};
/* UART Channel 5 Instance Data Structure */
tStUART_Channel UART_Channel_5 =
{
	.p_config = &UART_Channel_5_Configuration,
	.p_control = &UART_Channel_5_Control
#if( UART_CALLBACK_HANDLER_USED == 1U )
	,.p_callback = &UART_Channel_5_Callbacks
#endif
};
#endif	/* UART_CHANNEL_5_USED */


#endif	/* (UART_CHANNELS_USED > 0) */


/******************************************************************************
* Function Definitions
*******************************************************************************/


#if (defined(UART_TX_INTERRUPT_USED) ||\
	defined(UART_RX_INTERRUPT_USED) || \
	defined(UART_PROTOCOL_EVENT_USED))
extern void UART_isr_entry( void * p_channel, const eUART_InterruptOperation );
#endif


/******* ISR Handlers for UART channel 0 *******/

#ifdef UART_0_TX_HANDLER
/**
 * @function	UART_0_TX_HANDLER
 *
 * @brief		UART4 tx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_0_TX_HANDLER( void )
{
	/* Call ISR entry function */
	UART_isr_entry( &UART_Channel_0, UART_INTERRUPT_OPERATION_TX );
}
#endif	/* UART_0_TX_HANDLER */


#ifdef UART_0_RX_HANDLER
/**
 * @function	UART_0_RX_HANDLER
 *
 * @brief		UART4 rx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_0_RX_HANDLER( void )
{
	/* Call ISR entry function */
	UART_isr_entry( &UART_Channel_0, UART_INTERRUPT_OPERATION_RX );
}
#endif	/* UART_0_RX_HANDLER */


#ifdef UART_0_ERR_HANDLER
/**
 * @function	UART_0_ERR_HANDLER
 *
 * @brief		UART4 err handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_0_ERR_HANDLER( void )
{
	/* Call ISR entry function */
	UART_isr_entry( &UART_Channel_0, UART_INTERRUPT_OPERATION_ERROR );
}
#endif	/* UART_0_ERR_HANDLER */


/******* ISR Handlers for UART channel 2 *******/

#ifdef UART_2_TX_HANDLER
/**
 * @function	UART_2_TX_HANDLER
 *
 * @brief		UART2 tx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_2_TX_HANDLER( void )
{
	/* Call ISR entry function */
	UART_isr_entry( &UART_Channel_2, UART_INTERRUPT_OPERATION_TX );
}
#endif	/* UART_2_TX_HANDLER */


#ifdef UART_2_RX_HANDLER
/**
 * @function	UART_2_RX_HANDLER
 *
 * @brief		UART2 rx handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_2_RX_HANDLER( void )
{
	/* Call ISR entry function */
//	UART_isr_entry( &UART_Channel_2, UART_INTERRUPT_OPERATION_RX );/* Used for Logging */
}
#endif	/* UART_2_RX_HANDLER */


#ifdef UART_2_ERR_HANDLER
/**
 * @function	UART_2_ERR_HANDLER
 *
 * @brief		UART4 err handler
 *
 * <i>Imp Note:</i>
 *
 */
void UART_2_ERR_HANDLER( void )
{
	/* Call ISR entry function */
	UART_isr_entry( &UART_Channel_2, UART_INTERRUPT_OPERATION_ERROR );
}
#endif	/* UART_0_ERR_HANDLER */


/*********************************** End of File ******************************/
