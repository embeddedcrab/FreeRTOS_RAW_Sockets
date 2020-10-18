/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		UART API Header
* Filename:		dri_uart.h
* Author:		HS
* Origin Date:	09/29/2019
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_uart.h
 *  @brief:	This file contains API function declarations
 *  		for UART on USIC channel
 */
#ifndef DRI_UART_H_
#define DRI_UART_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_uart_types.h>


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
 * @brief	API Member functions
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif


/**
 * @function	UART_init
 *
 * @brief		Initialization function for UART channel
 *
 * @param[in]	init	-	initialization state
 * 							eUART_Init_	-	UART_INIT_OPEN
 * 											UART_INIT_CLOSE
 *
 * @param[out]	NA
 *
 * @return  	eUART_Status_
 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
 *          	UART_STATUS_FAILED:		Operation failed.<BR>
 *
 * \par<b>Description:</b><br>
 * 				This function will initialize UART channel
 * 				with required parameters for operation
 *
 * <i>Imp Note:</i>
 *
 */
long UART_init( void * p_channel, const unsigned char init );


/**
 * @function	UART_set_mode
 *
 * @brief		Mode set function for UART channel
 *
 * @param[in]	mode	-	mode state
 * 							eUART_Mode	-	UART_MODE_DIRECT
 * 											UART_MODE_INTERRUPT
 *
 * @param[out]	NA
 *
 * @return  	eUART_Status_
 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
 *          	UART_STATUS_FAILED:		Operation failed.<BR>
 *
 * \par<b>Description:</b><br>
 * 				This function will set UART channel mode
 *
 * <i>Imp Note:</i>
 *
 */
long UART_set_mode( void * p_channel, const unsigned char mode );


/**
 * @function		UART_set_baud
 *
 * @brief		Set baudrate function
 *
 * @param[in]	speed	-	speed of operation of UART
 *
 * @param[out]	NA
 *
 * @return  	eUART_Status_
 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
 *          	UART_STATUS_FAILED:		Operation failed.<BR>
 *
 * \par<b>Description:</b><br>
 * 				This function will set baud rate for a particular channel
 *
 * <i>Imp Note:</i>
 *
 */
long UART_set_baud( void * p_channel, const unsigned long speed );


/**
 * @function		UART_transmit
 *
 * @brief		Registers as request for data transmission
 *
 * @param[in]	p_src	-	source data pointer
 * 				length	-	length of source data
 *
 * @param[out]	NA
 *
 * @return  	eUART_Status
 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
 *          	UART_STATUS_FAILED:		Operation failed.<BR>
 *
 * \par<b>Description:</b><br>
 * 				This function transmit data using interrupt functionality
 *
 * <i>Imp Note:</i>
 * 				This function can have functionality of waiting as used
 * 				in Polling of transmission data
 *
 */
long UART_transmit( void * p_channel, const unsigned char * p_src, const unsigned long length );


/**
 * @function		UART_receive
 *
 * @brief		Registers a request for data reception
 *
 * @param[in]	p_dest	-	destination data pointer
 * 				length	-	length of data to be received
 *
 * @param[out]	p_dest	-	destination data
 *
 * @return		eUART_Status
 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
 *          	UART_STATUS_FAILED:		Operation failed.<BR>
 *
 * \par<b>Description:</b><br>
 * 				This function will receive data from UART channel
 *
 * <i>Imp Note:</i>
 * 				This function can have functionality of waiting as used in
 * 				Polling for receive data
 *
 */
long UART_receive( void * p_channel, unsigned char * const p_dest, const unsigned long length );


/**
 * @function		UART_abort
 *
 * @brief		Data abort function
 *
 * @param[in]	tx_rx	-	abort state for Tx/Rx
 * 							eUART_Abort_	-	UART_ABORT_TX
 * 												UART_ABORT_RX
 * 												UART_ABORT_TX_RX
 *
 * @param[out]	NA
 *
 * @return  	eUART_Status
 *          	UART_STATUS_SUCCESS:	Operation successful.<BR>
 *          	UART_STATUS_FAILED:		Operation failed.<BR>
 *
 * \par<b>Description:</b><br>
 *
 * <i>Imp Note:</i>
 *
 */
long UART_abort( void * p_channel, const unsigned char tx_rx );


#ifdef __cplusplus
}
#endif

#endif /* DRI_UART_H_ */

/********************************** End of File *******************************/
