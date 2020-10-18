/******************************************************************************
 * Copyright (c) 2019 - Hemant Sharma - All Rights Reserved
*
* Feel free to use this Code at your own risk for your own purposes.
*
*******************************************************************************/
/******************************************************************************
* Title:		GPIO API Source
* Filename:		dri_gpio_pins.cpp
* Author:		HS
* Origin Date:	10/01/2019
* Version:		1.0.0
* Notes:
*
* Change History
* --------------
*
*******************************************************************************/

/** @file:	dri_gpio_pins.cpp
 *  @brief:	This header file contains API function definitions for
 *  		GPIO pins information.
 */


/******************************************************************************
* Includes
*******************************************************************************/
#include <dri_gpio_pins.h>


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/


/******************************************************************************
* Configuration Constants
*******************************************************************************/

#define UART_USED
#define CAN_USED
#define ETHERNET_USED


/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/

/*
 * @brief Array size used for Pin configuration
 */
static constexpr auto sg_pins_array_size (4);

/*
 * @brief Structure array used for Pin configuration
 */
static constexpr tStPinConfig sg_gpio_pins[::sg_pins_array_size] =
{
	/** GPIO LEDs **/
	{/**< Port information for LED GPIO Pin 0 on Port 1 */
		XMC_GPIO_PORT1,
		{
			XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
			XMC_GPIO_OUTPUT_LEVEL_LOW,
			XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE
		},
		0
	},
	{/**< Port information for LED GPIO Pin 1 on Port 1 */
		XMC_GPIO_PORT1,
		{
			XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
			XMC_GPIO_OUTPUT_LEVEL_LOW,
			XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE
		},
		1
	},
	/** UART over USIC pins **/
	{/**< Port information for UART RX0 Pin 4 on Port 1, U0C0 */
		XMC_GPIO_PORT1,
		{
			XMC_GPIO_MODE_INPUT_TRISTATE,
			XMC_GPIO_OUTPUT_LEVEL_HIGH,
			XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
		},
		4
	},
	{/**< Port information for UART TX0 Pin 5 on Port 1, U0C0 */
		XMC_GPIO_PORT1,
		{
			XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
			XMC_GPIO_OUTPUT_LEVEL_HIGH,
			XMC_GPIO_OUTPUT_STRENGTH_STRONG_SOFT_EDGE
		},
		5
	}
};


#ifdef CAN_USED
/*
 * @brief Array size used for CAN Pin configuration
 */
static constexpr auto sg_can_gpio_pins_size (4);

/*
 * @brief Structure array used for CAN Pin configuration
 */
static constexpr tStPinConfig sg_can_gpio_pins[::sg_can_gpio_pins_size] =
{
	{/**< CAN Node 1 Tx pin, Pin 12 on Port 1 */
		XMC_GPIO_PORT1,
		{
			.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
			.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
			.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
		},
		12
	},
	{/**< CAN Node 1 Rx pin, Pin 13 on Port 1 */
		XMC_GPIO_PORT1,
		{
			.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
			.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
			.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
		},
		13
	},
	{/**< CAN Node 2 Tx pin, Pin 9 on Port 1 */
		XMC_GPIO_PORT1,
		{
			.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
			.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
			.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
		},
		9
	},
	{/**< CAN Node 2 Rx pin, Pin 8 on Port 1 */
		XMC_GPIO_PORT1,
		{
			.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
			.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
			.output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
		},
		8
	}
};
#endif


#ifdef ETHERNET_USED
/*
 * @brief Array size used for Ethernet RMII Pin configuration
 */
static constexpr auto sg_eth_gpio_pins_size (10);
/*
 * @brief Structure array used for Ethernet Pin configuration
 */
