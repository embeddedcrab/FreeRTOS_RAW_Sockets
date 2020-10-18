/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		SCU Configuration Header
* Filename:		dri_scu_conf.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_scu_conf.h
 *  @brief:	This file contains configurations for SCU
 *  		SCU and System Clock iinitialization is controlled from SystemInit
 */
#ifndef DRI_SCU_CONF_H_
#define DRI_SCU_CONF_H_


/******************************************************************************
* Includes
*******************************************************************************/


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/* Callback Handler */
#define SCU_CALLBACK_HANDLER_USED		1U

#define USING_EXTERNAL_CLOCK_OUTPUT		0U	/* Being controlled from SystemInit */
#define USING_BROWN_OUT_DETECT			1U
#define USING_WATCHDOG					1U


/******************************************************************************
* Configuration Constants
*******************************************************************************/


#if (USING_BROWN_OUT_DETECT == 1U)
#define BROWN_OUT_DETECT_VOLTAGE		2.8f/* Need to change according to conf.c file */
#endif

/******************************************************************************
* Macros
*******************************************************************************/

/* Interrupt Handler for SCU Events */
#define SCU_Interrupt_Handler			IRQ_Hdlr_0


#endif /* DRI_SCU_CONF_H_ */

/********************************** End of File ******************************/
