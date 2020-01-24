#include "nrf_test.h"



static uint8_t test_address[] = DEFAULT_TEST_ADDRESS;


static uint8_t test_payload[] = DEFAULT_TEST_PAYLOAD;

static uint8_t rx_data[2];

static void Delay100us(uint8_t cnt)
{
  /* Use software loop counter */
  uint8_t delay;
  while(cnt-- > 0)
  {
    for(delay=0;delay<117;delay++);     // 100µs for C8051F320-TB
  }
}

void test_nrf_l01p_pwr_up(void)
{
   hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
   Delay100us(20);

}  

// Carrier generation with L01P radio


void test_nrf_l01p_start_car(uint8_t power, uint8_t rf_ch)
{   	                         
    CE_LOW();
    hal_nrf_set_operation_mode(HAL_NRF_PTX);			  
    hal_nrf_enable_continuous_wave(1); 					   //  Enable CW Mode (bit 7 =1)
	hal_nrf_set_pll_mode(1);                               //  Enable PLL lock  (bit 4 =1)
	hal_nrf_set_output_power(power);                       //  Set Output Power                                               																    
    hal_nrf_set_rf_channel(rf_ch);                         //  Set the RF frequency to the frequency to be measured
	CE_HIGH();										   	 
}


void test_nrf_l01p_stop_car(void)
{   
	CE_LOW();
    hal_nrf_enable_continuous_wave(false);
    hal_nrf_set_pll_mode(false);
    
}	  


// Pseudo carrier generation with L01 radio	

/*
void test_nrf_l01_car(uint8_t power, uint8_t rf_ch)
{   	                         
	CE_LOW();
	hal_nrf_set_operation_mode(HAL_NRF_PTX);
        hal_nrf_open_pipe(HAL_NRF_PIPE0,0);             	        // Enable data pipes 0 but dissble its Auto Ack   
	hal_nrf_set_rf_channel(rf_ch);                                  // Set the RF frequency to the frequency to be measured
        hal_nrf_set_output_power(power );  			// Set Output Power
	hal_nrf_set_pll_mode(1);                 		        // Force PLL locked
	hal_nrf_set_datarate(HAL_NRF_1MBPS);                            // 1Mbps Data rate 
        hal_nrf_set_address(HAL_NRF_TX,carrier_address);                // All '1's in address
        hal_nrf_write_tx_pload(carrier_payload,TX_PL_W);                // All '1's in Payload 
        hal_nrf_set_crc_mode(HAL_NRF_CRC_OFF);                          // Turn off CRC  while keep Power up, PTX
        CE_PULSE();
	Delay100us(10);
	CE_HIGH();							// Pull CE high all the way	
	hal_nrf_reuse_tx();	
										   	 
}
*/
// Modulated carrier generation for L01/L01P radio

void test_nrf_l01_mod_car(uint8_t data_rate, uint8_t power, uint8_t rf_ch)
{
    
	CE_LOW();
	hal_nrf_set_operation_mode(HAL_NRF_PTX);
	hal_nrf_open_pipe(HAL_NRF_PIPE0, 0);						    // Enable data pipes 0 but dissble its Auto Ack   
	hal_nrf_set_output_power(power);  						// Set Output Power
	hal_nrf_set_crc_mode(HAL_NRF_CRC_OFF); 
	hal_nrf_set_address_width(MOD_ADR_WIDTH);
	hal_nrf_set_address(HAL_NRF_TX,test_address);                
    hal_nrf_set_datarate(data_rate);                             
    hal_nrf_set_rf_channel(rf_ch);
    hal_nrf_set_operation_mode(HAL_NRF_PTX);
    hal_nrf_write_tx_payload(test_payload, MOD_PL_LENGTH);
	hal_nrf_reuse_tx();				                                    // Re-use last sent payload while CE=1 
	
		
}


void test_nrf_RX_LO(uint8_t rf_ch)
{
    CE_LOW();                                                       // Pull CE low initially
	hal_nrf_set_operation_mode(HAL_NRF_PRX);
    hal_nrf_open_pipe(HAL_NRF_PIPE0,0);             			    // Enable data pipes 0 but dissble its Auto Ack   
	hal_nrf_set_rf_channel(rf_ch); 
  	CE_HIGH();

}

