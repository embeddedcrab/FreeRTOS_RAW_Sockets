/*********************************************************************************************************************
 * @file	led_task.cpp
 * @brief	LED Flasher API Source
 * @version	1.0.0
 * @date 	03/04/2020
 *
 * @cond
 *********************************************************************************************************************
 * Copyright (c) 2020, Hemant Sharma - All Rights Reserved
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************
 *
 * *************************** Change history ********************************
 *
 * @endcond
 */


/******************************************************************************
* Includes
*******************************************************************************/

/* Include class headers */
#include <led_task.h>


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

/* Declare static data member of class
 *
 * Note: Tell linker to take variable from this source file
 * */



/******************************************************************************
* Function Definitions
*******************************************************************************/

/******* Public Member function for LedTask__ class *******/


/**
 * @function
 *
 * @brief		LED Task iniitalization function
 *
 * <i>Imp Note:</i>
 */
void LedTask__ :: xvinit( void )
{
	/* Return Status of Task */
	BaseType_t xReturned = pdFAIL;

	/* Create task */
	xReturned = xTaskCreate( LedTask__::xsvTaskEntryPoint, xLedTaskParams.p_name,
			xLedTaskParams.stackDepth, this, xLedTaskParams.priority, &xHandle );

	/* Delete task if not created successfully */
	if( xReturned != pdPASS )
	{
		vTaskDelete( xHandle );

		/* Halt if LED Flasher task is not started */
		for( ; ; );
	}
	else
	{
		/* Can do some initial processing if needed */
	}
}


/**
 * @function
 *
 * @brief		LED Task Entry function
 *
 * <i>Imp Note:</i>
 */
void LedTask__ :: xvTaskEntry( void *pvParams )
{
	/* Not using */
	(void)pvParams;

	/* Local Variables */
    TickType_t xLastWakeTime = 0;
    const TickType_t xFrequency = 500;

    /* Initialize the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    /* Task execution entry */
    for( ; ; )
    {
		GPIO__::fp_toggle( (XMC_GPIO_PORT_t *)xLedPort, xLedPin );
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}

	/* Should never reach here, if so the delete the task and free memory */
	vTaskDelete( NULL );
}


/********************************** End of File *******************************/
