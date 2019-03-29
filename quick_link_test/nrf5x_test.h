#ifndef _NRF51_TEST_H
#define _NRF51_TEST_H 

#include "nrf_esb.h"

#define TX_AW     5
#define TX_PL_W   32


#define PT_PL_LENGTH 3
#define PT_COM_PACKAGES 1000
#define PT_LOST_PKT  62
#define PT_ADR_WIDTH   3
#define PER_TEST_PIPE 0

#define LO_CH  2
#define MID_CH 41
#define HI_CH  81 

#define DEFAULT_TEST_ADDRESS   {12, 13, 14, 15, 16}

#define DEFAULT_TEST_PAYLOAD   {0xB2, 0x43, 0xA1, 0xB3, 0x56, 0x76, 0x23, 0x11,	   \
                                0x87, 0xF9, 0x27, 0x56, 0x90, 0x34, 0x17, 0x92,	   \
                                0xD7, 0x4B, 0x6C, 0xA4, 0x63, 0x41, 0x82, 0x3E,    \
                                0xE1, 0x2A, 0x69, 0x71, 0x58, 0x8B, 0x13, 0x25}

#define IS_B1_PRESS  !(nrf_gpio_pin_read(BUTTON_1))
#define IS_B2_PRESS  !(nrf_gpio_pin_read(BUTTON_2))
#define IS_LED1_ON   !(nrf_gpio_pin_read(LED_1))
#define IS_LED2_ON   !(nrf_gpio_pin_read(LED_2))
#define IS_LED3_ON   !(nrf_gpio_pin_read(LED_3))
																

typedef void (*callback_t)(nrf_esb_evt_t const *);    //callback function for esb handler


//PER functions
uint32_t nrf5x_test_ptx_per_test_init(nrf_esb_bitrate_t datarate);
uint16_t test_nrf5x_ptx_per_test(uint8_t rf_chan);
bool     test_nrf5x_ptx_hop_channel(uint8_t rf_chan);																
uint32_t test_nrf5x_start_prx_per_test(nrf_esb_bitrate_t datarate);

																

#endif
