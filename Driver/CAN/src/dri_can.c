/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		CAN API Source
* Filename:		dri_can.c
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_can.c
 *  @brief:	This header file contains API function definitions for CAN
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
* Variables
*******************************************************************************/


/******************************************************************************
* Private Function Declarations
*******************************************************************************/

static long node_transfer( tStCAN_Data * p_data, const eCAN_OperationType type );


/******************************************************************************
* Public Function Definitions
*******************************************************************************/

/**
 * @function
 *
 * @brief
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_module_init( void * p_module, const unsigned char init )
{
	DRIVER_ASSERT( NULL == p_module, CAN_STATUS_INVALID_ARGUMENT );
	/* Local Variables */
	tStCAN_GlobalModuleConfiguartion * const p_config_l = ((tStCAN_GlobalModule *)p_module)->p_module_config;
	tStCAN_GlobalStatusControl * const p_control_l = ((tStCAN_GlobalModule *)p_module)->p_control;
	unsigned long frequency_l = 0;
	eCAN_Status ret_status = CAN_STATUS_SUCCESS;

	/* Check initialization */
	switch( init )
	{
		case CAN_INIT_OPEN:
			frequency_l = XMC_CAN_InitEx( (XMC_CAN_t *const) p_config_l->p_module_context,
									p_config_l->clock_source, p_config_l->frequency );
			/* Cross check the frequency */
			if( frequency_l == p_config_l->frequency )
			{
				/* Update initialization status */
				p_control_l->init = 1;
			}
			else
			{
				ret_status = CAN_STATUS_FAILED;
			}
			break;

		case CAN_INIT_CLOSE:
			/* Disable CAN */
			XMC_CAN_Disable( (XMC_CAN_t *const) p_config_l->p_module_context );
			/* Update initialization status */
			p_control_l->init = 0;
			break;

		default:
			ret_status = CAN_STATUS_INVALID_ARGUMENT;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function
 *
 * @brief
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_node_init( void * p_channel, const unsigned char node, const unsigned char init )
{
	DRIVER_ASSERT( NULL == p_channel, CAN_STATUS_INVALID_ARGUMENT );
	/* Local Variables */
	tStCAN_NodeConfiguartion * const p_config_l = ((tStCAN_Node *)p_channel)->p_config;
	tStCAN_Control * const p_control_l = ((tStCAN_Node *)p_channel)->p_control;
	long status_l = -1;
	eCAN_Status ret_status = CAN_STATUS_FAILED;


	/* Check initialization */
	switch( init )
	{
		case CAN_INIT_OPEN:
			/* Check already opened or not */
			if( 1 == p_control_l->state.init )
			{
				ret_status = CAN_STATUS_ALREADY_INITIALIZED;
			}
			else
			{
				p_control_l->state.node = (unsigned long) (node & 0x03);
				status_l = XMC_CAN_NODE_NominalBitTimeConfigureEx( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
									(const XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const) p_config_l->p_bit_time_config );

				/* Check status */
				if( XMC_CAN_STATUS_SUCCESS == status_l )
				{
					/* Allow changes to CAN Control */
					XMC_CAN_NODE_EnableConfigurationChange( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context );

					/* Configure Tx and Rx Message Objects */
					XMC_CAN_MO_Config( (const XMC_CAN_MO_t *const) p_config_l->p_message_tx );
					XMC_CAN_MO_Config( (const XMC_CAN_MO_t *const) p_config_l->p_message_rx );

					XMC_CAN_AllocateMOtoNodeList( CAN_GLOBAL, (unsigned char)p_control_l->state.node,
											(unsigned char)(p_config_l->message_tx_number & 0xFF) );
					XMC_CAN_AllocateMOtoNodeList( CAN_GLOBAL, (unsigned char)p_control_l->state.node,
											(unsigned char)(p_config_l->message_rx_number & 0xFF) );

					/* If loopback is enabled then GPIO pins shall not be initialized */
					if( XMC_CAN_LOOKBACKMODE_ENABLED == p_config_l->loopback )
					{
						XMC_CAN_NODE_EnableLoopBack( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context );
					}
					else
					{
						XMC_CAN_NODE_SetReceiveInput( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
														p_config_l->rx_input_pin );
					}

					/* Interrupt Configurations for CAN Node */
					#if (CAN_USING_INTERRUPT == 1U)
					/* Set Error Event Nodes */
					XMC_CAN_NODE_SetEventNodePointer( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
										XMC_CAN_NODE_POINTER_EVENT_ALERT, p_config_l->isr_event_conifg->sr );
					XMC_CAN_NODE_SetEventNodePointer( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
										XMC_CAN_NODE_POINTER_EVENT_LEC, p_config_l->isr_event_conifg->sr );
					XMC_CAN_NODE_SetEventNodePointer( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
								XMC_CAN_NODE_POINTER_EVENT_FRAME_COUNTER, p_config_l->isr_event_conifg->sr );

					/* Set Transfer Event Nodes */
					XMC_CAN_NODE_SetEventNodePointer( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
									XMC_CAN_NODE_POINTER_EVENT_TRANSFER_OK, p_config_l->isr_config->sr );

					/* Set Message Object Events Node Pointer */
					XMC_CAN_MO_SetEventNodePointer( p_config_l->p_message_tx,
										XMC_CAN_MO_POINTER_EVENT_TRANSMIT, p_config_l->isr_mo_conifg->sr );
					XMC_CAN_MO_SetEventNodePointer( p_config_l->p_message_tx,
										XMC_CAN_MO_POINTER_EVENT_RECEIVE, p_config_l->isr_mo_conifg->sr );

					XMC_CAN_MO_EnableEvent( p_config_l->p_message_tx, XMC_CAN_MO_POINTER_EVENT_TRANSMIT );
					XMC_CAN_MO_EnableEvent( p_config_l->p_message_tx, XMC_CAN_MO_POINTER_EVENT_RECEIVE );

					#if (CAN_USING_EVENTS == 1U)
					for( status_l = 0; status_l < (long)p_config_l->events; ++status_l )
					{
						XMC_CAN_NODE_EnableEvent( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context,
													p_config_l->p_events[status_l] );
					}
					/* Setup NVIC for CAN Events */
					NVIC_SetPriority( (IRQn_Type)p_config_l->isr_event_conifg->irq.irq_num,
						NVIC_EncodePriority(NVIC_GetPriorityGrouping(), p_config_l->isr_event_conifg->irq.irq_priority, 0U) );
					NVIC_EnableIRQ( (IRQn_Type)p_config_l->isr_event_conifg->irq.irq_num );
					#endif	/* CAN_USING_EVENTS */

					/* Setup NVIC for CAN Transfer */
					NVIC_SetPriority( (IRQn_Type)p_config_l->isr_config->irq.irq_num,
						NVIC_EncodePriority(NVIC_GetPriorityGrouping(), p_config_l->isr_event_conifg->irq.irq_priority, 0U) );
					NVIC_EnableIRQ( (IRQn_Type)p_config_l->isr_config->irq.irq_num );
					/* Setup NVIC for CAN MO */
					NVIC_SetPriority( (IRQn_Type)p_config_l->isr_mo_conifg->irq.irq_num,
						NVIC_EncodePriority(NVIC_GetPriorityGrouping(), p_config_l->isr_event_conifg->irq.irq_priority, 0U) );
					NVIC_EnableIRQ( (IRQn_Type)p_config_l->isr_mo_conifg->irq.irq_num );
					#endif	/* CAN_USING_INTERRUPT */

					/* ENable CAN Node */
					XMC_CAN_NODE_Enable( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context );

					/* Disbale CAN Node configuration changes */
					XMC_CAN_NODE_DisableConfigurationChange( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context );

					/* Update status */
					p_control_l->state.init = 1;
				}
				else
				{
					ret_status = CAN_STATUS_FAILED;
				}
			}
			break;

		case CAN_INIT_CLOSE:
			/* Disbale CAN Node */
			XMC_CAN_NODE_Disable( (XMC_CAN_NODE_t *const) p_config_l->p_channel_context );
			/* Update status */
			p_control_l->state.init = 0;
			break;

		default:
			ret_status = CAN_STATUS_INVALID_ARGUMENT;
	}

	/* Return status */
	return ret_status;
}


/**
 * @function
 *
 * @brief
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_node_transmit( void * p_channel, const unsigned char node,
				const void * const p_data, const unsigned char length )
{
	DRIVER_ASSERT( NULL == p_channel, CAN_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( NULL == p_data, CAN_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( length > 8, CAN_STATUS_OUT_OF_BOUND );

	/* Local Variables */
	tStCAN_Control * const p_control_l = ((tStCAN_Node *)p_channel)->p_control;
	tStCAN_Data can_data_l;
	eCAN_Status ret_status = CAN_STATUS_FAILED;

	/* Check node */
	switch( node )
	{
		case 0:
			ret_status = (0 == p_control_l->state.init) ? CAN_STATUS_NOT_INITIALIZED : CAN_STATUS_SUCCESS;
		break;

		case 1:
			ret_status = (0 == p_control_l->state.init) ? CAN_STATUS_NOT_INITIALIZED : CAN_STATUS_SUCCESS;
		break;

		case 2:
			ret_status = (0 == p_control_l->state.init) ? CAN_STATUS_NOT_INITIALIZED : CAN_STATUS_SUCCESS;
		break;

		default:
			/* Update return status */
			ret_status = CAN_STATUS_INVALID_ARGUMENT;
			return ret_status;
	}

	/* Call Send operation */
	if( CAN_STATUS_SUCCESS == ret_status )
	{
		/* Update CAN Data */
		can_data_l.p_node = (tStCAN_Node *)p_channel;
		can_data_l.p_data = ((tStCAN_DataBytes* )p_data);
		can_data_l.data_length = length;
		/* Send CAN Data */
		ret_status = node_transfer( &can_data_l, CAN_OPERATION_TYPE_TRANSMIT );
	}

	/* Return status */
	return ret_status;
}


