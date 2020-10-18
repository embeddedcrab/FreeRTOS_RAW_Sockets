/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		SCU API Header
* Filename:		dri_scu.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_scu.h
 *  @brief:	This file contains API function declarations for SCU
 */
#ifndef DRI_SCU_H_
#define DRI_SCU_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_scu_types.h>


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
 * 				De-iniitalization of SCU might not be needed
 *
 */
long SCU_init( void * p_channel, const unsigned char init );


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
 * 				If RTOS used then initialize in unique task (LED flasher)
 *
 */
static inline void SCU_interrupt_init( void )
{
	/* Configure NVIC for SCU */
	XMC_WDT_Start();
	XMC_SCU_CLOCK_EnableClock( XMC_SCU_CLOCK_WDT );
	NVIC_EnableIRQ( (IRQn_Type)SCU_0_IRQn );
}


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
 * 				De-iniitalization of SCU might not be needed
 *
 */
static inline long SCU_assert_reset( void * p_channel, const unsigned long peripheral, const unsigned char state )
{
	(void) p_channel;
	/* Assert Reset on Peripherals */
	if( 1 == state )
	{
		XMC_SCU_RESET_AssertPeripheralReset( (const XMC_SCU_PERIPHERAL_RESET_t)peripheral );
		return SCU_STATUS_SUCCESS;
	}
	/* De-Assert Reset on Peripherals */
	else if( 0 == state )
	{
		XMC_SCU_RESET_DeassertPeripheralReset( (const XMC_SCU_PERIPHERAL_RESET_t)peripheral );
		return SCU_STATUS_SUCCESS;
	}
	else
	{
		return SCU_STATUS_FAILED;
	}
}


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
 * 				De-iniitalization of SCU might not be needed
 *
 */
static inline long SCU_check_assert( void * p_channel, const unsigned long peripheral )
{
	(void) p_channel;
	return XMC_SCU_RESET_IsPeripheralResetAsserted( (const XMC_SCU_PERIPHERAL_RESET_t)peripheral );
}


#ifdef __cplusplus
}
#endif


#endif	/* DRI_SCU_H_ */

/********************************** End of File *******************************/
