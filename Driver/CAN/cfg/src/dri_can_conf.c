/*****************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		CAN Configuration Source
* Filename:		dri_can_conf.c
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_can_conf.c
 *  @brief:	This source file contains configuration function definitions for CAN
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_can.h>


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
* Private Function Declarations
*******************************************************************************/

#if (CAN_USING_INTERRUPT == 1U)
void CAN_ISR_Handler_Entry( void * p_node, const eCAN_EventType event_type, const eCAN_Event event );
#endif


/******************************************************************************
* Variables
*******************************************************************************/

/*
 * @brief	CAN API functions
 *
 * \par<b>Description:</b><br>
 * 			This is data structure for CAN API functions to be used by Application
 */


/*****************************************************************************/

/* Callback handler for user */
#if (CAN_CALLBACK_HANDLER_USED == 1U)
extern void CAN_callback_handler( void * p_channel, const eCAN_EventType event_type, const eCAN_Event event );
#endif


/** CAN Global Module Configuration **/
static tStCAN_GlobalModuleConfiguartion CAN_GlobalConfiguration =
{
	.p_module_context = CAN,
	.frequency = FCAN,
	.clock_source = XMC_CAN_CANCLKSRC_FPERI
};
static tStCAN_Control CAN_GlobalControl;


/* CAN Node 0 Configurations */
#if (CAN0_CHANNEL_USED == 1U)
static XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t CAN0_BitConfiguration =
{
	.can_frequency = FCAN,
	.baudrate = (500 * 1000),
	.sample_point = (80 * 100),
	.sjw = 2
};

static XMC_CAN_MO_t CAN0_MessageObject_Transmitter =
{
	.can_mo_ptr = CAN_MO0,

	/**< mo_ar */
	.can_identifier = 0x1111,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_priority = XMC_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1,

	/**< mo_amr */
	.can_id_mask = 0x7FF,
	.can_ide_mask = 1U,

	.can_data_length = 8,
	.can_data[0] = 0,
	.can_data[1] = 0,

	.can_mo_type = XMC_CAN_MO_TYPE_TRANSMSGOBJ
};

static XMC_CAN_MO_t CAN0_MessageObject_Receiver =
{
	.can_mo_ptr = CAN_MO1,

	/**< mo_ar */
	.can_identifier = 0x1111,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_priority = XMC_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1,

	/**< mo_amr */
	.can_id_mask = 0x7FF,
	.can_ide_mask = 1U,

	.can_data_length = 8,
	.can_data[0] = 0,
	.can_data[1] = 0,

	.can_mo_type = XMC_CAN_MO_TYPE_RECMSGOBJ
};

#if (CAN_USING_INTERRUPT == 1U)
tStCAN_InterruptConfiguartion CAN0_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_0_IRQn, .irq_priority = 0 },
	.sr = 0
};
tStCAN_InterruptConfiguartion CAN0_MO_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_1_IRQn, .irq_priority = 0 },
	.sr = 1
};
#if (CAN0_EVENTS_USED == 1U)
tStCAN_InterruptConfiguartion CAN0_Event_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_4_IRQn, .irq_priority = 0 },
	.sr = 4
};
XMC_CAN_NODE_EVENT_t CAN0_Events[CAN0_EVENTS] =
{
	XMC_CAN_NODE_EVENT_TX_INT,
	XMC_CAN_NODE_EVENT_ALERT,
	XMC_CAN_NODE_EVENT_LEC,
	XMC_CAN_NODE_EVENT_CFCIE
};
#endif	/* CAN0_EVENTS_USED */
#endif	/* CAN_USING_INTERRUPT */