/**
 * @function
 *
 * @brief
 *
 * <i>Imp Note:</i>
 * 					Not Recommended to be used for now
 *
 */
long CAN_node_receive( void * p_channel, const unsigned char node,
					void * const p_data, const unsigned char length )
{
	DRIVER_ASSERT( NULL == p_channel, CAN_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( NULL == p_data, CAN_STATUS_INVALID_ARGUMENT );
	DRIVER_ASSERT( length > 8, CAN_STATUS_OUT_OF_BOUND );

	/* Local Variables */
	tStCAN_Control * const p_control_l = ((tStCAN_Node *)p_channel)->p_control;
	tStCAN_Data can_data_l;
	eCAN_Status ret_status = CAN_STATUS_FAILED;

	/* Check node */
	switch( node )
	{
		case 0:
			ret_status = (0 == p_control_l->state.init) ? CAN_STATUS_NOT_INITIALIZED : CAN_STATUS_SUCCESS;
		break;

		case 1:
			ret_status = (0 == p_control_l->state.init) ? CAN_STATUS_NOT_INITIALIZED : CAN_STATUS_SUCCESS;
		break;

		case 2:
			ret_status = (0 == p_control_l->state.init) ? CAN_STATUS_NOT_INITIALIZED : CAN_STATUS_SUCCESS;
		break;

		default:
			/* Update return status */
			ret_status = CAN_STATUS_INVALID_ARGUMENT;
			return ret_status;
	}

	/* Call Send operation */
	if( CAN_STATUS_SUCCESS == ret_status )
	{
		/* Update CAN Data */
		can_data_l.p_node = (tStCAN_Node *)p_channel;
		can_data_l.p_data =  ((tStCAN_DataBytes* )p_data);
		can_data_l.data_length = length;
		/* Send CAN Data */
		ret_status = node_transfer( &can_data_l, CAN_OPERATION_TYPE_RECEIVE );
	}

	/* Return status */
	return ret_status;
}


/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
long node_transfer( tStCAN_Data * p_data, eCAN_OperationType type )
{
	/* Local Variables */
	unsigned char length_l = 0;
	long ret_status = CAN_STATUS_FAILED;

	/* Check type of operation */
	switch( type )
	{
	default:
		/* Transmission operation */
		case CAN_OPERATION_TYPE_TRANSMIT:
			if( 1 == p_data->p_node->p_control->state.tx_busy )
			{
				ret_status = CAN_STATUS_BUSY;
			}
			else
			{
				/* Update CAN Data in MO */
				p_data->p_node->p_config->p_message_tx->can_data_length = p_data->data_length;
				for( length_l = 0; length_l < p_data->data_length; ++length_l )
				{
					p_data->p_node->p_config->p_message_tx->can_data_byte[length_l] = p_data->p_data->data_byte[length_l];
				}
				ret_status = XMC_CAN_MO_UpdateData( p_data->p_node->p_config->p_message_tx );
				/* Check status */
				if( XMC_CAN_STATUS_SUCCESS == ret_status )
				{
					ret_status = XMC_CAN_MO_Transmit( p_data->p_node->p_config->p_message_tx );
					/* Update status */
					if( XMC_CAN_STATUS_BUSY == ret_status )
					{
						ret_status = CAN_STATUS_BUSY;
					}
					else if( XMC_CAN_STATUS_MO_DISABLED == ret_status )
					{
						ret_status = CAN_STATUS_FAILED;
					}
					else
					{
						/* Update busy flag */
						p_data->p_node->p_control->state.tx_busy = 1;
						ret_status = CAN_STATUS_SUCCESS;
					}
				}
				else
				{/* XMC_CAN_STATUS_MO_NOT_ACCEPTABLE case not valid */
					ret_status = CAN_STATUS_NOT_ACCEPTABLE;
				}
			}
		break;
		/* Receive operation */
		case CAN_OPERATION_TYPE_RECEIVE:
			if( 1 == p_data->p_node->p_control->state.rx_busy )
			{
				ret_status = CAN_STATUS_BUSY;
			}
			else
			{
				ret_status = XMC_CAN_MO_ReceiveData( p_data->p_node->p_config->p_message_rx );
				/* Check status */
				if( XMC_CAN_STATUS_SUCCESS == ret_status )
				{
					/* Update busy flag */
					p_data->p_node->p_control->state.rx_busy = 1;
					/* Update received data */
					for( length_l = 0; length_l < p_data->data_length; ++length_l )
					{
						p_data->p_data->data_byte[length_l] = p_data->p_node->p_config->p_message_rx->can_data_byte[length_l];
					}
				}
				else
				{
					/* Update status */
					ret_status = (XMC_CAN_STATUS_BUSY == ret_status) ? CAN_STATUS_BUSY : CAN_STATUS_NOT_ACCEPTABLE;
				}
			}
		break;
	}

	/* Return status */
	return ret_status;
}


/********************************** End of File *******************************/
