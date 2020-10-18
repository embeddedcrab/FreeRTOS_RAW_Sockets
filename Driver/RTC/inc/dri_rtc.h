/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		RTC API Header
* Filename:		dri_rtc.h
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*******************************************************************************/

/** @file:	dri_rtc.h
 *  @brief:	This file contains API function declarations for RTC
 */
#ifndef DRI_RTC_H_
#define DRI_RTC_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_rtc_types.h>


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
long RTC_init( void * p_channel, const unsigned char init );


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
static inline void RTC_set_time( void * p_channel, const struct tm *const p_time )
{
	XMC_RTC_SetTimeStdFormat( p_time );
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
 *
 */
static inline void RTC_get_time( void * p_channel, struct tm *const p_time )
{
	XMC_RTC_GetTimeStdFormat( p_time );
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
 *
 */
static inline void RTC_set_alarm( void * p_channel, const struct tm *const p_time )
{
	XMC_RTC_SetAlarmStdFormat( p_time );
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
 *
 */
static inline void RTC_get_alarm( void * p_channel, struct tm *const p_time )
{
	XMC_RTC_GetAlarmStdFormat( p_time );
}


#ifdef __cplusplus
}
#endif


#endif	/* DRI_RTC_H_ */

/********************************** End of File *******************************/
