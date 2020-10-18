/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART data types Header
* Filename:		dri_uart_types.h
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_uart_types.h
 *  @brief:	This file contains data types definitions for UART channel on USIC
 */
#ifndef DRI_UART_TYPES_H_
#define DRI_UART_TYPES_H_


/******************************************************************************
* Includes
*******************************************************************************/
/* Include XMC Headers */
#include <xmc_uart.h>

/* Include private headers first */
#include <private/dri_uart_conf.h>

/* Include typedefs */
#include <dri_types.h>


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

/*
 * @brief	Enum for Status of UART
 * */
typedef enum eUART_Status_
{
	UART_STATUS_SUCCESS = 0,
	UART_STATUS_FAILED,
	UART_STATUS_BUSY,
	UART_STATUS_INVALID_ARGUMENT,
	UART_STATUS_OUT_OF_BOUND,
	UART_STATUS_NOT_INITIALIZED
} eUART_Status;

/*
 * @brief	Enum for Open or close
 * */
typedef enum eUART_Init_
{
	UART_INIT_CLOSE = 0,
	UART_INIT_OPEN
} eUART_Init;


/*
 * @brief	Enum for communication mode
 * */
typedef enum eUART_ComMode_
{
	UART_MODE_DIRECT = 0,
	UART_MODE_INTERRUPT
} eUART_ComMode;

/*
 * @brief	Enum for Operating Mode of UART
 * */
typedef enum eUART_Mode_
{
	UART_MODE_FULL_DUPLEX = 0,
	UART_MODE_HALF_DUPLEX,
	UART_MODE_LOOPBACK
} eUART_Mode;

/*
 * @brief	Enum for Operation Events of UART
 * */
typedef enum eUART_Event_
{
	UART_EVENT_SYNC_BREAK_DETECT = 0,
	UART_EVENT_COLLISION_DETECT,
	UART_EVENT_RECEIVER_NOISE_DETECT,
	UART_EVENT_FORMAT_ERROR_0,
	UART_EVENT_FORMAT_ERROR_1,
	UART_EVENT_MAX,	/* Used as counter for Max error possibilities */
	UART_EVENT_RX_CHAR,
	UART_EVENT_TX_COMPLETE,
	UART_EVENT_RX_COMPLETE
} eUART_Event;

/*
 * @brief	Enum for UART ISR operation
 * */
typedef enum eUART_InterruptOperation_
{
	UART_INTERRUPT_OPERATION_TX = 0,
	UART_INTERRUPT_OPERATION_RX,
	UART_INTERRUPT_OPERATION_ERROR
} eUART_InterruptOperation;

/*
 * @brief	Enum for Abort operation
 * */
typedef enum eUART_Abort_
{
	UART_ABORT_TX = 0,
	UART_ABORT_RX,
	UART_ABORT_TX_RX
} eUART_Abort;

/*
 * @brief	Enum for UART Channel Information
 * */
typedef enum eUART_Channel_
{
	UART_CHANNEL_0 = 0,
	UART_CHANNEL_1,
	UART_CHANNEL_2,
	UART_CHANNEL_3,
	UART_CHANNEL_4,
	UART_CHANNEL_5
} eUART_Channel;


/*
 * @brief	typedef for callback function pointer
 * */
typedef void (*uart_callback_pointer) ( const eUART_Channel channel, unsigned char *p_data, const eUART_Event event );


/******************************************************************************
 *
 * @brief	Data Types for UART instance
 *
 *****************************************************************************/

/*
 * @brief	typedef for status control of UART
 */
typedef struct tStUART_StatusControl_
{
	union
	{
		struct
		{
			unsigned long init		: 1;
			unsigned long tx_lock	: 1;
			unsigned long tx_busy	: 1;
			unsigned long rx_lock	: 1;
			unsigned long rx_busy	: 1;
			unsigned long mode		: 1;	/* Supports Direct and Interrupt mode
											0 - Direct mode
											1 - Interrupt mode
			 	 	 	 	 	 	 	 	DAM is not supported in this feature */
			unsigned long state_res	: 26;
		};
		unsigned long state_value;
	};
} tStUART_StatusControl;

/*
 * @brief	Structure for UART control
 */
typedef struct tStUART_Control_
{
	/**< Pointer to the transmit data buffer */
	unsigned char * p_tx_data;
	/**< Pointer to the receive data buffer */
	unsigned char * p_rx_data;
	unsigned long tx_data_count;			/**< Transmit data count */
	unsigned long tx_data_index;			/**< Transmit data index */
	unsigned long rx_data_count;			/**< Receive data count */
	unsigned long rx_data_index;			/**< Receive data index */
	volatile tStUART_StatusControl state;	/**< UART data control state */
} tStUART_Control;

/*
 * @brief Structure for UART channel interrupt configuration
 */
typedef struct tStUART_InterruptConfiguartion_
{
	/**< Service request number assigned to transmit interrupt */
	unsigned long tx_sr;
	unsigned long rx_sr;
	/**< Interrupt configuration for tx, rx and events */
#ifdef UART_PROTOCOL_EVENT_USED
	unsigned long protocol_events;
	unsigned long event_sr;
	tStInterruptConfig event_irq;
#endif
#ifdef UART_TX_INTERRUPT_USED
	tStInterruptConfig tx_irq;
#endif
#ifdef UART_RX_INTERRUPT_USED
	tStInterruptConfig rx_irq;
#endif
} tStUART_InterruptConfiguartion;

/*
 * @brief Structure for UART channel fifo size configuration
 */
typedef struct tStUART_FIFO_Configuartion_
{
	/* fifo size configuration for tx and rx */
	XMC_USIC_CH_FIFO_SIZE_t tx_fifo_size;
	XMC_USIC_CH_FIFO_SIZE_t rx_fifo_size;
} tStUART_FIFO_Configuartion;

/*
 * @brief Structure for UART channel configuration
 */
typedef struct tStUART_Configuartion_
{
	/**< Pointer to channel context */
	void * p_channel_context;
	/**< pointer to configuration structure */
	XMC_UART_CH_CONFIG_t channel_cfg;
	/**< Channel interrupt configuration */
#if( UART_INTERRUPT_USED == 1U )
	tStUART_InterruptConfiguartion isr_config;
#endif
	/**< Channel FIFO configuration */
	tStUART_FIFO_Configuartion fifo_cfg;
	XMC_USIC_CH_INPUT_t channel_input;
	unsigned char channel_input_source;
} tStUART_Configuartion;

/*
 * @brief Structure for UART channel configuration
 */
typedef struct tStUART_Api_
{
	void (*fp_init)( void * p_channel, const unsigned char );
	long (*fp_set_baud)( void * p_channel, const unsigned long );
	long (*fp_transmit)( void * p_channel, const unsigned char *, const unsigned long );
	long (*fp_receive)( void * p_channel, unsigned char * const, const unsigned long );
	long (*fp_abort)( void * p_channel, const unsigned char );
} tStUART_Api;

/*
 * @brief Structure for UART channel callbacks
 */
typedef struct tStUART_Callbacks_
{
#if (UART_CALLBACK_HANDLER_USED == 1U)
	uart_callback_pointer fp_callback;
#endif
} tStUART_Callbacks;

/*
 * @brief Structure for UART channel instance and information
 */
typedef struct tStUART_Channel_
{
	tStUART_Configuartion * const p_config;
	tStUART_Control * const p_control;
#if (UART_CALLBACK_HANDLER_USED == 1U)
	tStUART_Callbacks * p_callback;
#endif
} tStUART_Channel;


#endif /* DRI_UART_TYPES_H_ */

/********************************** End of File *******************************/
