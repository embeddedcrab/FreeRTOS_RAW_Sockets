/******************************************************************************
* Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		Interrupt Handlers Source
* Filename:		interrupts.c
* Author:		HS
* Origin Date:
* Version:
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	interrupts.c
 *  @brief:	This source file contains Exception handlers
 */


/******************************************************************************
* Includes
*******************************************************************************/

/* Include SCU Header */
#include <xmc_scu.h>
#include <xmc_gpio.h>

/* Include Peripherals Headers */
#include <dri_scu_extern.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

#define EXCEPTION_HANDLER_DEBUG_ON			0U


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Functions Declarations
*******************************************************************************/

/* Exception Handling functions */
#if (EXCEPTION_HANDLER_DEBUG_ON == 1U)
void get_registers_from_stack( unsigned long *pulFaultStackAddress );
#endif	/* EXCEPTION_HANDLER_DEBUG_ON */

void BusFault_Handler( void );
void HardFault_Handler( void );
void MemManage_Handler( void );
void UsageFault_Handler( void );


void NMI_Handler( void );



/******************************************************************************/

/******************************************************************************
* Functions Definitions
*******************************************************************************/


#if (EXCEPTION_HANDLER_DEBUG_ON == 1U)

void get_registers_from_stack( unsigned long *pulFaultStackAddress )
{
	/* These are volatile to try and prevent the compiler/linker optimizing them
	 away as the variables never actually get used.  If the debugger won't show the
	 values of the variables, make them global my moving their declaration outside
	 of this function. */
	volatile unsigned long r0;
	volatile unsigned long r1;
	volatile unsigned long r2;
	volatile unsigned long r3;
	volatile unsigned long r12;
	volatile unsigned long lr;	/**< Link register. */
	volatile unsigned long pc;	/**< Program counter. */
	volatile unsigned long psr;	/**< Program status register. */

	/* Assign Register values into local variables */
	r0 = pulFaultStackAddress[0];
	r1 = pulFaultStackAddress[1];
	r2 = pulFaultStackAddress[2];
	r3 = pulFaultStackAddress[3];

	r12 = pulFaultStackAddress[4];
	lr = pulFaultStackAddress[5];
	pc = pulFaultStackAddress[6];
	psr = pulFaultStackAddress[7];

	/* When the following line is hit, the variables contain the register values. */
	for( ; ; );

	/* Avoid compiler warnings */
	(void) r0; (void) r1; (void) r2; (void) r3; (void) r12;
	(void) lr; (void) pc; (void) psr;
}

#endif	/* EXCEPTION_HANDLER_DEBUG_ON */

void BusFault_Handler(void)
{
#if (EXCEPTION_HANDLER_DEBUG_ON == 1U)
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" ldr r1, [r0, #24]                                         \n"
			" ldr r2, bus_fault_handler_address_const                   \n"
			" bx r2                                                     \n"
			" bus_fault_handler_address_const: .word get_registers_from_stack    \n"
	);
#endif
	for( ; ; );
}

void HardFault_Handler(void)
{
#if (EXCEPTION_HANDLER_DEBUG_ON == 1U)
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" ldr r1, [r0, #24]                                         \n"
			" ldr r2, hard_fault_handler_address_const                  \n"
			" bx r2                                                     \n"
			" hard_fault_handler_address_const: .word get_registers_from_stack    \n"
	);
#endif	/* EXCEPTION_HANDLER_DEBUG_ON */
	for( ; ; );
}

void MemManage_Handler(void)
{
#if (EXCEPTION_HANDLER_DEBUG_ON == 1U)
    __asm volatile
        (
         " tst lr, #4                                                \n"
         " ite eq                                                    \n"
         " mrseq r0, msp                                             \n"
         " mrsne r0, psp                                             \n"
         " ldr r1, [r0, #24]                                         \n"
         " ldr r2, mem_manage_handler_address_const                  \n"
         " bx r2                                                     \n"
         " mem_manage_handler_address_const: .word get_registers_from_stack    \n"
        );
#endif	/* EXCEPTION_HANDLER_DEBUG_ON */
    for( ; ; );
}


void UsageFault_Handler(void)
{
#if (EXCEPTION_HANDLER_DEBUG_ON == 1U)
    __asm volatile
        (
         " tst lr, #4                                                \n"
         " ite eq                                                    \n"
         " mrseq r0, msp                                             \n"
         " mrsne r0, psp                                             \n"
         " ldr r1, [r0, #24]                                         \n"
         " ldr r2, usage_fault_handler_address_const                  \n"
         " bx r2                                                     \n"
         " usage_fault_handler_address_const: .word get_registers_from_stack    \n"
        );
#endif	/* EXCEPTION_HANDLER_DEBUG_ON */
    for( ; ; );
}



/** NMI Handler for NMI Traps **/
void NMI_Handler( void )
{
	/* Local Variables */
	unsigned long status_l = 0;

	#if (USING_BROWN_OUT_DETECT == 1U)
	float power_l = XMC_SCU_POWER_GetEVR33Voltage();
	/* Check System Power */
	if( power_l < BROWN_OUT_DETECT_VOLTAGE )
	{
		XMC_GPIO_ToggleOutput( (XMC_GPIO_PORT_t *)XMC_GPIO_PORT1, 1 );
		/* We have got a Brown-Out trap, clear it for now! */
		XMC_SCU_TRAP_ClearStatus( status_l );
	}
	#endif	/* USING_BROWN_OUT_DETECT */

	/* Get SCU Interrupt status */
	status_l = XMC_SCU_INTERUPT_GetEventStatus();
	/* Watchdog Trap Pre-Warn occured */
	if( status_l & SCU_INTERRUPT_SRRAW_PRWARN_Msk )
	{
		__asm("NOP");
	}
	/* RTC Alarm has occured */
	if( status_l & SCU_INTERRUPT_SRRAW_AI_Msk )
	{
		/* TODO: (Update) Ring a Bell using DAC, LED used for test indication (not to be used in projects) */
//		XMC_GPIO_SetOutputLevel( (XMC_GPIO_PORT_t *)XMC_GPIO_PORT1, 1, XMC_GPIO_OUTPUT_LEVEL_HIGH );
		XMC_SCU_INTERRUPT_ClearEventStatus( XMC_SCU_NMIREQ_RTC_AI );
	}

	/* Get SCU Trap status register, check specific trap status */
	status_l = XMC_SCU_TRAP_GetStatus();
}


/********************************** End of File *******************************/
