#include "hal_nrf.h"
#include "nrf_test.h"

//-----------------------------------------------------------------------------
// Local Functions
//-----------------------------------------------------------------------------


void mcu_init()
{
	RF = 0;                               // Radio IRQ disable
	CKCON = 0x02;                         // nRF24LU1p PAN 011 #2
  TICKDV = 0;
  CE_LOW();				                    	// Reset clock control register
  RFCTL = 0x10;                         // RF SPI Enable                                       
  RFCKEN = 1;                           // RF clk enable
  
}



void wait_ms(uint16_t cnt)
{
                           
  TMOD |= 0x01;                                 // T0:Mode1, 16-bit timer.
  TR0 = 1;                                      // Start timer0...

  while (cnt > 0)
  {	 
     TH0 = 0xFA;                               // Init T0 value = 0xFFFF - 0x0535 (1333)                  
     TL0 = 0xCA;
	 while (TF0!=1)
	  ;
	 TF0=0;	  
	 cnt--;
   }

  TR0 = 0;                                      // Stop Timer0...	 
}



//-----------------------------------------------------------------------------
// LE1 Receiver test setup
//-----------------------------------------------------------------------------



void main(void)
{
  mcu_init(); 
  CE_LOW();
  hal_nrf_set_power_mode(HAL_NRF_PWR_UP);                         // Set PWR_UP=1
  wait_ms(2);	
  test_nrf_l01p_start_prx_per_test(HAL_NRF_2MBPS,LO_CH);  

  while(1)
  {
     test_nrf_prx_prod_test();     // PROD_TEST_DUT    
  }
				
}




