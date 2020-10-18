/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		SCU API Source
* Filename:		dri_scu.c
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_scu.c
 *  @brief:	This header file contains API function definitions for SCU
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_scu.h>


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
long SCU_init( void * p_channel, const unsigned char init )
{
	/* Local Varibales */
	long ret_status = SCU_STATUS_FAILED;

	tStSCU_GlobalConfiguartion * p_config_l = ((tStSCU *)p_channel)->p_module_config;
	tStSCU_Control * p_control_l = ((tStSCU *)p_channel)->p_control;

	/* Check init */
	switch( init )
	{
		/* Initialize SCU */
		case 1:
//			XMC_SCU_CLOCK_Init( (const XMC_SCU_CLOCK_CONFIG_t *const) p_config_l->p_config );
			/* Enable Traps */
			XMC_SCU_TRAP_Enable( p_config_l->traps );
			XMC_SCU_INTERRUPT_EnableNmiRequest( p_config_l->nmi_request );
			XMC_SCU_INTERRUPT_SetEventHandler( 0, p_config_l->trap_handler );

			/* Enable Events */
			#if (USING_WATCHDOG == 1U)
			XMC_SCU_INTERRUPT_EnableEvent( p_config_l->events );
			XMC_SCU_CLOCK_SetWdtClockDivider( p_config_l->wdt_clock_div );
			XMC_SCU_CLOCK_SetWdtClockSource( p_config_l->wdt_clock );
			XMC_WDT_SetMode( p_config_l->wdt_mode );
			XMC_WDT_SetServicePulseWidth( p_config_l->wdt_servie_pulse_width );
			XMC_WDT_Init( (const XMC_WDT_CONFIG_t *const)p_config_l->p_wdt_config );
			#endif
			/* External Clock Output */
			#if (USING_EXTERNAL_CLOCK_OUTPUT == 1U)
			XMC_SCU_CLOCK_SetExternalOutputClockSource( p_config_l->external_output_clock );
			/* ENable output pins as well */
			#endif
			/* Enable Monitor */
			#if (USING_BROWN_OUT_DETECT == 1U)
			XMC_SCU_POWER_EnableMonitor( p_config_l->threshold, p_config_l->interval );
			#endif

			/* Configure NVIC */
			NVIC_SetPriority( (IRQn_Type)p_config_l->p_isr_config->irq.irq_num,
								NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
								(IRQn_Type)p_config_l->p_isr_config->irq.irq_priority, 0U) );

			/* Update control parameters */
			p_control_l->state.init = 1;
		break;
		/* De-initialize SCU */
		case 0:
			p_control_l->state.init = 0;
		break;
		/* Invalid Arguments */
		default:
		ret_status = SCU_STATUS_INVALID_ARGUMENT;
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
