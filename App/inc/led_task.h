/*********************************************************************************************************************
 * @file	led_task.h
 * @brief	LED Flasher Task Header
 * @version	1.0.0
 * @date 	01/18/2020
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

#ifndef LED_TASK_HPP_
#define LED_TASK_HPP_


/******************************************************************************
* Includes
*******************************************************************************/

/* Include class headers */
#include <dri_gpio.h>

/* Include FreeRTOS Headers */
#include <FreeRTOS.h>
#include <task.h>


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

/* Parameters for task creation */
typedef struct TaskCreationParams_
{
	char * p_name;
	configSTACK_DEPTH_TYPE stackDepth;
	UBaseType_t priority;
}TaskCreationParams;


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
 *
 * @brief	Class for LED_Task__ Instance
 *
 ******************************************************************************/
/* Define a class for LED task */
class LedTask__
{
public:
	/** Constructors and Destructors */
	explicit LedTask__( XMC_GPIO_PORT_t * port, uint8_t pin, TaskCreationParams *param ) :
		xLedPort(port), xLedPin(pin), xHandle(NULL)
	{
		xLedTaskParams.p_name = param->p_name;
		xLedTaskParams.priority = param->priority;
		xLedTaskParams.stackDepth = param->stackDepth;
	}

	LedTask__() = delete;
	LedTask__( const LedTask__ & ) = delete;
	LedTask__ operator=( const LedTask__& ) = delete;

	/* Destructor for Led task class */
	virtual ~LedTask__()
	{
		;/* Do Nothing for now
		or Free Dynamic Allocated Memory (if any) */
		vTaskDelete( xHandle );
	}

	/* Task Initialization Function */
	void xvinit( void );

private:
	/* Port Info */
	volatile XMC_GPIO_PORT_t * xLedPort;
	uint8_t xLedPin;

	/* Task Info */
	TaskCreationParams xLedTaskParams;
	TaskHandle_t xHandle;

	/* Will work as a linker to the actual task function */
	static void xsvTaskEntryPoint( void *params )
	{
		((LedTask__ *) params)->xvTaskEntry(params);
	}

	/* This function contains the actual body of task */
	void xvTaskEntry( void *pvParams );
};


#endif /* LED_TASK_HPP_ */

/********************************** End of File *******************************/
