/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		RTC Configuration Header
* Filename:		dri_rtc_conf.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_rtc_conf.h
 *  @brief:	This file contains configurations for RTC
 */
#ifndef DRI_RTC_CONF_H_
#define DRI_RTC_CONF_H_


/******************************************************************************
* Includes
*******************************************************************************/


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/* Callback Handler */
#define RTC_CALLBACK_HANDLER_USED		1U

/* RTC Alarm Interrupt Configuration */
#define RTC_ALARM_NMI_REQUEST			1U


/******************************************************************************
* Configuration Constants
*******************************************************************************/

/* Note: Information Macros */
#define FRTC							32768	/* Frtc = XTALrtc */
/* End */

#define RTC_USED						1U

/* Standard time format for RTC */
#define RTC_USING_STANDARD_TIME			1U


#define RTC_PRESCALE					0x7FFF	/* says 1s acc. to 32768Hz */



/******************************************************************************
* Macros
*******************************************************************************/

/* RTC Interrupt Handlers */
#if (RTC_USED == 1U)
extern void RTC_Event_Handler( void );
#if (RTC_ALARM_NMI_REQUEST == 0U)
extern void RTC_Alarm_Handler( void );
#endif
#endif


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/


#endif /* DRI_RTC_CONF_H_ */

/********************************** End of File ******************************/