/* CAN Node 0 Configuration Structure */
tStCAN_NodeConfiguartion CAN0_Node_Configuration =
{
	.p_channel_context = CAN_NODE0,
	.p_bit_time_config = &CAN0_BitConfiguration,
	.p_message_tx = &CAN0_MessageObject_Transmitter,
	.p_message_rx = &CAN0_MessageObject_Receiver,
	.message_tx_number = 0,
	.message_rx_number = 1
	#if (CAN_USING_INTERRUPT == 1U)
	,.isr_config = &CAN0_Interrupt_Configuration,
	.isr_mo_conifg = &CAN0_MO_Interrupt_Configuration
		#if (CAN0_EVENTS_USED == 1U)
		,.isr_event_conifg = &CAN0_Event_Interrupt_Configuration,
		.p_events = CAN0_Events,
		.events = 4
		#endif
	#endif
	,.loopback = XMC_CAN_LOOKBACKMODE_DISABLED,
	.rx_input_pin = 0
};

tStCAN_Control CAN0_Node_Control;

#if (CAN_CALLBACK_HANDLER_USED == 1U)
tStCAN_Callbacks CAN0_Node_Callback =
{
	.fp_callback = &CAN_callback_handler
};
#endif
#endif	/* CAN0_CHANNEL_USED */


/* CAN Node 1 Configurations */
#if (CAN1_CHANNEL_USED == 1U)
static XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t CAN1_BitConfiguration =
{
	.can_frequency = FCAN,
	.baudrate = (500 * 1000),
	.sample_point = (80 * 100),
	.sjw = 2
};

static XMC_CAN_MO_t CAN1_MessageObject_Transmitter =
{
	.can_mo_ptr = CAN_MO2,

	/**< mo_ar */
	.can_identifier = 0x1111,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_priority = XMC_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1,

	/**< mo_amr */
	.can_id_mask = 0x7FF,
	.can_ide_mask = 1U,

	.can_data_length = 8,
	.can_data[0] = 0,
	.can_data[1] = 0,

	.can_mo_type = XMC_CAN_MO_TYPE_TRANSMSGOBJ
};

static XMC_CAN_MO_t CAN1_MessageObject_Receiver =
{
	.can_mo_ptr = CAN_MO3,

	/**< mo_ar */
	.can_identifier = 0x2222,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_priority = XMC_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1,

	/**< mo_amr */
	.can_id_mask = 0x7FF,
	.can_ide_mask = 1U,

	.can_data_length = 8,
	.can_data[0] = 0,
	.can_data[1] = 0,

	.can_mo_type = XMC_CAN_MO_TYPE_RECMSGOBJ
};

#if (CAN_USING_INTERRUPT == 1U)
tStCAN_InterruptConfiguartion CAN1_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_2_IRQn, .irq_priority = 49 },
	.sr = 2
};
tStCAN_InterruptConfiguartion CAN1_MO_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_3_IRQn, .irq_priority = 49 },
	.sr = 3
};
#if (CAN1_EVENTS_USED == 1U)
tStCAN_InterruptConfiguartion CAN1_Event_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_4_IRQn, .irq_priority = 49 },
	.sr = 4
};
XMC_CAN_NODE_EVENT_t CAN1_Events[CAN1_EVENTS] =
{
	XMC_CAN_NODE_EVENT_TX_INT,
	XMC_CAN_NODE_EVENT_ALERT,
	XMC_CAN_NODE_EVENT_LEC,
	XMC_CAN_NODE_EVENT_CFCIE
};
#endif	/* CAN1_EVENTS_USED */
#endif	/* CAN_USING_INTERRUPT */

/* CAN Node 1 Configuration Structure */
tStCAN_NodeConfiguartion CAN1_Node_Configuration =
{
	.p_channel_context = CAN_NODE1,
	.p_bit_time_config = &CAN1_BitConfiguration,
	.p_message_tx = &CAN1_MessageObject_Transmitter,
	.p_message_rx = &CAN1_MessageObject_Receiver,
	.message_tx_number = 2,
	.message_rx_number = 3
	#if (CAN_USING_INTERRUPT == 1U)
	,.isr_config = &CAN1_Interrupt_Configuration,
	.isr_mo_conifg = &CAN1_MO_Interrupt_Configuration
		#if (CAN1_EVENTS_USED == 1U)
		,.isr_event_conifg = &CAN1_Event_Interrupt_Configuration,
		.p_events = CAN1_Events,
		.events = 4
		#endif
	#endif
	,.loopback = XMC_CAN_LOOKBACKMODE_ENABLED,
	.rx_input_pin = XMC_CAN_NODE_RECEIVE_INPUT_RXDCC
};

