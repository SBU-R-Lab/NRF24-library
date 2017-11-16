#ifndef _NRF24_H
#define _NRF24_h

/*
 * Commands
 */
#define R_REGISTER  0x00
#define W_REGISTER  0x20
#define R_RX_PAYLOAD    0x61
#define W_TX_PAYLOAD    0xA0
#define FLUSH_TX    0xE1
#define FLUSH_RX    0xE2
#define REUSE_TX_PL 0xE3
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD   0xA8
#define W_TX_PAYLOAD_NOACK  0xB0
#define NOP 0xFF

/*
 *  Registers
 */
#define CONFIG  0x00
#define EN_AA   0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH   0x05
#define RF_SETUP    0x06
#define STATUS  0x07
#define OBSERVE_TX  0x08
#define RPD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D


/*
 * CONFIG
 */
#define PRIM_RX 0
#define PWR_UP  1
#define CRCO    2
#define EN_CRC  3
#define MASK_MAX_RT 4
#define MASK_TX_DS  5
#define MASK_RX_DR  6


/*
 * EN_AA
 */
#define ENAA_P0 0
#define ENAA_P1 1
#define ENAA_P2 2
#define ENAA_P3 3
#define ENAA_P4 4
#define ENAA_P5 5

/*
 * EN_RXADDR
 */
#define ERX_P0  0
#define ERX_P1  1
#define ERX_P2  2
#define ERX_P3  3
#define ERX_P4  4
#define ERX_P5  5

/*
 * SETUP_AW
 */
#define AW 0

/*
 * SETUP_RETR
 */
#define ARC 0
#define ARD 4

/*
 * RF_SETUP
 */
#define RF_PWR      1
#define RF_DR_HIGH  3
#define PLL_LOCK    4
#define RF_DR_LOW   5
#define CONT_WAVE   7

/*
 * STATUS
 */
#define STATUS_TX_FULL     0
#define RX_P_NO     1
#define MAX_RT      4
#define TX_DS       5
#define RX_DR       6

/*
 * OBSERVE_TX
 */
#define ARC_CNT     0
#define PLOC_CNT    4

/*
 * FIFO_STATUS
 */
#define RX_EMPTY    0
#define RX_FULL     1
#define TX_EMPTY    4
#define FIFO_STATUS_TX_FULL 5
#define TX_REUSE    6

/*
 * DYNPD
 */
#define DPL_P0      0
#define DPL_P1      1
#define DPL_P2      2
#define DPL_P3      3
#define DPL_P4      4
#define DPL_P5      5

/*
 * FEATURE
 */
#define EN_DYN_ACK  0
#define EN_ACK_PAY  1
#define EN_DPL      2



/**************  Structures  ****************/
typedef  struct {
  unsigned char pipe_number ;
  unsigned char* addr ;
  unsigned char en_ack ;
  unsigned char paylaod_size ;
  unsigned char en_dynpld ;
  unsigned char* recieve_buffer ;
  unsigned char* transmit_buffer ;
} RX_PIPE_CONFIG ; 

typedef  struct {
  unsigned char setup_retr ;
  unsigned char* tx_addr ;
} TX_CONFIG ; 

typedef struct {
  unsigned char CE_PIN;
  unsigned char IRQ_PIN;
} nRF_CONFIG;
/*********************************************/


/**********  Macros and Definitions  *********/
#define BV(x) (1 << x)
#define SetBit(x, y)  x | BV(y)
#define ClrBit(x, y)  x & ~BV(y)

#define CalcRetransmitDelay(x)  (x / 250) - 1

#define RX  1
#define TX  0

#define POWER_UP  1
#define POWER_DOWN  0

#define ENABLE  1
#define DISABLE 0

#define CRC_16BIT 1
#define CRC_8BIT  0

#define RATE_250KBPS  2
#define RATE_1MBPS  0
#define RATE_2MBPS  1

#define OUTPUT_18dBm 0
#define OUTPUT_12dBm 1
#define OUTPUT_6dBm 2
#define OUTPUT_0dBm 3

#define NORMAL  0
#define ACK_PAYLOAD 1
#define NOACK_PAYLOAD 2
/*********************************************/


/*********  Configuration functions  *********/
void nrf_init(void);
void nrf_interrupt(void);
void nrf_set_mode(unsigned char mode);
void nrf_set_power_state(unsigned char power);
void nrf_config_crc(unsigned char enable, unsigned char scheme);
void nrf_set_retries(unsigned short delay, unsigned char count);
void nrf_set_address_width(unsigned char width);
void nrf_set_rf_channel(unsigned char channel);
void nrf_set_data_rate(unsigned char rate);
void nrf_set_output_power(unsigned char power);
void nrf_clear_interrupt_flags(void);
unsigned char nrf_get_status(void);
/*********************************************/

/************  Pipeline functions  ***********/
void nrf_config_tx_pipe(TX_CONFIG* conf);
void nrf_config_rx_pipe(RX_PIPE_CONFIG* conf);
void nrf_set_rx_pipe_en(unsigned char n,unsigned char en);

unsigned char nrf_read_rx_payload_size(void);
void nrf_read_rx_payload(unsigned char* data, unsigned char length);
void nrf_write_tx_payload(unsigned char mode, unsigned char length, unsigned char* data);
void nrf_flush_tx(void);
void nrf_flush_rx(void);
/*********************************************/


#endif