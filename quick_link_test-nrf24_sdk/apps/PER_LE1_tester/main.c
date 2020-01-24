#include "hal_nrf.h"
#include "nrf_test.h"

//#define LE1_32PIN
	#define LE1_48PIN

#ifdef LE1_32PIN
// Reserve P0.3, P0.4 for 2W

sbit B0  = P0^0;                               // BUTTON0 on nRFGo
sbit B1  = P0^1;                               // BUTTON1 on nRFGo
sbit B2  = P0^2;                               // BUTTON2 on nRFGo
sbit B5  = P0^5;
sbit B6  = P0^6;
sbit B7  = P0^7;


sbit LED0 = P1^0;                               // LED0 on nRFGo
sbit LED1 = P1^1;                               // LED1 on nRFGo
sbit LED2 = P1^2;                               // LED2 on nRFGo
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;

#endif

#ifdef LE1_48PIN

sbit B0  = P0^0;                               // BUTTON0 on nRFGo
sbit B1  = P0^1;                               // BUTTON1 on nRFGo
sbit B2  = P0^2;                               // BUTTON2 on nRFGo
sbit B3  = P0^3;
sbit B4  = P0^4;
sbit B5  = P0^5;
sbit B6  = P0^6;
sbit B7  = P0^7;


sbit LED0 = P2^0;                               // LED0 on nRFGo
sbit LED1 = P2^1;                               // LED1 on nRFGo
sbit LED2 = P2^2;                               // LED2 on nRFGo
sbit LED3 = P2^3;
sbit LED4 = P2^4;
sbit LED5 = P2^5;
sbit LED6 = P2^6;
sbit LED7 = P2^7;

#endif

/**
Initialization function the nRF24LE1 microcontroller.

Does the following:
- Enables radio SPI
- Enables radio IRQ
- Sets up GP I/O so that buttons on basic feature board is input and LEDs are output.
- Enables MCU wakeup independent of IEN1 
*/


#define get_buttons() ((~P0) & 0x07)   



#ifdef LE1_32PIN
#define set_leds(byte) (P1 = (byte & 0x07)) 
#endif 

#ifdef LE1_48PIN
#define set_leds(byte) (P2 = (byte & 0x07)) 
#endif 




//-----------------------------------------------------------------------------
// LE1 Receiver test setup
//-----------------------------------------------------------------------------

uint8_t test_address[] = DEFAULT_TEST_ADDRESS;


uint8_t test_payload[] = DEFAULT_TEST_PAYLOAD;

//-----------------------------------------------------------------------------
// Functions Definition
//-----------------------------------------------------------------------------

void  mcu_init();
void  wait_ms(uint16_t cnt);
void  LE1_ptx_prod_test(void);


//-----------------------------------------------------------------------------
// Variables Definition
//-----------------------------------------------------------------------------

// rx data buffer 
static uint8_t* rx_data;



void main(void)
{
  uint8_t  loop_cnt;
  bool     all_fail;
 
  loop_cnt=0;
  all_fail=0;

  mcu_init(); 
  CE_LOW();
  hal_nrf_set_power_mode(HAL_NRF_PWR_UP);                         // Set PWR_UP=1
  wait_ms(2);
  
  set_leds(0);



 
while(1)
{
	
	if(!B0)
	{
      all_fail =0;
	  LE1_ptx_prod_test();
	  if ( !( LED0 | LED1 | LED2))
      
	     all_fail = 1;
     
	}
    else if(!B1)
	  {
	    set_leds(0);
		all_fail=0;
	  }	 

	
	 if (loop_cnt==10)
	 {
		
		LED4^=1;
		if (all_fail)
		{
		   
		    LED0^=1;
		    LED1^=1;
		    LED2^=1;
		   
		}
		loop_cnt=0;
	 }
	 
	 wait_ms(50);
	 loop_cnt++;
}




}


//-----------------------------------------------------------------------------------------
// Local Functions
//-----------------------------------------------------------------------------------------


void mcu_init()
{
  CE_LOW();				                    	// Reset clock control register
  
  RF = 0;                                       // Radio IRQ disable
  RFCKEN = 1;                                   // RF clk enable
 
  P0DIR = 0xFF;								    // Set P0 as input
  
#ifdef LE1_32PIN
  P1DIR = 0;								    // Set P1 as output
  P1 = 0;
#endif  

#ifdef LE1_48PIN  
  P2DIR =0;
  P2=0;
#endif
   
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



void LE1_ptx_prod_test(void)
{
    //uint16_t i;   // for debug purpose only
	
	
		if(nrf_test_ptx_per_test(LO_CH) > (PT_COM_PACKAGES-PT_LOST_PKT) )   LED0=1;
		 
		
		wait_ms(1);
	
	   
		if (nrf_test_ptx_hop_channel(MID_CH))
		{
	  	  wait_ms(4);                                              // Delay 4ms to wait for PRX steady
			  //i = nrf_test_ptx_per_test(MID_CH);                     // for debug purpose only
		    if(nrf_test_ptx_per_test(MID_CH) > (PT_COM_PACKAGES-PT_LOST_PKT) )  LED1=1;
		
		}
	

    if (nrf_test_ptx_hop_channel(HI_CH))
		{
	  	  wait_ms(4);// Delay 4ms to wait for PRX steady
		  if(nrf_test_ptx_per_test(HI_CH) > (PT_COM_PACKAGES-PT_LOST_PKT) )  LED2=1;
					  
		
		}

		nrf_test_ptx_hop_channel(LO_CH);
	
}