tStCAN_Control CAN1_Node_Control;

#if (CAN_CALLBACK_HANDLER_USED == 1U)
tStCAN_Callbacks CAN1_Node_Callback =
{
	.fp_callback = &CAN_callback_handler
};
#endif
#endif	/* CAN1_CHANNEL_USED */


/* CAN Node 2 Configurations */
#if (CAN2_CHANNEL_USED == 1U)
static XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t CAN2_BitConfiguration =
{
	.can_frequency = FCAN,
	.baudrate = (500 * 1000),
	.sample_point = (80 * 100),
	.sjw = 2
};

static XMC_CAN_MO_t CAN2_MessageObject_Transmitter =
{
	.can_mo_ptr = CAN_MO4,

	/**< mo_ar */
	.can_identifier = 0x2222,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_priority = XMC_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1,

	/**< mo_amr */
	.can_id_mask = 0x7FF,
	.can_ide_mask = 1U,

	.can_data_length = 8,
	.can_data[0] = 0,
	.can_data[1] = 0,

	.can_mo_type = XMC_CAN_MO_TYPE_TRANSMSGOBJ
};

static XMC_CAN_MO_t CAN2_MessageObject_Receiver =
{
	.can_mo_ptr = CAN_MO5,

	/**< mo_ar */
	.can_identifier = 0x1111,
	.can_id_mode = XMC_CAN_FRAME_TYPE_STANDARD_11BITS,
	.can_priority = XMC_CAN_ARBITRATION_MODE_ORDER_BASED_PRIO_1,

	/**< mo_amr */
	.can_id_mask = 0x7FF,
	.can_ide_mask = 1U,

	.can_data_length = 8,
	.can_data[0] = 0,
	.can_data[1] = 0,

	.can_mo_type = XMC_CAN_MO_TYPE_RECMSGOBJ
};

#if (CAN_USING_INTERRUPT == 1U)
tStCAN_InterruptConfiguartion CAN2_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_5_IRQn, .irq_priority = 48 },
	.sr = 5
};
tStCAN_InterruptConfiguartion CAN2_MO_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_6_IRQn, .irq_priority = 48 },
	.sr = 6
};
#if (CAN2_EVENTS_USED == 1U)
tStCAN_InterruptConfiguartion CAN2_Event_Interrupt_Configuration =
{
	.irq = { .irq_num = CAN0_7_IRQn, .irq_priority = 48 },
	.sr = 7
};
XMC_CAN_NODE_EVENT_t CAN2_Events[CAN2_EVENTS] =
{
	XMC_CAN_NODE_EVENT_TX_INT,
	XMC_CAN_NODE_EVENT_ALERT,
	XMC_CAN_NODE_EVENT_LEC,
	XMC_CAN_NODE_EVENT_CFCIE
};
#endif	/* CAN2_EVENTS_USED */
#endif	/* CAN_USING_INTERRUPT */

/* CAN Node 2 Configuration Structure */
tStCAN_NodeConfiguartion CAN2_Node_Configuration =
{
	.p_channel_context = CAN_NODE2,
	.p_bit_time_config = &CAN2_BitConfiguration,
	.p_message_tx = &CAN2_MessageObject_Transmitter,
	.p_message_rx = &CAN2_MessageObject_Receiver,
	.message_tx_number = 4,
	.message_rx_number = 5
	#if (CAN_USING_INTERRUPT == 1U)
	,.isr_config = &CAN2_Interrupt_Configuration,
	.isr_mo_conifg = &CAN2_MO_Interrupt_Configuration
		#if (CAN2_EVENTS_USED == 1U)
		,.isr_event_conifg = &CAN2_Event_Interrupt_Configuration,
		.p_events = CAN2_Events,
		.events = 4
		#endif
	#endif
	,.loopback = XMC_CAN_LOOKBACKMODE_ENABLED,
	.rx_input_pin = XMC_CAN_NODE_RECEIVE_INPUT_RXDCA
};

