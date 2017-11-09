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




#define NRF24L01_CE_OUT		LPC_GPIO0->FIODIR |= (1<<17)
#define NRF24L01_CE_HIGH	LPC_GPIO0->FIOSET |= (1<<17)
#define NRF24L01_CE_LOW		LPC_GPIO0->FIOCLR |= (1<<17)

#define NRF24L01_CSN_HIGH	LPC_GPIO0->FIOSET |= (1<<6)
#define NRF24L01_CSN_LOW	LPC_GPIO0->FIOCLR |= (1<<6)
#define NRF24L01_CSN_OUT        LPC_GPIO0->FIODIR |= (1<<6)



typedef struct {
  unsigned char CONFIG_R ;
  unsigned char EN_AA_R ;
  unsigned char EN_RXADDR_R ;
  unsigned char SETUP_AW_R ;
  unsigned char SETUP_RETR_R ;
  unsigned char RF_CH_R ;
  unsigned char RF_SETUP_R ;
  unsigned char STATUS_R ;
  unsigned char OBSERVE_TX_R ;
  unsigned char RPD_R ;
  unsigned char FIFO_STATUS_R ;
  unsigned char DYNPD_R ;
  unsigned char FEATURE_R ;
//  unsigned char[5] RX_ADDR_P0 ;
//  unsigned char[5] RX_ADDR_P1 ;
//  unsigned char RX_ADDR_P2 ;
//  unsigned char RX_ADDR_P3 ;
//  unsigned char RX_ADDR_P4 ;
//  unsigned char RX_ADDR_P5 ;
//  unsigned char RX_ADDR_P6 ;
  unsigned char* RX_ADDR_R[6] ;  
  unsigned char RX_PW_R[6] ;
  unsigned char* TX_ADDR_R ;
  
} nRF_REGISTERS;

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

void nrf_power_up();
void interrupt() ;
void nrf_init() ;
void init_system() ;
void init_irq() ;
void init_gpio() ;
void init_ssp() ;
void init_registers() ;
void config_tx_pipe(TX_CONFIG* conf);
void config_rx_pipe(RX_PIPE_CONFIG* conf);
void set_rx_pipe_en(unsigned char n,unsigned char en);
void set_tx_payload(unsigned char* data,unsigned char length);
void nrf_RX_Mode();
void nrf_TX_Mode();
void nrf_power_up();
void nrf_power_down();
void rx_payload(unsigned char* data) ;
void flush_tx();
void write_register(unsigned char command, unsigned char* data, unsigned int length) ;
void read_register(unsigned char command, unsigned char* data, unsigned int length) ;