void test_nrf_l01p_per_init(uint8_t datarate, uint8_t pwr)
{
  CE_LOW();
  
  hal_nrf_enable_dynamic_payload(1);
  hal_nrf_setup_dynamic_payload(0xFF);   

  hal_nrf_set_datarate(datarate);
  
  if(datarate==HAL_NRF_250KBPS)   
     hal_nrf_set_auto_retr(0, 500); 
  else   
	 hal_nrf_set_auto_retr(0, 250); 


  hal_nrf_set_output_power(pwr); 
  hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);    
  hal_nrf_set_address_width(PT_ADR_WIDTH);
  hal_nrf_set_pll_mode(0);    
  hal_nrf_set_address(HAL_NRF_PIPE0, test_address);
  hal_nrf_set_address(HAL_NRF_TX, test_address);
  hal_nrf_open_pipe(HAL_NRF_PIPE0, 1);
  hal_nrf_get_clear_irq_flags(); 
  hal_nrf_flush_rx();
  hal_nrf_flush_tx();
   
}

void test_nrf_l01p_start_prx_per_test(uint8_t datarate, uint8_t rf_chan)
{
  test_nrf_l01p_per_init(datarate,HAL_NRF_0DBM);
 // hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
 // Delay100us(20);   				                   // Delay 2 ms (this must be customized for MCU)
  CE_LOW();
  hal_nrf_set_rf_channel(rf_chan);
  hal_nrf_set_operation_mode(HAL_NRF_PRX);
  CE_HIGH();
}

void test_nrf_l01p_clean_rx_fifo(void)
{
   uint16_t int_temp;     	     
	
   if(RFF)
   {
	 RFF=0;
	 int_temp = hal_nrf_get_clear_irq_flags();
	 if(int_temp == (1<<RX_DR))
	 {
	   hal_nrf_flush_rx();
	 }
   }	 
}

void test_nrf_prx_prod_test(void)
{
  uint16_t int_temp;      
 
	 if(RFF)
	 {
	   RFF=0;
	   int_temp = hal_nrf_get_clear_irq_flags();
	   if(int_temp == (1<<RX_DR))
	   {
         
		 hal_nrf_read_rx_payload(rx_data); 	
		   
		 if (rx_data[0] < 90)
	
		  {
			  
			  Delay100us(40);  						    //Delay 4ms to let ACK go before channel switching
			  
			  CE_LOW();
			  hal_nrf_set_rf_channel(rx_data[0]);
			  hal_nrf_flush_rx(); 
			  CE_HIGH();
			  
		  }
		  else 
		  {
			 hal_nrf_flush_rx();
		  }
	   }

	 } 
	     
}

uint16_t nrf_test_ptx_per_test(uint8_t rf_ch)
{
   uint16_t i,ack_cnt;
   uint8_t  int_temp;
  
   ack_cnt=0;
   
   //test_nrf_l01p_per_init(HAL_NRF_2MBPS, HAL_NRF_18DBM);
	test_nrf_l01p_per_init(HAL_NRF_2MBPS, HAL_NRF_0DBM);
   
   hal_nrf_set_rf_channel(rf_ch);
  
   hal_nrf_set_operation_mode(HAL_NRF_PTX);
   hal_nrf_write_tx_payload(test_payload, PT_PL_LENGTH);
   hal_nrf_get_clear_irq_flags();
  
   for (i=0;i<PT_COM_PACKAGES;i++)
   {
    
        hal_nrf_reuse_tx();
		CE_PULSE();
	  
        while(!RFF);
        RFF = 0;    
 	    int_temp = hal_nrf_get_clear_irq_flags();
		if(int_temp == (1 << TX_DS))
        {
		        ack_cnt++;
        } 
        else if(int_temp == (1 << MAX_RT))
        {

        }
				//Modify: 8/31/2015 Add Delay for nRF51 ESB
        Delay100us(1);
	                 
   }

  return ack_cnt;
}

bool nrf_test_ptx_hop_channel(uint8_t rf_ch)
{
    uint8_t int_temp;  
    uint8_t hop_ch[1]; 	
	
	CE_LOW();
    hop_ch[0] = rf_ch;
	hal_nrf_set_output_power(HAL_NRF_0DBM); 
	hal_nrf_set_auto_retr(10, 250); 
    hal_nrf_set_operation_mode(HAL_NRF_PTX);
    hal_nrf_write_tx_payload(hop_ch, 1);
    hal_nrf_get_clear_irq_flags();
    CE_PULSE();
    while(!RFF);
    RFF = 0;    
 	int_temp = hal_nrf_get_clear_irq_flags();
	if (int_temp == (1 << TX_DS))
	{
	  return 1;									  //indicates that it succeeds to hop
	}
    else if(int_temp == (1 << MAX_RT))
    {
	 
	  return 0;		                              //indicates that it fails to hop
	  
    }    	
	
    return 0; 						             // default 0
}
				