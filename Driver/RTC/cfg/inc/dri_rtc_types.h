/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		RTC data types Header
* Filename:		dri_rtc_types.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_rtc_types.h
 *  @brief:	This file contains data types definitions for RTC
 */
#ifndef DRI_RTC_TYPES_H_
#define DRI_RTC_TYPES_H_


/******************************************************************************
* Includes
*******************************************************************************/

/* Include XMC Headers */
#include <xmc_rtc.h>
#include <xmc_scu.h>

/* Include private headers first */
#include <private/dri_rtc_conf.h>
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
 * @brief	Enum for Status of RTC
 * */
typedef enum eRTC_Status_
{
	RTC_STATUS_SUCCESS = 0,
	RTC_STATUS_FAILED,
	RTC_STATUS_BUSY,
	RTC_STATUS_INVALID_ARGUMENT,
	RTC_STATUS_OUT_OF_BOUND,
	RTC_STATUS_ALREADY_INITIALIZED
} eRTC_Status;

/*
 * @brief	Enum for Open or close
 * */
typedef enum eRTC_Init_
{
	RTC_INIT_CLOSE = 0,
	RTC_INIT_OPEN
} eRTC_Init;


/*
 * @brief	Enum for RTC Events
 * */
typedef enum eRTC_Events_
{
	RTC_EVENT_ALARM = 0,
	RTC_EVENT_SECOND,
	RTC_EVENT_MINUTE,
	RTC_EVENT_HOUR,
	RTC_EVENT_DAY,
	RTC_EVENT_MONTH,
	RTC_EVENT_YEAR
} eRTC_Events;

/*
 * @brief	typedef for callback function pointer
 * */
typedef void (*rtc_callback_pointer) ( void * p_channel, const eRTC_Events event );


/******************************************************************************
 *
 * @brief	Data Types for RTC instance
 *
 *****************************************************************************/

/*
 * @brief	typedef for status control of RTC
 */
typedef struct tStRTC_StatusControl_
{
	union
	{
		struct
		{
			unsigned long init			: 1;
			unsigned long state_res		: 31;
		};
		unsigned long state_value;
	};
} tStRTC_StatusControl;


/************************************************************************************/


/************************************************************************************/

/*
 * @brief	Structure for RTC Module Configuration
 */
typedef struct tStRTC_ModuleConfiguartion_
{
	/**< Context for RTC module */
	void * p_module_context;
	/**< Configuration for RTC */
	XMC_RTC_CONFIG_t * p_channel_config;
	XMC_SCU_HIB_RTCCLKSRC_t clock_source;
} tStRTC_ModuleConfiguartion;


/*
 * @brief	Structure for RTC Control
 */
typedef struct tStRTC_Control_
{
	#if (RTC_USING_STANDARD_TIME == 1U)
	struct tm time;
	struct tm alarm;
	#endif
	tStRTC_StatusControl state;
} tStRTC_Control;

/*
 * @brief Structure for RTC channel callbacks
 */
typedef struct tStRTC_Callbacks_
{
#if (RTC_CALLBACK_HANDLER_USED == 1U)
	rtc_callback_pointer fp_callback;
#endif
} tStRTC_Callbacks;

/*
 * @brief Structure for RTC channel API
 */
typedef struct tStRTC_Api_
{
	long (*fp_init) ( void * );
} tStRTC_Api;

/*
 * @brief Structure for RTC
 */
typedef struct tStRTC_
{
	/**< RTC Configurations */
	tStRTC_ModuleConfiguartion * p_config;
	/* RTC control */
	tStRTC_Control * p_control;
	#if (RTC_CALLBACK_HANDLER_USED == 1U)
	tStRTC_Callbacks * p_callback;
	#endif
} tStRTC;


/************************************************************************************/


#endif /* DRI_RTC_TYPES_H_ */

/********************************** End of File *******************************/