static constexpr tStPinConfig sg_eth_gpio_pins[sg_eth_gpio_pins_size] =
{
    {/**< Port information for ETH_MDIO, Ethernet Channel 0 Pin 0 on Port 2 */
        XMC_GPIO_PORT2,
        {
            XMC_GPIO_MODE_INPUT_TRISTATE,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  /* Default Value */
        },
        0
    },
    {/**< Port information for ETH_RXD0, Ethernet Channel 0 Pin 2 on Port 2 */
        XMC_GPIO_PORT2,
        {
            XMC_GPIO_MODE_INPUT_TRISTATE,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  /* Default Value */
        },
        2
    },
    {/**< Port information for ETH_RXD1, Ethernet Channel 0 Pin 3 on Port 2 */
        XMC_GPIO_PORT2,
        {
            XMC_GPIO_MODE_INPUT_TRISTATE,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  /* Default Value */
        },
        3
    },
    {/**< Port information for ETH_RXER, Ethernet Channel 0 Pin 4 on Port 2 */
        XMC_GPIO_PORT2,
        {
            XMC_GPIO_MODE_INPUT_TRISTATE,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  /* Default Value */
        },
        4
    },
    {/**< Port information for ETH_CLK, Ethernet Channel 0 Pin 8 on Port 15 */
        XMC_GPIO_PORT15,
        {
            XMC_GPIO_MODE_INPUT_TRISTATE,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  /* Default Value */
        },
        8
    },
    {/**< Port information for ETH_CRS_DV, Ethernet Channel 0 Pin 9 on Port 15 */
        XMC_GPIO_PORT15,
        {
            XMC_GPIO_MODE_INPUT_TRISTATE,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE  /* Default Value */
        },
        9
    },
    {/**< Port information ETH_MDC for Ethernet Channel 0 Pin 7 on Port 2 */
        XMC_GPIO_PORT2,
        {
            XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
        },
        7
    },
    {/**< Port information for ETH_TXEN Ethernet Channel 0 Pin 5 on Port 2 */
        XMC_GPIO_PORT2,
        {
            XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
            XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
        },
        5
    },
    {/**< Port information for ETH_TXD0 Ethernet Channel 0 Pin 8 on Port 2 */
        XMC_GPIO_PORT2,
        {
        	XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
			XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
        },
        8
    },
    {/**< Port information for ETH_TXD1 Ethernet Channel 0 Pin 9 on Port 2 */
        XMC_GPIO_PORT2,
        {
        	XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
            XMC_GPIO_OUTPUT_LEVEL_LOW,
			XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
        },
        9
    }
};
#endif


/******************************************************************************
* Function Definitions
*******************************************************************************/

/**
 * @function	init
 *
 * @brief		Pins initialization function
 *
 * <i>Imp Note:</i>
 * 				This function has to be called first in main or before main!
 *
 * 	Tasks:
 * 	1. CAN pins needs to be initialized if not working in Loopback mode
 *
 */
void GPIO_Pins__::init( void )
{
	/* Local Variables */
	unsigned char pins = 0;

	/* Initialize Peripherals Pins */
	for( pins = 0; pins < ::sg_pins_array_size; ++pins )
	{
		XMC_GPIO_Init( ::sg_gpio_pins[pins].p_port,
						::sg_gpio_pins[pins].pin,
						 &::sg_gpio_pins[pins].config
		);
	}

	#ifdef ETHERNET_USED
    /* Initialize Ethernet pins */
    for( pins = 0; pins < ::sg_eth_gpio_pins_size; ++pins )
    {
        XMC_GPIO_Init( ::sg_eth_gpio_pins[pins].p_port,
                        ::sg_eth_gpio_pins[pins].pin,
                        &::sg_eth_gpio_pins[pins].config
                        );
    }
    /* Initialize HW functionality for Ethernet pins */
    XMC_GPIO_SetHardwareControl( ::sg_eth_gpio_pins[0].p_port, ::sg_eth_gpio_pins[0].pin,
                            XMC_GPIO_HWCTRL_PERIPHERAL1
                            );
    #endif
}


/* Providing call to pins initialization for C */
extern "C"
{

void gpio_init_c( void );
void gpio_init_c( void )
{
	/* Initialize Hardware pins functionality options */
	GPIO_Pins__::init();
}

}

/********************************** End of File *******************************/
