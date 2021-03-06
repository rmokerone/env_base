//nrf24l01.h

#define F_CPU 8E6
#include <avr/io.h>
#include <util/delay.h>

#define uchar unsigned char

//快捷方式设置寄存器
#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x),(BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x),(BIT((y))))

//置位CSN
#define CSN_EN SETBIT(PORTB,CSN)
//清零CSN
#define CSN_CLR CLEARBIT(PORTB,CSN)

#define CE_EN SETBIT(PORTB,CE)
#define CE_CLR SETBIT(PORTB,CE)

#ifndef _BYTE_DEF_
#define _BYTE_DEF_

#define BYTE unsigned char 
#define uchar unsigned char 

#define TX_ADR_WIDTH    5   // 5  bytes TX(RX) address width

#define TX_PLOAD_WIDTH  32  // 32 bytes TX payload


#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address #define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define STA_MARK_RX     0X40
#define STA_MARK_TX     0X20
#define STA_MARK_MX     0X10	 
//定义接收缓冲区和发送数据缓冲区，要收发数据时可直接重缓冲区中读取   
uchar rx_buf[TX_PLOAD_WIDTH];
uchar tx_buf[TX_PLOAD_WIDTH];//这个应该是发送数据的缓冲区
uchar flag;


//***************************************************************//
//                   FUNCTION's PROTOTYPES  //
/****************************************************************
 void SPI_Init(BYTE Mode);     // Init HW or SW SPI
 BYTE SPI_RW(BYTE byte);                                // Single SPI read/write
 BYTE SPI_Read(BYTE reg);                               // Read one byte from nRF24L01
 BYTE SPI_RW_Reg(BYTE reg, BYTE byte);                  // Write one byte to register 'reg'
 BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);  // Writes multiply bytes to one register
 BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);   // Read multiply bytes from one register
*****************************************************************/
//void NRF24L01_SPI_Init(BYTE Mode);     // Init HW or SW SPI

void SPI_Init();  // init spi
  uchar SPI_RW(BYTE byte);                                // Single SPI read/write
  uchar SPI_Read(BYTE reg);                               // Read one byte from nRF24L01
  uchar SPI_RW_Reg(BYTE reg, BYTE byte);                  // Write one byte to register 'reg'
  uchar SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);  // Writes multiply bytes to one register
  uchar SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);   // Read multiply bytes from one register
void power_off (void); 
void init_nrf24l01_io(void);
void ifnnrf_rx_mode(unsigned char * TX_ADDRESS);
void ifnnrf_tx_mode(unsigned char * TX_ADDRESS);
void ifnnrf_CLERN_ALL();
//封装好的发送函数
uchar nrf_tx (unsigned char * TX_ADDRESS);
//封装好的接收函数
uchar nrf_rx (unsigned char * TX_ADDRESS);


//定义各个引脚的端口号
#define CE      PB1
#define CSN     PB2
#define MOSI    PB3
#define MISO    PB4
#define SCK     PB5
#define IRQ 	PD2
#endif 
