/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		CAN data types Header
* Filename:		dri_can_types.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_can_types.h
 *  @brief:	This file contains data types definitions for CAN
 */
#ifndef DRI_CAN_TYPES_H_
#define DRI_CAN_TYPES_H_


/******************************************************************************
* Includes
*******************************************************************************/

/* Include XMC Headers */
#include <xmc_can.h>

/* Include private headers first */
#include <private/dri_can_conf.h>
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
 * @brief	Enum for Status of CAN
 * */
typedef enum eCAN_Status_
{
	CAN_STATUS_SUCCESS = 0,
	CAN_STATUS_FAILED,
	CAN_STATUS_BUSY,
	CAN_STATUS_INVALID_ARGUMENT,
	CAN_STATUS_OUT_OF_BOUND,
	CAN_STATUS_NOT_INITIALIZED,
	CAN_STATUS_ALREADY_INITIALIZED,
	CAN_STATUS_NOT_ACCEPTABLE
} eCAN_Status;

/*
 * @brief	Enum for Open or close
 * */
typedef enum eCAN_Init_
{
	CAN_INIT_CLOSE = 0,
	CAN_INIT_OPEN
} eCAN_Init;

/*
 * @brief	Enum for CAN operation type
 * */
typedef enum eCAN_OperationType_
{
	CAN_OPERATION_TYPE_TRANSMIT = 0,
	CAN_OPERATION_TYPE_RECEIVE
} eCAN_OperationType;

/*
 * @brief	Enum for Event
 * */
typedef enum eCAN_Event_
{
	CAN_EVENT_NODE_TRANSFER_TRANSMIT = 0,/**< CAN Node Transfer Transmit */
	CAN_EVENT_NODE_TRANSFER_RECEIVE,/**< CAN Node Transfer Receive */
	CAN_EVENT_NODE_ALERT,			/**< CAN Node Alert */
	CAN_EVENT_NODE_LEC,				/**< CAN Node Last Error Code */
	CAN_EVENT_NODE_CFCIE,			/**< CAN Node Frame Count Event */
	CAN_EVENT_MESSAGE_TRANSMIT,		/**< CAN MO Transmit */
	CAN_EVENT_MESSAGE_RECEIVE,		/**< CAN MO Receive */
	CAN_EVENT_MESSAGE_OVERFLOW,		/**< CAN MO Overflow */
	CAN_EVENT_BUS_OFF
} eCAN_Event;

/*
 * @brief	Enum for Event type
 * */
typedef enum eCAN_EventType_
{
	CAN_EVENT_TYPE_NODE_TRANSFER = 0,	/**< CAN Node Transfer */
	CAN_EVENT_TYPE_NODE_ERROR,
	CAN_EVENT_TYPE_MO_TRANSFER
} eCAN_EventType;

/*
 * @brief	typedef for callback function pointer
 * */
typedef void (*can_callback_pointer) ( void * p_channel, const eCAN_EventType event_type, const eCAN_Event event );


/******************************************************************************
 *
 * @brief	Data Types for CAN instance
 *
 *****************************************************************************/

/*
 * @brief	typedef for status control of Global CAN
 */
typedef struct tStCAN_GlobalStatusControl_
{
	union
	{
		struct
		{
			volatile unsigned long init	: 1;/* Might not work */
			unsigned long state_res		: 31;
		};
		unsigned long state_value;
	};
} tStCAN_GlobalStatusControl;


/*
 * @brief	typedef for status control of CAN
 */
typedef struct tStCAN_StatusControl_
{
	union
	{
		struct
		{
			volatile unsigned long init		: 1;/* Might not work */
			volatile unsigned long tx_busy	: 1;
			volatile unsigned long rx_busy	: 1;
			unsigned long node				: 2;
			unsigned long state_res			: 27;
		};
		unsigned long state_value;
	};
} tStCAN_StatusControl;

/************************************************************************************/

/*
 * @brief Structure for CAN channel interrupt configuration
 */
typedef struct tStCAN_InterruptConfiguartion_
{
	/**< Interrupt configuration */
	tStInterruptConfig irq;
	/**< Service request number assigned to interrupt */
	unsigned long sr;
} tStCAN_InterruptConfiguartion;


/************************************************************************************/


/************************************************************************************/

/*
 * @brief	Structure for CAN Module Configuration
 */
typedef struct tStCAN_GlobalModuleConfiguartion_
{
	/**< Module Context for CAN module */
	void * p_module_context;
	unsigned long frequency;
	XMC_CAN_CANCLKSRC_t clock_source;
} tStCAN_GlobalModuleConfiguartion;


/*
 * @brief	Structure for CAN Node Configuration
 */
typedef struct tStCAN_NodeConfiguartion_
{
	/**< Context for CAN Node */
	void * p_channel_context;
	/**< Can Node Bit timing configuration */
	XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t * p_bit_time_config;
	/**< CAN Message Object list pointer */
	XMC_CAN_MO_t * p_message_tx;
	XMC_CAN_MO_t * p_message_rx;
	unsigned short message_tx_number;
	unsigned short message_rx_number;
	#if (CAN_USING_INTERRUPT == 1U)
	tStCAN_InterruptConfiguartion * isr_config;
	tStCAN_InterruptConfiguartion * isr_mo_conifg;
		#if (CAN_USING_EVENTS == 1U)
		tStCAN_InterruptConfiguartion * isr_event_conifg;
		long * p_events;
		unsigned char events;
		#endif
	#endif
	/**< loop back mode or normal mode */
	XMC_CAN_LOOKBACKMODE_t loopback;
	XMC_CAN_NODE_RECEIVE_INPUT_t rx_input_pin;
} tStCAN_NodeConfiguartion;


/*
 * @brief	Structure for CAN Control
 */
typedef struct tStCAN_Control_
{
	tStCAN_StatusControl state;
} tStCAN_Control;

/*
 * @brief Structure for CAN channel callbacks
 */
typedef struct tStCAN_Callbacks_
{
	can_callback_pointer fp_callback;
} tStCAN_Callbacks;

/*
 * @brief Structure for CAN channel API
 */
typedef struct tStCAN_Api_
{
	long (*fp_init) ( void * );
} tStCAN_Api;


/*
 * @brief Structure for CAN Global Module
 */
typedef struct tStCAN_GlobalModule_
{
	/**< Configurations for Module */
	tStCAN_GlobalModuleConfiguartion * p_module_config;
	/* Can add other parameters for control */
	tStCAN_GlobalStatusControl * p_control;
} tStCAN_GlobalModule;


/*
 * @brief Structure for CAN Node
 */
typedef struct tStCAN_Node_
{
	/**< Configurations for CAN Node */
	tStCAN_NodeConfiguartion * p_config;
	/* Can add other parameters for control */
	tStCAN_Control * p_control;
	#if (CAN_CALLBACK_HANDLER_USED == 1U)
	tStCAN_Callbacks * p_callback;
	#endif
} tStCAN_Node;


/************************************************************************************/

/*
 * @brief	Structure for CAN Data Type
 */
typedef struct tStCAN_DataBytes_
{
	union{
		unsigned char data_byte[8];
		unsigned short data_word[4];
		unsigned long data_dword[2];
		unsigned long long data_qword;
	};
} tStCAN_DataBytes;


/*
 * @brief	Structure for CAN Data Transfer
 */
typedef struct tStCAN_Data_
{
	/**< Node Context for CAN Data */
	tStCAN_Node * p_node;
	tStCAN_DataBytes * p_data;
	unsigned char data_length;
} tStCAN_Data;


#endif /* DRI_CAN_TYPES_H_ */

/********************************** End of File *******************************/
