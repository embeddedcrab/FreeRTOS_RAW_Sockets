/*****************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		SCU Configuration Source
* Filename:		dri_scu_conf.c
* Author:		HS
* Origin Date:	02/23/2020
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_scu_conf.c
 *  @brief:	This source file contains configuration function definitions for SCU
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


/*****************************************************************************/

/* Callback handler for user */
#if (SCU_CALLBACK_HANDLER_USED == 1U)
extern void SCU_callback_handler( void );
#endif

/* Trap Hanlder for SCU Trap Events */
void SCU_Trap_Handler( void );



/** SCU Configurations **/
tStSCU_InterruptConfiguartion SCU_InterruptConfiguration =
{
	.irq = { .irq_num = SCU_0_IRQn, .irq_priority = 40 }
};

#if (USING_WATCHDOG == 1U)
XMC_WDT_CONFIG_t SCU_WDT_Configuration =
{
	.window_upper_bound = 243902,	/* ~1s (so LED Flasher can authenticate
								functioning of system, Fwdt = Fofi/100 = 24MHz/100 */
	.window_lower_bound = 0,	/* 0s, making range of 0 - ~1s */
	.prewarn_mode = 1,
	.run_in_debug_mode = 0,
	.service_pulse_width = 0
};
#endif	/* USING_WATCHDOG */

tStSCU_GlobalConfiguartion SCU_GlobalConfiguration =
{
	.p_isr_config = &SCU_InterruptConfiguration,
	.traps = XMC_SCU_TRAP_OSC_WDG |
			XMC_SCU_TRAP_VCO_LOCK |
			XMC_SCU_TRAP_USB_VCO_LOCK |
			XMC_SCU_TRAP_ULP_WDG |
			XMC_SCU_TRAP_PER_BRIDGE0 |
			XMC_SCU_TRAP_PER_BRIDGE1
			#if (USING_BROWN_OUT_DETECT == 1U)
			| XMC_SCU_TRAP_BROWNOUT,
			#else
			,
			#endif
	.trap_handler = &SCU_Trap_Handler,
	.nmi_request = XMC_SCU_NMIREQ_WDT_WARN |
					XMC_SCU_NMIREQ_ERU0_0 |
					XMC_SCU_NMIREQ_ERU0_1 |
					XMC_SCU_NMIREQ_ERU0_2 |
					XMC_SCU_NMIREQ_ERU0_3,

	#if (USING_WATCHDOG == 1U)
	.p_wdt_config = &SCU_WDT_Configuration,
	.wdt_clock_div = 100,
	.wdt_servie_pulse_width = 16,/* (SPW + 1) */
	.events = XMC_SCU_INTERRUPT_EVENT_WDT_WARN,
	.wdt_clock = XMC_SCU_CLOCK_WDTCLKSRC_OFI,
	.wdt_mode = XMC_WDT_MODE_PREWARNING,
	#endif
	#if (USING_EXTERNAL_CLOCK_OUTPUT == 1U)
	.external_output_clock = XMC_SCU_CLOCK_EXTOUTCLKSRC_PLL,/* Clock division also needed */
	#endif
	#if (USING_BROWN_OUT_DETECT == 1U)
	.threshold = 0x7D,/* VADC33V = (VDDP/LSB33V) + 1, where VDDP=2.8V and LSB33V=22.5mV */
	.interval = 255
	#endif
};

tStSCU_Control SCU_Control;

#if (SCU_CALLBACK_HANDLER_USED == 1U)
tStSCU_Callbacks SCU_Callbacks =
{
	.fp_callback = SCU_callback_handler
};
#endif

tStSCU SCU =
{
	.p_module_config = &SCU_GlobalConfiguration,
	.p_control = &SCU_Control
	#if (SCU_CALLBACK_HANDLER_USED == 1U)
	,.p_callback = &SCU_Callbacks
	#endif
};


/******************************************************************************
* Function Definitions
*******************************************************************************/


void SCU_Interrupt_Handler( void )
{
	XMC_SCU_IRQHandler( 0 );
}


/* Trap Handler for SCU Traps */
void SCU_Trap_Handler( void )
{
	/* TODO: Debug and Understand first */
	__asm("NOP");
}


/*********************************** End of File ******************************/
