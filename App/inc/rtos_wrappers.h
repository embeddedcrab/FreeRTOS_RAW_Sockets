/*********************************************************************************************************************
 * @file	rtos_wrappers.h
 * @brief	RTOS Wrappers API Header
 * @version	1.0.0
 * @date	10/27/2019
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

#ifndef RTOS_WRAPPERS_H_
#define RTOS_WRAPPERS_H_


/******************************************************************************
* Includes
*******************************************************************************/

/* RTOS headers inclusion */
#include <FreeRTOS.h>
#include <semphr.h>
#include <timers.h>


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

/**
 * @brief
 */


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
 *
 * @brief	Class for RTOS Wrappers
 *
 ******************************************************************************/
class RTOS_WRAPPER__
{
/* Public Members */
public:
	/* Task creation function */
	static BaseType_t create( TaskFunction_t fp_task_code,
								const char * const p_name,
								const configSTACK_DEPTH_TYPE& stack_depth,
								void * const parameters, UBaseType_t priority,
								TaskHandle_t * const p_task_handle )
	{
		/* Local Variables */
		BaseType_t ret_status = pdTRUE;
		/* Create Task */
		ret_status = xTaskCreate( fp_task_code, p_name, stack_depth, parameters,
									priority, p_task_handle );
		/* Check return status */
		if( ret_status != pdTRUE )
		{
			vTaskDelete( *p_task_handle );
		}
		/* Return status */
		return ret_status;
	}

	static void destroy( TaskHandle_t * const p_task_handle )
	{
		vTaskDelete( *p_task_handle );
	}

/* Private Members */
private:
};


#endif /* RTOS_WRAPPERS_H_ */

/********************************** End of File *******************************/
