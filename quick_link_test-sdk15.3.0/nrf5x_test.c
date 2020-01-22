#include "nrf5x_test.h"
#include "nrf_delay.h"
#include "sdk_common.h"

static uint8_t test_address[] = DEFAULT_TEST_ADDRESS;

static uint8_t test_payload[] = DEFAULT_TEST_PAYLOAD;

static nrf_esb_payload_t       m_payload;

static bool m_radio_isr_flag = false;

static bool m_ack_received = false;

static uint16_t m_ack_count =0;


/*****************************************************************************/
/** Static functions  */
/*****************************************************************************/


static uint32_t test_nrf5x_per_init(nrf_esb_tx_power_t pwr)
{
	
	VERIFY_SUCCESS(nrf_esb_set_tx_power(pwr));
	
	VERIFY_SUCCESS(nrf_esb_set_address_length(PT_ADR_WIDTH));
	
	nrf_esb_set_base_address_0(&test_address[1]);
	
	VERIFY_SUCCESS(nrf_esb_set_prefixes(test_address, 1));
	
	VERIFY_SUCCESS(nrf_esb_enable_pipes(1 << PER_TEST_PIPE));
	
	VERIFY_SUCCESS(nrf_esb_flush_rx());
	
	VERIFY_SUCCESS(nrf_esb_flush_tx());
	
  return NRF_SUCCESS;
}	


static void test_nrf5x_esb_rx_data_ready_isr(void)
{	    
       nrf_esb_payload_t rx_payload;
	
	     rx_payload.length = PT_PL_LENGTH;
	     rx_payload.pipe   = PER_TEST_PIPE;
	     nrf_esb_read_rx_payload(&rx_payload);
	
	
	    if(rx_payload.data[0] < 90)
			{
         nrf_delay_ms(4);       //Delay 4ms to let ACK go before channel switching 				
				
				 nrf_esb_stop_rx();          			
				 nrf_esb_set_rf_channel(rx_payload.data[0]);	
				 nrf_esb_start_rx();
      }				
	     nrf_esb_flush_rx();
}	



static void esb_ptx_event_handler(nrf_esb_evt_t const * p_event)
{
	 m_radio_isr_flag = true;
	
   switch (p_event->evt_id)
   {
		 case NRF_ESB_EVENT_TX_SUCCESS:
			 
		  m_ack_count++;
			m_ack_received = true;
		  break;
           
     case NRF_ESB_EVENT_TX_FAILED:
           
		 m_ack_received = false;
		 nrf_esb_flush_tx();
		 break;
		 
     case NRF_ESB_EVENT_RX_RECEIVED:
			
				
		default:
					
		break;
	 }		
}

static void esb_prx_event_handler(nrf_esb_evt_t const * p_event)
{
  switch (p_event->evt_id)
  {
			  case NRF_ESB_EVENT_TX_SUCCESS:
           
        case NRF_ESB_EVENT_TX_FAILED:
           
        case NRF_ESB_EVENT_RX_RECEIVED:
					
				    test_nrf5x_esb_rx_data_ready_isr();
				
				default:
					
				break;
	}		

}	
	
/*****************************************************************************/
/** PER Test functions  */
/*****************************************************************************/

uint32_t  nrf5x_test_ptx_per_test_init(nrf_esb_bitrate_t datarate)
{
  nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
	nrf_esb_config.bitrate                  = datarate;
  nrf_esb_config.retransmit_delay	        = 250;
  nrf_esb_config.event_handler            = esb_ptx_event_handler;
	nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;
	nrf_esb_config.retransmit_count         = 0;
  nrf_esb_config.payload_length           = PT_PL_LENGTH;	
	
	 // Initialize ESB
   VERIFY_SUCCESS(nrf_esb_init(&nrf_esb_config));

	 VERIFY_SUCCESS(test_nrf5x_per_init(NRF_ESB_TX_POWER_0DBM));
	
	 return NRF_SUCCESS;
}


uint16_t test_nrf5x_ptx_per_test(uint8_t rf_chan)
{
	 uint16_t i,ack_cnt;
	
	 nrf_esb_set_auto_retr(0,250);
	 nrf_esb_set_tx_power(NRF_ESB_TX_POWER_0DBM);
	 nrf_esb_set_rf_channel(rf_chan);
	
	 m_payload.length = PT_PL_LENGTH;
	 m_payload.pipe   = PER_TEST_PIPE;
   memcpy(m_payload.data, test_payload, PT_PL_LENGTH);
	
	  for (i=0;i<PT_COM_PACKAGES;i++)
    {
			m_radio_isr_flag = false;
		  nrf_esb_write_payload(&m_payload);	
			while( !m_radio_isr_flag);			
	  } 
		ack_cnt = m_ack_count;
		m_ack_count = 0;
	
	 return ack_cnt;
}


bool test_nrf5x_ptx_hop_channel(uint8_t rf_chan)
{
	
	 m_ack_received = false;
	 m_radio_isr_flag = false;
	 nrf_esb_set_tx_power(NRF_ESB_TX_POWER_0DBM);
	 nrf_esb_set_auto_retr(5,250);
	 m_payload.length  = 1;
   m_payload.pipe    = 0;
   m_payload.data[0] = rf_chan;
 
	nrf_esb_write_payload(&m_payload);
	
	while( !m_radio_isr_flag);	  
	
	return m_ack_received;
}	

uint32_t test_nrf5x_start_prx_per_test(nrf_esb_bitrate_t datarate)
{

	 
	 nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
	 nrf_esb_config.bitrate                  = datarate;
   nrf_esb_config.event_handler            = esb_prx_event_handler;
	 nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
	 nrf_esb_config.payload_length           = PT_PL_LENGTH;
   nrf_esb_config.selective_auto_ack       = false;
	 
	
   // Initialize ESB
   VERIFY_SUCCESS(nrf_esb_init(&nrf_esb_config));

	 VERIFY_SUCCESS(test_nrf5x_per_init(NRF_ESB_TX_POWER_0DBM));
	
	 //Set RF Channel
	 VERIFY_SUCCESS(nrf_esb_set_rf_channel(LO_CH)); 
		
	
   // Enable ESB to start receiving
   VERIFY_SUCCESS(nrf_esb_start_rx());  
	
	return NRF_SUCCESS;
	
}

