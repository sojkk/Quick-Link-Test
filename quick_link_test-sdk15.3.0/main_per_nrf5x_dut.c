/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRANTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*
* $LastChangedRevision: 15516 $
*/

/** 
 * @file
 * @brief Enhanced Shockburst Transmitter (PRX) example
 * @defgroup esb_prx_example Enhanced Shockburst Transmitter (PRX)
 * @{
 * @ingroup esb_03_examples
 *
 * This project requires that a device running the  @ref esb_prx_example is 
 * used as a counterpart for  receiving the data. This can be on either nRF51 
 * device or a nRF24LE1 device running the esb_prx_example in the nRFgo SDK.
 * 
 * This example listens for a packet and sends an ACK
 * when a packet is received. The contents of the first payload byte of 
 * the received packet is output on the GPIO Port BUTTONS. 
 * The contents of GPIO Port LEDS are sent in the first payload byte (byte 0) 
 * of the ACK packet.
 */


#include "nrf_esb.h"
#include "nrf5x_test.h"


/*****************************************************************************/
/**
* @brief Static functions.
*/
/*****************************************************************************/
static void clock_initialization(void)
{
    /* Start crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }

}

/*****************************************************************************/
/**
* @brief Main function.
* @return ANSI required int return type.
*/
/*****************************************************************************/
int main()
{
	  clock_initialization();

    (void)test_nrf5x_start_prx_per_test(NRF_ESB_BITRATE_2MBPS );
    
    for(;;)
    {
        
    }
}








/** @} */
/** @} */
