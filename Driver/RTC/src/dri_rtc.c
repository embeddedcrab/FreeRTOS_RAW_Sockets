/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		RTC API Source
* Filename:		dri_rtc.c
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_rtc.c
 *  @brief:	This header file contains API function definitions for RTC
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

/* RTC Events for Event Handler */
#define RTC_EVENTS			(XMC_RTC_EVENT_PERIODIC_MINUTES |\
							XMC_RTC_EVENT_PERIODIC_HOURS | XMC_RTC_EVENT_PERIODIC_DAYS |\
							XMC_RTC_EVENT_PERIODIC_MONTHS | XMC_RTC_EVENT_PERIODIC_YEARS |\
							XMC_RTC_EVENT_ALARM)
/* XMC_RTC_EVENT_PERIODIC_SECONDS */


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
long RTC_init( void * p_channel, const unsigned char init )
{
	DRIVER_ASSERT( NULL == p_channel, RTC_STATUS_INVALID_ARGUMENT );
	/* Local Variables */
	tStRTC_ModuleConfiguartion * p_config_l = ((tStRTC *)p_channel)->p_config;
	tStRTC_Control * p_control_l = ((tStRTC *)p_channel)->p_control;
	long ret_status = RTC_STATUS_FAILED;

	/* Check Initiallization */
	switch( init )
	{
		case RTC_INIT_OPEN:
			/* Check initialization state */
			if( 1 == p_control_l->state.init )
			{
				ret_status = RTC_STATUS_ALREADY_INITIALIZED;
			}
			else
			{
				/* Set RTC CLock source */
				XMC_SCU_HIB_SetRtcClockSource( p_config_l->clock_source );
				/* Initialize RTC channel */
				ret_status = XMC_RTC_Init( (const XMC_RTC_CONFIG_t *const) p_config_l->p_channel_config );
				/* Check status */
				if( XMC_RTC_STATUS_OK == ret_status )
				{
					/* Set Default RTC Time */
					XMC_RTC_SetTime( &(p_config_l->p_channel_config->time) );
					/* Set Alarm */
					XMC_RTC_SetAlarm( &(p_config_l->p_channel_config->alarm) );

					/* Get standard time values */
					#if (RTC_USING_STANDARD_TIME == 1U)
					XMC_RTC_GetTimeStdFormat( &(p_control_l->time) );
					XMC_RTC_GetAlarmStdFormat( &(p_control_l->alarm) );
					#endif
					/* Enable RTC Events */
					XMC_RTC_EnableEvent( RTC_EVENTS );
					/* Enable Hibernate Wakeup for RTC Alarm */
					XMC_RTC_EnableHibernationWakeUp( XMC_RTC_WAKEUP_EVENT_ON_ALARM );

					#if (RTC_ALARM_NMI_REQUEST == 1U)
					/* Enable NMI request for RTC Alarm
					 * Note: NMI has to be handled in NMI handler */
					XMC_SCU_INTERRUPT_EnableNmiRequest( XMC_SCU_NMIREQ_RTC_AI );
					#elif (RTC_ALARM_NMI_REQUEST == 0U)
					XMC_SCU_INTERRUPT_SetEventHandler( XMC_SCU_INTERRUPT_EVENT_RTC_ALARM, &RTC_Alarm_Handler );
					#endif
					/* Set RTC Event Handlers,
					 * Note: Not checking the return status, should consider the max event numbers */
					XMC_SCU_INTERRUPT_SetEventHandler( XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC, &RTC_Event_Handler );
					XMC_SCU_INTERRUPT_EnableEvent( XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC | XMC_SCU_INTERRUPT_EVENT_RTC_ALARM );

					/* Enable and Start RTC */
					XMC_RTC_Enable();
					XMC_RTC_Start();

					/* Update parameters */
					p_control_l->state.init = 1;
					/* Update return status */
					ret_status = RTC_STATUS_SUCCESS;
				}
			}
		break;

		case RTC_INIT_CLOSE:
			/* Disable and Stop RTC */
			XMC_RTC_Disable();
			XMC_RTC_Stop();
			ret_status = RTC_STATUS_SUCCESS;
			/* update initialization satte */
			p_control_l->state.init = 0;
		break;

		default:
			ret_status = RTC_STATUS_INVALID_ARGUMENT;
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


/********************************** End of File *******************************/
