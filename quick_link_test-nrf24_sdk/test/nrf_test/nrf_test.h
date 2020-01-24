#ifndef _NRF_TEST_H
#define _NRF_TEST_H 

#include "hal_nrf.h"

#define TX_AW     5
#define TX_PL_W   32

#define MOD_ADR_WIDTH  5
#define MOD_PL_LENGTH  32

#define PT_PL_LENGTH 3
#define PT_COM_PACKAGES 1000
#define PT_LOST_PKT  62
#define PT_ADR_WIDTH 3


#define LO_CH  2
#define MID_CH 41
#define HI_CH  81 

#define DEFAULT_TEST_ADDRESS   {12, 13, 14, 15, 16}
#define DEFAULT_TEST_PAYLOAD   {0xB2, 0x43, 0xA1, 0xB3, 0x56, 0x76, 0x23, 0x11,	   \
                                0x87, 0xF9, 0x27, 0x56, 0x90, 0x34, 0x17, 0x92,	   \
                                0xD7, 0x4B, 0x6C, 0xA4, 0x63, 0x41, 0x82, 0x3E,    \
                                0xE1, 0x2A, 0x69, 0x71, 0x58, 0x8B, 0x13, 0x25}



#define DEFAULT_CARRIER_ADDRESS {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#define DEFAULT_CARRIER_PAYLOAD {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   \
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   \
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   \
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  
											   


void test_nrf_l01p_pwr_up(void);

void test_nrf_l01p_start_car(uint8_t power, uint8_t rf_ch);    // Carrier generation with L01P radio

void test_nrf_l01p_stop_car(void);

/*
void test_nrf_L01_car(uint8_t power, uint8_t rf_ch);     // Simulated carrier generation with L01 radio	
*/

void test_nrf_l01_mod_car(uint8_t data_rate, uint8_t power, uint8_t rf_ch);    // Modulated carrier generation for L01/L01P radio

void test_nrf_RX_LO(uint8_t rf_ch);

void test_nrf_l01p_per_init(uint8_t datarate, uint8_t pwr);

void test_nrf_l01p_start_prx_per_test(uint8_t datarate, uint8_t rf_chan);

void test_nrf_l01p_clean_rx_fifo(void);

void test_nrf_prx_prod_test(void);

uint16_t nrf_test_ptx_per_test(uint8_t rf_ch);

bool nrf_test_ptx_hop_channel(uint8_t rf_ch);


#endif