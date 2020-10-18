/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		CAN API Header
* Filename:		dri_can.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_can.h
 *  @brief:	This file contains API function declarations for CAN
 */
#ifndef DRI_CAN_H_
#define DRI_CAN_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_can_types.h>


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
* Function Prototypes
*******************************************************************************/


/******************************************************************************
 *
 * @brief
 *
 * \par<b>Description:</b><br>:
 *
 ******************************************************************************/
/******************************************************************************
 *
 * @brief	API Member functions
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * @param[out]	NA
 *
 * @return
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_module_init( void * p_module, const unsigned char init );


/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * @param[out]	NA
 *
 * @return
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_node_init( void * p_channel, const unsigned char node, const unsigned char init );


/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * @param[out]	NA
 *
 * @return
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_node_transmit( void * p_channel, const unsigned char node,
				const void * const p_data, const unsigned char length );


/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * @param[out]	NA
 *
 * @return
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
long CAN_node_receive( void * p_channel, const unsigned char node,
					void * const p_data, const unsigned char length );


/**
 * @function
 *
 * @brief
 *
 * @param[in]
 *
 * @param[out]	NA
 *
 * @return
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
static inline long CAN_node_transfer( void * p_channel, const unsigned char node,
									void * const p_data, const unsigned char type )
{
	/* Local variables */
	eCAN_Status ret_status = CAN_STATUS_INVALID_ARGUMENT;

	/* Check type */
	if( 0 == type )
	{
		return CAN_node_transmit( p_channel, node, p_data, 8 );
	}
	else if( 1 == type )
	{
		return CAN_node_receive( p_channel, node, p_data, 8 );
	}
	else
	{
		/* Do Nothing */
	}

	/* Return status */
	return ret_status;
}



#ifdef __cplusplus
}
#endif


#endif	/* DRI_CAN_H_ */

/********************************** End of File *******************************/
