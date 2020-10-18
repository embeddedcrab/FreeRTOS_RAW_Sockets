/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		CAN Configuration Header
* Filename:		dri_can_conf.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_can_conf.h
 *  @brief:	This file contains configurations for CAN
 */
#ifndef DRI_CAN_CONF_H_
#define DRI_CAN_CONF_H_


/******************************************************************************
* Includes
*******************************************************************************/


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/* Interrupt or DMA usage */
#define CAN_USING_INTERRUPT				1U
#define CAN_USING_DMA					0U

#if ((CAN_USING_INTERRUPT == 1U) && (CAN_USING_DMA == 1U))
#error "Cannot use both at same time"
#endif

#define CAN_USING_EVENTS				1U

/* Callback Handler */
#define CAN_CALLBACK_HANDLER_USED		1U


/******************************************************************************
* Configuration Constants
*******************************************************************************/

#define FCAN							120000000	/* Fcan = Fsys */
#define CAN_GLOBAL						CAN			/* Global CAN Module */

#define CAN_NODES_USED					2U
#define CAN_EVENTS						4U


#if (CAN_NODES_USED > 0U)
#define CAN0_CHANNEL_USED				0U
#define CAN1_CHANNEL_USED				1U
#define CAN2_CHANNEL_USED				1U
#endif


/******************************************************************************
* Macros
*******************************************************************************/

#if (CAN_USING_EVENTS == 1U)

/* CAN 0 Event Configurations */
#if (CAN0_CHANNEL_USED == 1U)
#define CAN0_EVENTS_USED				0U
#define CAN0_EVENTS						4U
#endif	/* CAN0_CHANNEL_USED */

/* CAN 1 Event Configurations */
#if (CAN1_CHANNEL_USED == 1U)
#define CAN1_EVENTS_USED				1U
#define CAN1_EVENTS						4U
#endif	/* CAN1_CHANNEL_USED */

/* CAN 2 Event Configurations */
#if (CAN2_CHANNEL_USED == 1U)
#define CAN2_EVENTS_USED				1U
#define CAN2_EVENTS						4U
#endif	/* CAN2_CHANNEL_USED */

#if ((CAN0_EVENTS_USED == 1U) && (CAN1_EVENTS_USED == 1U) && (CAN0_EVENTS_USED == 1U))
#error "Cannot Use All Channel Events Simultaneously"
#endif

#endif	/* CAN_USING_EVENTS */

/*-----------------------------------------------------------*/

/* CAN Interrupt Handlers */
#if (CAN0_CHANNEL_USED == 1U)
#define CAN0_Node_Transfer_Handler		IRQ_Hdlr_76
#define CAN0_MessageObject_Handler		IRQ_Hdlr_77
#if (CAN0_EVENTS_USED == 1U)/* Not avaiable, only 8 SR, need to switch between channels */
#define CAN0_Event_Handler				IRQ_Hdlr_80
#endif
#endif	/* CAN0_CHANNEL_USED */

#if (CAN1_CHANNEL_USED == 1U)
#define CAN1_Node_Transfer_Handler		IRQ_Hdlr_78
#define CAN1_MessageObject_Handler		IRQ_Hdlr_79
#if (CAN1_EVENTS_USED == 1U)
#define CAN1_Event_Handler				IRQ_Hdlr_80
#endif
#endif	/* CAN1_CHANNEL_USED */

#if (CAN2_CHANNEL_USED == 1U)
#define CAN2_Node_Transfer_Handler		IRQ_Hdlr_81
#define CAN2_MessageObject_Handler		IRQ_Hdlr_82
#if (CAN2_EVENTS_USED == 1U)
#define CAN2_Event_Handler				IRQ_Hdlr_83
#endif
#endif	/* CAN2_CHANNEL_USED */


#endif /* DRI_CAN_CONF_H_ */

/********************************** End of File ******************************/