tStCAN_Control CAN2_Node_Control;

#if (CAN_CALLBACK_HANDLER_USED == 1U)
tStCAN_Callbacks CAN2_Node_Callback =
{
	.fp_callback = &CAN_callback_handler
};
#endif
#endif	/* CAN2_CHANNEL_USED */


/*****************************************************************************/


/** CAN Global Handle **/
tStCAN_GlobalModule CAN_Global =
{
	.p_module_config =  &CAN_GlobalConfiguration,
	.p_control = &CAN_GlobalControl
};

/** CAN Node Handles **/

/** CAN Node Handles **/
#if (CAN0_CHANNEL_USED == 1U)
tStCAN_Node CAN0 =
{
	.p_config = &CAN0_Node_Configuration,
	.p_control = &CAN0_Node_Control
	#if (CAN_CALLBACK_HANDLER_USED == 1U)
	,.p_callback = &CAN0_Node_Callback
	#endif
};
#endif
#if (CAN1_CHANNEL_USED == 1U)
tStCAN_Node CAN1 =
{
	.p_config = &CAN1_Node_Configuration,
	.p_control = &CAN1_Node_Control
	#if (CAN_CALLBACK_HANDLER_USED == 1U)
	,.p_callback = &CAN1_Node_Callback
	#endif
};
#endif
#if (CAN2_CHANNEL_USED == 1U)
tStCAN_Node CAN2 =
{
	.p_config = &CAN2_Node_Configuration,
	.p_control = &CAN2_Node_Control
	#if (CAN_CALLBACK_HANDLER_USED == 1U)
	,.p_callback = &CAN2_Node_Callback
	#endif
};
#endif



/******************************************************************************
* Function Definitions
*******************************************************************************/

/******* ISR Handlers for CAN nodes *******/

#if (CAN0_CHANNEL_USED == 1U)
void CAN0_Node_Transfer_Handler( void )
{
	unsigned long status_l = XMC_CAN_NODE_GetStatus( (XMC_CAN_NODE_t *const)CAN0.p_config->p_channel_context );
	if( status_l & CAN_NODE_NSR_TXOK_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN0, CAN_EVENT_TYPE_NODE_TRANSFER, CAN_EVENT_NODE_TRANSFER_TRANSMIT );
	}

	if( status_l & CAN_NODE_NSR_RXOK_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN0, CAN_EVENT_TYPE_NODE_TRANSFER, CAN_EVENT_NODE_TRANSFER_RECEIVE );
	}
}
void CAN0_MessageObject_Handler( void )
{
	/* Check transmit or receive event for CAN Node MO's */
	unsigned long status_l = XMC_CAN_MO_GetStatus( CAN0.p_config->p_message_rx );
	if( status_l & (CAN_MO_MOSTAT_RXPND_Msk|CAN_MO_MOSTAT_MSGVAL_Msk) )/* Message Validity and reception */
	{
		CAN_ISR_Handler_Entry( &CAN0, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_MESSAGE_RECEIVE );
	}

	/* Transmitter check */
	status_l = XMC_CAN_MO_GetStatus( CAN0.p_config->p_message_tx );
	if( status_l & (CAN_MO_MOSTAT_TXPND_Msk|CAN_MO_MOSTAT_MSGVAL_Msk) )/* Message Validity and transmission */
	{
		CAN_ISR_Handler_Entry( &CAN0, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_MESSAGE_TRANSMIT );
	}
}
#if (CAN0_EVENTS_USED == 1U)
void CAN0_Event_Handler( void )
{
}
#endif	/* CAN0_EVENTS_USED */
#endif	/* CAN0_CHANNEL_USED */

/*****************************************************************************/

