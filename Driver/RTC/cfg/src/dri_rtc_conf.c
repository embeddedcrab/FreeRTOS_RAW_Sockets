/*****************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		RTC Configuration Source
* Filename:		dri_rtc_conf.c
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_rtc_conf.c
 *  @brief:	This source file contains configuration function definitions for RTC
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_rtc.h>


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

/*
 * @brief	RTC API functions
 *
 * \par<b>Description:</b><br>
 * 			This is data structure for RTC API functions to be used by Application
 */


/*****************************************************************************/

/* Callback handler for user */
#if (RTC_CALLBACK_HANDLER_USED == 1U)
extern void RTC_callback_handler( void * p_channel, const eRTC_Events event );
#endif


#if (RTC_USED == 1U)
/* RTC Channel Configurations */
XMC_RTC_CONFIG_t RTC_ChannelConfiguration =
{
	.time =
	{
		.seconds = 0,
		.minutes = 20,
		.hours = 1,
		.days = 1,

		.daysofweek = XMC_RTC_WEEKDAY_SUNDAY,
		.month = 3,
		.year = 2020
	},
	.alarm =
	{
		.seconds = 0,
		.minutes = 22,
		.hours = 1,
		.days = 1,

		.month = 3,
		.year = 2020
	},
	.prescaler = RTC_PRESCALE
};

/* RTC Module Configurations */
tStRTC_ModuleConfiguartion RTC_Configuration =
{
	.p_module_context = RTC,
	.p_channel_config = &RTC_ChannelConfiguration,
	.clock_source = XMC_SCU_HIB_RTCCLKSRC_ULP
};
/* RTC Module Control */
tStRTC_Control RTC_Control;


tStRTC_Callbacks RTC_Callback =
{
	.fp_callback = &RTC_callback_handler
};

/** RTC Module **/
tStRTC RTC_ =
{
	.p_config = &RTC_Configuration,
	.p_control = &RTC_Control
	#if (RTC_CALLBACK_HANDLER_USED == 1U)
	,.p_callback = &RTC_Callback
	#endif
};
#endif	/* RTC_USED */


/******************************************************************************
* Function Definitions
*******************************************************************************/

/******* ISR Handlers for RTC channel *******/

/* RTC ISR Entries */
#if (RTC_USED == 1U)
void RTC_Event_Handler( void )
{
	__asm("NOP");
}

#if (RTC_ALARM_NMI_REQUEST == 0U)
void RTC_Alarm_Handler( void )
{
	__asm("NOP");
}
#endif
#endif


/*********************************** End of File ******************************/
