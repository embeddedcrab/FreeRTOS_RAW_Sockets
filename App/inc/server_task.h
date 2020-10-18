/*********************************************************************************************************************
 * @file	server_task.h
 * @brief	Server Task Header
 * @version
 * @date	01/18/2020
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

#ifndef SERVER_TASK_H_
#define SERVER_TASK_H_


/******************************************************************************
* Includes
*******************************************************************************/
#include <rtos_wrappers.h>
#include <dri_mem.h>

/* Include FreeRTOS-Plus TCP/IP files */
#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/

#define INFINEON_HOST_SERVER_IP_ADDRESS0			192
#define INFINEON_HOST_SERVER_IP_ADDRESS1			168
#define INFINEON_HOST_SERVER_IP_ADDRESS2			1
#define INFINEON_HOST_SERVER_IP_ADDRESS3			9

#define INFINEON_HOST_SERVER_IP_ADDRESS				"192.168.1.9"
#define INFINEON_HOST_SERVER_PORT					5900


/******************************************************************************
* Macros
*******************************************************************************/

/* The maximum time to wait for a closing socket to close. */
#define tcpechoSHUTDOWN_DELAY	( pdMS_TO_TICKS( 5000 ) )


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
 *
 * @brief	Class for ServerTask__ Instance
 *
 ******************************************************************************/

class ServerTask__
{
/* public members */
public:
	/** Constructors and Destructors */
	explicit ServerTask__( const UBaseType_t& uxPriority, const configSTACK_DEPTH_TYPE& usStackDepth )
	{
		(void) RTOS_WRAPPER__::create( ServerTask__::server_entry,
										xtask_name, usStackDepth, NULL,
										uxPriority, xhandle );
	}

	ServerTask__() = delete;
	ServerTask__( const ServerTask__& ) = delete;
	ServerTask__ operator=( const ServerTask__& ) = delete;

	virtual ~ServerTask__()
	{
		/* Add definition for Destructor */
		RTOS_WRAPPER__::destroy( xhandle );
	}

	/******* API Member functions *******/

	/**
	 * @function
	 *
	 * @brief
	 *
	 * @param[in]
	 *
	 * @param[out]
	 *
	 * @return
	 *
	 * \par<b>Description:</b><br>
	 *
	 * <i>Imp Note:</i>
	 *
	 */


/* private members */
private:
	/** Data Members **/
	const char * const xtask_name = "ServerTask";
	TaskHandle_t * xhandle = nullptr;

	/******* Private Member Functions *******/
	static void server_entry( void * pvParameters )
	{
		/* Pass the control to server body */
		((ServerTask__ *) pvParameters)->server_body( pvParameters );
	}

	/**
	 * @function	server_body
	 *
	 * @brief		This function contains server body which waits for connection
	 * 				and create a handler task for particular client
	 *
	 * <i>Imp Note:</i>
	 *
	 */
	void server_body( void *pvParameters );

	static void server_handler_entry( void * pvParameters )
	{
		/* Pass the control to server handler body */
		((ServerTask__ *) pvParameters)->server_handler_body( pvParameters );
	}

	void server_handler_body( void * pvParameters );

/* protected members, if any */
protected:

};



/******************************************************************************
* Variables
*******************************************************************************/


#endif /* SERVER_TASK_H_ */

/********************************** End of File *******************************/