#if (CAN1_CHANNEL_USED == 1U)
void CAN1_Node_Transfer_Handler( void )
{
	unsigned long status_l = XMC_CAN_NODE_GetStatus( (XMC_CAN_NODE_t *const)CAN1.p_config->p_channel_context );
	if( status_l & CAN_NODE_NSR_TXOK_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN1, CAN_EVENT_TYPE_NODE_TRANSFER, CAN_EVENT_NODE_TRANSFER_TRANSMIT );
	}

	if( status_l & CAN_NODE_NSR_RXOK_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN1, CAN_EVENT_TYPE_NODE_TRANSFER, CAN_EVENT_NODE_TRANSFER_RECEIVE );
	}
}

void CAN1_MessageObject_Handler( void )
{
	/* Check transmit or receive event for CAN Node MO's */
	unsigned long status_l = XMC_CAN_MO_GetStatus( CAN1.p_config->p_message_rx );
	if( status_l & (CAN_MO_MOSTAT_RXPND_Msk|CAN_MO_MOSTAT_MSGVAL_Msk) )/* Message Validity and reception */
	{
		CAN_ISR_Handler_Entry( &CAN1, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_MESSAGE_RECEIVE );
	}

	/* Transmitter check */
	status_l = XMC_CAN_MO_GetStatus( CAN1.p_config->p_message_tx );
	if( status_l & (CAN_MO_MOSTAT_TXPND_Msk|CAN_MO_MOSTAT_MSGVAL_Msk) )/* Message Validity and transmission */
	{
		CAN_ISR_Handler_Entry( &CAN1, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_MESSAGE_TRANSMIT );
	}
}

#if (CAN1_EVENTS_USED == 1U)
void CAN1_Event_Handler( void )
{
	unsigned long status_l = XMC_CAN_NODE_GetStatus( (XMC_CAN_NODE_t *const)CAN1.p_config->p_channel_context );
	/* Check status */
	if( status_l & CAN_NODE_NSR_ALERT_Msk )
	{
		/* Need to check the Alert levels */
		CAN_ISR_Handler_Entry( &CAN1, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_NODE_ALERT );
	}

	if( status_l & CAN_NODE_NSR_LEC_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN1, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_NODE_LEC );
	}
}
#endif	/* CAN1_EVENTS_USED */
#endif	/* CAN1_CHANNEL_USED */

/*****************************************************************************/

#if (CAN2_CHANNEL_USED == 1U)
void CAN2_Node_Transfer_Handler( void )
{
	unsigned long status_l = XMC_CAN_NODE_GetStatus( (XMC_CAN_NODE_t *const)CAN2.p_config->p_channel_context );
	if( status_l & CAN_NODE_NSR_TXOK_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN2, CAN_EVENT_TYPE_NODE_TRANSFER, CAN_EVENT_NODE_TRANSFER_TRANSMIT );
	}

	if( status_l & CAN_NODE_NSR_RXOK_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN2, CAN_EVENT_TYPE_NODE_TRANSFER, CAN_EVENT_NODE_TRANSFER_RECEIVE );
	}
}

void CAN2_MessageObject_Handler( void )
{
	/* Check transmit or receive event for CAN Node MO's */
	unsigned long status_l = XMC_CAN_MO_GetStatus( CAN2.p_config->p_message_rx );
	if( status_l & (CAN_MO_MOSTAT_RXPND_Msk|CAN_MO_MOSTAT_MSGVAL_Msk) )/* Message Validity and reception */
	{
		CAN_ISR_Handler_Entry( &CAN2, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_MESSAGE_RECEIVE );
	}

	/* Transmitter check */
	status_l = XMC_CAN_MO_GetStatus( CAN2.p_config->p_message_tx );
	if( status_l & (CAN_MO_MOSTAT_TXPND_Msk|CAN_MO_MOSTAT_MSGVAL_Msk) )/* Message Validity and transmission */
	{
		CAN_ISR_Handler_Entry( &CAN2, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_MESSAGE_TRANSMIT );
	}
}
#if (CAN2_EVENTS_USED == 1U)
void CAN2_Event_Handler( void )
{
	unsigned long status_l = XMC_CAN_NODE_GetStatus( (XMC_CAN_NODE_t *const)CAN2.p_config->p_channel_context );
	/* Check status */
	if( status_l & CAN_NODE_NSR_ALERT_Msk )
	{
		/* Need to check the Alert levels */
		CAN_ISR_Handler_Entry( &CAN2, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_NODE_ALERT );
	}

	if( status_l & CAN_NODE_NSR_LEC_Msk )
	{
		CAN_ISR_Handler_Entry( &CAN2, CAN_EVENT_TYPE_MO_TRANSFER, CAN_EVENT_NODE_LEC );
	}
}
#endif	/* CAN2_EVENTS_USED */
#endif	/* CAN2_CHANNEL_USED */


