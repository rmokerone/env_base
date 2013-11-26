#include "STC5A60S2.H"

#include <intrins.h>

#include <string.h>

#include "NRF_24L01.h"

#include "UART.H"

#define uchar unsigned char


uchar const TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address

uchar rx_buf[TX_PLOAD_WIDTH];

uchar tx_buf[TX_PLOAD_WIDTH];//这个应该是发送数据的缓冲区

uchar flag;



/**************************************************/



//sbit LED1= P3^7;
/**************************************************/
unsigned char	bdata sta;

sbit	RX_DR	=sta^6;

sbit	TX_DS	=sta^5;

sbit	MAX_RT	=sta^4;
/**************************************************/

/**************************************************
Function: init_io();
Description:
  flash led one time,chip enable(ready to TX or RX Mode),
  Spi disable,Spi clock line init high
/**************************************************/
#define KEY 0xaa
void init_nrf24l01_io(void)
{
	CE=0;			// chip enable
	CSN=1;			// Spi disable	
	SCK=0;			// Spi clock line init high
}
/**************************************************/

/**************************************************
Function: Inituart();

Description:
  set uart working mode 
/**************************************************/


void delay_ms(unsigned int _ms)
{
  for(_ms;_ms;_ms--)
  {
    _delay_us(1000);
  }
}
/**************************************************/

/**************************************************
Function: SPI_RW();

Description:
  Writes one byte to nRF24L01, and return the byte read
  from nRF24L01 during write, according to SPI protocol
/**************************************************/
bdata unsigned char st=0;
sbit st_1=st^0;
sbit st_2=st^1;
sbit st_3=st^2;
sbit st_4=st^3;
sbit st_5=st^4;
sbit st_6=st^5;
sbit st_7=st^6;
sbit st_8=st^7;
bdata unsigned char st1=0;
sbit st_11=st1^0;
sbit st_12=st1^1;
sbit st_13=st1^2;
sbit st_14=st1^3;
sbit st_15=st1^4;
sbit st_16=st1^5;
sbit st_17=st1^6;
sbit st_18=st1^7;
/*
uchar SPI_RW(uchar byte)
{
	uchar bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit
   	{
   		MOSI = (byte & 0x80);         // output 'byte', MSB to MOSI
   		byte = (byte << 1);           // shift next bit into MSB..
   		SCK = 1;                      // Set SCK high..
		MISO=1;
   		byte |= MISO;       		  // capture current MISO bit
   		SCK = 0;            		  // ..then set SCK low again
   	}
    return(byte);           		  // return read byte
}
*/

uchar SPI_RW(uchar byte)
{
	//uchar bit_ctr;

    st=byte;

    MOSI=st_8;
    SCK = 1;
    st_18=MISO;
    SCK = 0;

    MOSI=st_7;
    SCK = 1;
    st_17=MISO;
    SCK = 0;

    MOSI=st_6;
    SCK = 1;
    st_16=MISO;
    SCK = 0;

    MOSI=st_5;
    SCK = 1;
    st_15=MISO;
    SCK = 0;

    MOSI=st_4;
    SCK = 1;
    st_14=MISO;
    SCK = 0;

    MOSI=st_3;
    SCK = 1;
    st_13=MISO;
    SCK = 0;

    MOSI=st_2;
    SCK = 1;
    st_12=MISO;
    SCK = 0;

    MOSI=st_1;
    SCK = 1;
    st_11=MISO;
    SCK = 0;


    return(st1);           		  // return read byte
}
/**************************************************/

