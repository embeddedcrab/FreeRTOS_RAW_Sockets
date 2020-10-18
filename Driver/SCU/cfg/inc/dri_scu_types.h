/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		SCU data types Header
* Filename:		dri_scu_types.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_scu_types.h
 *  @brief:	This file contains data types definitions for SCU
 */
#ifndef DRI_SCU_TYPES_H_
#define DRI_SCU_TYPES_H_


/******************************************************************************
* Includes
*******************************************************************************/

/* Include XMC Headers */
#include <xmc_scu.h>
#include <xmc_wdt.h>

/* Include private headers first */
#include <private/dri_scu_conf.h>
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
 * @brief	Enum for Status of SCU
 * */
typedef enum eSCU_Status_
{
	SCU_STATUS_SUCCESS = 0,
	SCU_STATUS_FAILED,
	SCU_STATUS_INVALID_ARGUMENT,
	SCU_STATUS_ALREADY_INITIALIZED
} eSCU_Status;

/*
 * @brief	Enum for Open or close
 * */
typedef enum eSCU_Init_
{
	SCU_INIT_CLOSE = 0,
	SCU_INIT_OPEN
} eSCU_Init;

/*
 * @brief	typedef for callback function pointer
 * */
typedef void (*scu_callback_pointer) ( void );


/******************************************************************************
 *
 * @brief	Data Types for SCU instance
 *
 *****************************************************************************/

/*
 * @brief	typedef for status control of SCU
 */
typedef struct tStSCU_StatusControl_
{
	union
	{
		struct
		{
			volatile unsigned long init	: 1;
			unsigned long state_res		: 31;
		};
		unsigned long state_value;
	};
} tStSCU_StatusControl;

/************************************************************************************/

/*
 * @brief Structure for SCU channel interrupt configuration
 */
typedef struct tStSCU_InterruptConfiguartion_
{
	/**< Interrupt configuration */
	tStInterruptConfig irq;
} tStSCU_InterruptConfiguartion;


/************************************************************************************/


/************************************************************************************/

/*
 * @brief	Structure for SCU Module Configuration
 *
 * <i>Imp Note:</i>
 * 			Not using Module Context because of Unique and Global module
 */
typedef struct tStSCU_GlobalConfiguartion_
{
	/**< SCU User Configuration */
//	XMC_SCU_CLOCK_CONFIG_t * p_config;/* Already included in SystemInit */
	/**< SCU Interrupt Configurations */
	tStSCU_InterruptConfiguartion * p_isr_config;

	/**< SCU Traps */
	unsigned long traps;
	XMC_SCU_INTERRUPT_EVENT_HANDLER_t trap_handler;
	XMC_SCU_NMIREQ_t nmi_request;

	#if (USING_WATCHDOG == 1U)
	unsigned long events;
	XMC_WDT_CONFIG_t * p_wdt_config;
	unsigned long wdt_clock_div;
	unsigned char wdt_servie_pulse_width;
	XMC_SCU_CLOCK_WDTCLKSRC_t wdt_clock;
	XMC_WDT_MODE_t wdt_mode;
	#endif
	#if (USING_EXTERNAL_CLOCK_OUTPUT == 1U)
	XMC_SCU_CLOCK_EXTOUTCLKSRC_t external_output_clock;
	#endif
	/**< Monitor Configurations */
	#if (USING_BROWN_OUT_DETECT == 1U)
	unsigned char threshold;
	unsigned char interval;
	#endif
} tStSCU_GlobalConfiguartion;


/*
 * @brief	Structure for SCU Control
 */
typedef struct tStSCU_Control_
{
	volatile tStSCU_StatusControl state;
} tStSCU_Control;

/*
 * @brief Structure for SCU Module callback
 */
typedef struct tStSCU_Callbacks_
{
#if (SCU_CALLBACK_HANDLER_USED == 1U)
	scu_callback_pointer fp_callback;
#endif
} tStSCU_Callbacks;

/*
 * @brief Structure for SCU Module
 */
typedef struct tStSCU_
{
	/**< Configurations ofr Module */
	tStSCU_GlobalConfiguartion * p_module_config;
	/* SCU Control */
	tStSCU_Control * p_control;
	#if (SCU_CALLBACK_HANDLER_USED == 1U)
	tStSCU_Callbacks * p_callback;
	#endif
} tStSCU;


/************************************************************************************/


#endif /* DRI_SCU_TYPES_H_ */

/********************************** End of File *******************************/