/*****************************************************************************/


#if (CAN_USING_INTERRUPT == 1U)
/** Channels ISR Entries **/
void CAN_ISR_Handler_Entry( void * p_channel, const eCAN_EventType event_type, const eCAN_Event event )
{
	DRIVER_ASSERT_NRET( NULL == p_channel );
	/* Local Variables */
	XMC_CAN_NODE_t * const p_node_l = (XMC_CAN_NODE_t *) ((tStCAN_Node *)p_channel)->p_config->p_channel_context;

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
			#if (CAN_CALLBACK_HANDLER_USED == 1U)
				if( ((tStCAN_Node *)p_channel)->p_callback->fp_callback != NULL )
				{
					((tStCAN_Node *)p_channel)->p_callback->fp_callback( p_channel, event_type, event );
				}
			#endif
				/* Clear TX status */
				XMC_CAN_NODE_ClearStatus( p_node_l, XMC_CAN_NODE_STATUS_TX_OK );
			break;
			case CAN_EVENT_NODE_TRANSFER_RECEIVE:
			#if (CAN_CALLBACK_HANDLER_USED == 1U)
				if( ((tStCAN_Node *)p_channel)->p_callback->fp_callback != NULL )
				{
					((tStCAN_Node *)p_channel)->p_callback->fp_callback( p_channel, event_type, event );
				}
			#endif
				/* Clear RX status */
				XMC_CAN_NODE_ClearStatus( p_node_l, XMC_CAN_NODE_STATUS_RX_OK );
			break;
		}
		break;	/* CAN_EVENT_NODE_TRANSFER_TRANSMIT */

		case CAN_EVENT_TYPE_NODE_ERROR:
			/* Check Node Error Event */
			switch( event )
			{
			default:
				case CAN_EVENT_NODE_ALERT:
					/* Check Bus-Off */
					if( XMC_CAN_NODE_GetStatus( p_node_l ) & CAN_NODE_NSR_BOFF_Msk )
					{
						XMC_CAN_NODE_ClearStatus( p_node_l, XMC_CAN_NODE_STATUS_BUS_OFF );
					}

					XMC_CAN_NODE_ClearStatus( p_node_l, XMC_CAN_NODE_STATUS_ALERT_WARNING );
				break;

				case CAN_EVENT_NODE_LEC:
					XMC_CAN_NODE_ClearStatus( p_node_l, XMC_CAN_NODE_STATUS_LAST_ERROR_CODE );
				break;

				case CAN_EVENT_NODE_CFCIE:
				break;
			}
		break;	/* CAN_EVENT_TYPE_NODE_ERROR */

		case CAN_EVENT_TYPE_MO_TRANSFER:
			/* Check MO Event */
			switch( event )
			{
			default:
				case CAN_EVENT_MESSAGE_TRANSMIT:
				break;

				case CAN_EVENT_MESSAGE_RECEIVE:
					/* Handle received message */
				break;

				case CAN_EVENT_MESSAGE_OVERFLOW:
				break;
			}
		break;	/* CAN_EVENT_TYPE_MO_TRANSFER */
	}
}
#endif	/* CAN_USING_INTERRUPT */

/*********************************** End of File ******************************/