/**************************************************
Function: SPI_RW_Reg();

Description:
  Writes value 'value' to register 'reg'
/**************************************************/
uchar SPI_RW_Reg(BYTE reg, BYTE value)
{
	uchar status;
  	CSN = 0;                   // CSN low, init SPI transaction
  	status = SPI_RW(reg);      // select register
  	SPI_RW(value);             // ..and write value to it..
  	CSN = 1;                   // CSN high again
  	return(status);            // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'
/**************************************************/
BYTE SPI_Read(BYTE reg)
{
	BYTE reg_val;

  	CSN = 0;                // CSN low, initialize SPI communication...
  	SPI_RW(reg);            // Select register to read from..
  	reg_val = SPI_RW(0);    // ..then read registervalue
  	CSN = 1;                // CSN high, terminate SPI communication

  	return(reg_val);        // return register value
}
/**************************************************/

/**************************************************
Function: SPI_Read_Buf();

Description:
  Reads 'bytes' #of bytes from register 'reg'
  Typically used to read RX payload, Rx/Tx address
/**************************************************/
uchar SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	uchar status,byte_ctr;

  	CSN = 0;                    		// Set CSN low, init SPI tranaction
  	status = SPI_RW(reg);       		// Select register to write to and read status byte

  	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0);    // Perform SPI_RW to read byte from nRF24L01

  	CSN = 1;                           // Set CSN high again

  	return(status);                    // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Write_Buf();

Description:
  Writes contents of buffer '*pBuf' to nRF24L01
  Typically used to write TX payload, Rx/Tx address
/**************************************************/
uchar SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	uchar status,byte_ctr;

  	CSN = 0;                   // Set CSN low, init SPI tranaction
  	status = SPI_RW(reg);    // Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	SPI_RW(*pBuf++);
  	CSN = 1;                 // Set CSN high again
  	return(status);          // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: RX_Mode();

Description:
  This function initializes one nRF24L01 device to
  RX Mode, set RX address, writes RX payload width,
  select RF channel, datarate & LNA HCURR.
  After init, CE is toggled high, which means that
  this device is now ready to receive a datapacket.
/**************************************************/
//切换成关机状态，只有在关机状态才能够进行寄存器的设置
void power_off()
{
  				CE=0;
				//仅把高4位清零，意义不明确
				SPI_RW_Reg(WRITE_REG + CONFIG, 0x0D); 
				CE=1;
				_delay_us(20);
}
void ifnnrf_rx_mode(void)
{
//重新给模块上电并把寄存器全部清零
    power_off();
	//芯片的模式控制线
	CE=0;
	//使用和TX发送相同的地址发送数据
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device
	//使能auto ack
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	//选择发射通道
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
	//使用和TX相同的发送数据宽度
  	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
	//选择地噪放大器增益，发射功率，无线速率
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
	//切换工作模式
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..

  	CE = 1; // Set CE pin high to enable RX device

  //  This device is now ready to receive one packet of 16 bytes payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.

}
/**************************************************/

/**************************************************
Function: TX_Mode();

Description:
  This function initializes one nRF24L01 device to
  TX mode, set TX address, set RX address for auto.ack,
  fill TX payload, select RF channel, datarate & TX pwr.
  PWR_UP is set, CRC(2 bytes) is enabled, & PRIM:TX.

  ToDo: One high pulse(>10us) on CE will now send this
  packet and expext an acknowledgment from the RX device.
/**************************************************/
//切换成发送模式
void ifnnrf_tx_mode(void)
{
//切换成关机状态
    power_off();
	CE=0;
	//写入TX节点的地址	
  	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
	//写入RX节点的地址
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	//向Tx_PLOAD中写入数据
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // Writes data to TX payload
	//使能AUTO ACK 
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	//使能PIPE0
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	//配置自动重发次数
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	//选择通信通道
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
	//配置发射参数 (低噪放大器增益，发射功率，无线速率）
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
	//配置成发射模式
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
	CE=1;

}

void SPI_CLR_Reg(BYTE R_T)
{
  	CSN = 0; 
	if(R_T==1)                  // CSN low, init SPI transaction
  	SPI_RW(FLUSH_TX);             // ..and write value to it..
	else
	SPI_RW(FLUSH_RX);             // ..and write value to it..
  	CSN = 1;                   // CSN high again
}
//清除寄存器中的数值
void ifnnrf_CLERN_ALL()
{
  SPI_CLR_Reg(0);
  SPI_CLR_Reg(1);
  //写清空STATUS寄存器
  SPI_RW_Reg(WRITE_REG+STATUS,0xff);
  IRQ=1;
}

