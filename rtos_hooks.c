/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		RTOS Hooks Source
* Filename:		rtos_hooks.c
* Author:		HS
* Origin Date:
* Version:
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	rtos_hooks.c
 *  @brief:	This source file contains callback functions of RTOS
 */


/******************************************************************************
* Includes
*******************************************************************************/

/* Include FreeRTOS Headers */
#include <FreeRTOS.h>
#include <task.h>


/******************************************************************************
* Functions
*******************************************************************************/


/*-----------------------------------------------------------*/

#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    (void) xTask;
    (void) pcTaskName;
    /* Check for Stack Overflow cause */

    /* For Debugging only */
	#if (DEBUG_ == 1U)
    for( ; ; )
    {
    	/* Halt here */
    	__asm("NOP");
    }
	#endif
}

#endif

/*-----------------------------------------------------------*/

#if( configUSE_TICK_HOOK > 0 )
void vApplicationTickHook( void );
void vApplicationTickHook( void )
{
    /* Dummy Implementation */
	return;
}

#endif

/*-----------------------------------------------------------*/

#if ( configUSE_IDLE_HOOK == 1 )
void vApplicationIdleHook( void );
void vApplicationIdleHook( void )
{
	/* It must *NOT* attempt to block.  In this case the
	 * idle task just sleeps or yields to lower the CPU usage. */
	portYIELD();
}

#endif

/*-----------------------------------------------------------*/

#if( configUSE_MALLOC_FAILED_HOOK == 1 )
void vApplicationMallocFailedHook( void );
void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

    /* Report malloc failed hook */
	for( ; ; )
	{
		__asm("NOP");
	}
}

#endif  /* #if( configUSE_MALLOC_FAILED_HOOK == 1 ) */

/*-----------------------------------------------------------*/

/* Use by the pseudo random number generator. */
static UBaseType_t ulNextRand;

UBaseType_t uxRand( void );
UBaseType_t uxRand( void )
{
const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;

	/* Utility function to generate a pseudo random number. */

	ulNextRand = ( ulMultiplier * ulNextRand ) + ulIncrement;
	return( ( int ) ( ulNextRand >> 16UL ) & 0x7fffUL );
}

/*-----------------------------------------------------------*/
void vLoggingPrintf( const char *pcFormat, ... );
void vLoggingPrintf( const char *pcFormat, ... )
{
    /* Dummy implementation */
    (void) pcFormat;
}


/*
* Callback that provides the inputs necessary to generate a randomized TCP
* Initial Sequence Number per RFC 6528.  In this case just a psuedo random
* number is used so THIS IS NOT RECOMMENDED FOR PRODUCTION SYSTEMS.
*/
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
uint16_t usSourcePort, uint32_t ulDestinationAddress, uint16_t usDestinationPort );
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress, uint16_t usSourcePort,
						uint32_t ulDestinationAddress, uint16_t usDestinationPort
						)
{
     ( void ) ulSourceAddress;
     ( void ) usSourcePort;
     ( void ) ulDestinationAddress;
     ( void ) usDestinationPort;

     return uxRand();
}


BaseType_t xApplicationGetRandomNumber( uint32_t *pulNumber );
BaseType_t xApplicationGetRandomNumber( uint32_t *pulNumber )
{
	*pulNumber = uxRand();
	return pdTRUE;
}


/********************************** End of File *******************************/
