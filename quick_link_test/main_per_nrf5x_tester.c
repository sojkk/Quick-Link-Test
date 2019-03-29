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
#include "boards.h"
#include "nrf_delay.h"



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

static void mcu_init(void)
{
  uint8_t i;
	
	clock_initialization();
	
	for(i=LED_START; i<=LED_STOP; i++) 
	{
	  nrf_gpio_pin_set(i);    // Set Output port high
	  nrf_gpio_cfg_output(i);
	}	
	 
	nrf_gpio_range_cfg_input(BUTTON_START,BUTTON_STOP,BUTTON_PULL);
	
}


static void app_ptx_prod_test(void)
{
	 //uint16_t i;  //for debug purpose only
	
	 //i = test_nrf5x_ptx_per_test(LO_CH);
	
   if(test_nrf5x_ptx_per_test(LO_CH) > (PT_COM_PACKAGES-PT_LOST_PKT) )  nrf_gpio_pin_clear(LED_1);
	
	 nrf_delay_ms(1);                         
	
	 if(test_nrf5x_ptx_hop_channel(MID_CH))   
	 {
      nrf_delay_ms(4);      // Delay 4ms to wait for PRX steady
		  //i = test_nrf5x_ptx_per_test(MID_CH);
		  if(test_nrf5x_ptx_per_test(MID_CH) > (PT_COM_PACKAGES-PT_LOST_PKT) )  nrf_gpio_pin_clear(LED_2);
   }		

   if(test_nrf5x_ptx_hop_channel(HI_CH))   
	 {
      nrf_delay_ms(4);      // Delay 4ms to wait for PRX steady
		  if(test_nrf5x_ptx_per_test(HI_CH) > (PT_COM_PACKAGES-PT_LOST_PKT) )  nrf_gpio_pin_clear(LED_3);
   }		 	 
	 
	 test_nrf5x_ptx_hop_channel(LO_CH);
}

/*****************************************************************************/
/**
* @brief Main function.
* @return ANSI required int return type.
*/
/*****************************************************************************/
int main()
{
	  uint8_t  i,loop_cnt;
    bool     all_fail;
 
    loop_cnt=0;
    all_fail=0;
	
	  mcu_init();
    nrf5x_test_ptx_per_test_init(NRF_ESB_BITRATE_2MBPS);
	  nrf_delay_ms(2);
    
    for(;;)
    {
        if(IS_B1_PRESS)
				{
             all_fail = false;
					   app_ptx_prod_test();
					   if ( !(IS_LED1_ON | IS_LED2_ON | IS_LED3_ON) )   	
				     {
                all_fail = true;
             }					
        }	
				else if (IS_B2_PRESS)
        {
          	for(i=LED_START; i<=LED_STOP; i++) 
	          {
	             nrf_gpio_pin_set(i);
	          }
					  all_fail =false;
        }	

       if (loop_cnt==10)
	     {
		
	      	nrf_gpio_pin_toggle(LED_4);
		 
		      if (all_fail)
		      {	   
			       nrf_gpio_pin_toggle(LED_1);
			       nrf_gpio_pin_toggle(LED_2);
			       nrf_gpio_pin_toggle(LED_3);		  
		      }
		      loop_cnt=0;
	     }
	 
	     nrf_delay_ms(50);
	     loop_cnt++;				
    }
}








/** @} */
/** @} */
