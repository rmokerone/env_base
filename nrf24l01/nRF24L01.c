#include "nRF24L01.h"

#define uchar unsigned char

uchar TX_ADDRESS[TX_ADR_WIDTH] = {0X34,0X43,0X10,0X10,0X01};

void init_nrf24l01_io(void)
{	
	CLEARBIT(PORTB,CE);
   	SETBIT(PORTB,CSN);
	CLEARBIT(PORTB,SCK);
}

void SPI_Init(void)     // Init SPI 
{
	DDRB |= (1<<SCK) | (1<<MOSI) | (1<<CSN) | (1<<CE);
	SPCR |= (1<<SPE) | (1<<MSTR);
}

uchar SPI_RW(BYTE byte)         // Single SPI read/write
{
   	SPDR = byte;
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

/**************************************************
Function: SPI_RW_Reg();

Description:
  Writes value 'value' to register 'reg'
**************************************************/
uchar SPI_RW_Reg(BYTE reg, BYTE value)   // Read one byte from nRF24L01
{
	uchar status;
  	CSN_CLR;                   // CSN low, init SPI transaction
  	status = SPI_RW(reg);      // select register
  	SPI_RW(value);             // ..and write value to it..
  	CSN_EN;                   // CSN high again
        return(status);            // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'
**************************************************/
BYTE SPI_Read(BYTE reg)
{
	BYTE reg_val;

  	CSN_CLR;                // CSN low, initialize SPI communication...
  	SPI_RW(reg);            // Select register to read from..
  	reg_val = SPI_RW(0);    // ..then read registervalue
  	CSN_EN;                // CSN high, terminate SPI communication

  	return(reg_val);        // return register value
}
/**************************************************/

/**************************************************
Function: SPI_Read_Buf();

Description:
  Reads 'bytes' #of bytes from register 'reg'
  Typically used to read RX payload, Rx/Tx address
**************************************************/
uchar SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	uchar status,byte_ctr;

  	CSN_CLR;                    		// Set CSN low, init SPI tranaction
  	status = SPI_RW(reg);       		// Select register to write to and read status byte

  	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0);    // Perform SPI_RW to read byte from nRF24L01

  	CSN_EN;                           // Set CSN high again

  	return(status);                    // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Write_Buf();

Description:
  Writes contents of buffer '*pBuf' to nRF24L01
  Typically used to write TX payload, Rx/Tx address
**************************************************/
uchar SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
	uchar status,byte_ctr;

  	CSN_CLR;                   // Set CSN low, init SPI tranaction
  	status = SPI_RW(reg);    // Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	SPI_RW(*pBuf++);
  	CSN_EN;                 // Set CSN high again
  	return(status);          // return nRF24L01 status byte
}
/**************************************************/
void power_off()
{
	CE_CLR;	//仅把高4位清零，意义不明确
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0D); 
	CE_EN;
	_delay_us(20);
}

/**************************************************
Function: TX_Mode();

Description:
  This function initializes one nRF24L01 device to
  TX mode, set TX address, set RX address for auto.ack,
  fill TX payload, select RF channel, datarate & TX pwr.
  PWR_UP is set, CRC(2 bytes) is enabled, & PRIM:TX.

  ToDo: One high pulse(>10us) on CE will now send this
  packet and expext an acknowledgment from the RX device.
**************************************************/
//切换成发送模式
void ifnnrf_tx_mode(void)
{
//切换成关机状态
    power_off();
	CE_CLR;
	//写入TX节点的地址	
  	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
	//写入RX节点的地址
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	//向Tx_PLOAD中写入数据
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // Writes data to TX payload
	//使能AUTO ACK (从0x01修改到0x00）
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	//使能PIPE0 (从0x01修改到0x00）
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	//配置自动重发次数 (从0x1a修改到0x00)
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x01); // 500us + 86us, 10 retrans...
	//选择通信通道
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);        // Select RF channel 40
	//配置发射参数 (低噪放大器增益，发射功率，无线速率）
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
	//配置成发射模式
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
	CE_EN;
}

void ifnnrf_rx_mode(void)
{
//重新给模块上电并把寄存器全部清零
    power_off();
	//芯片的模式控制线
	CE_CLR;
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

	CE_EN; // Set CE pin high to enable RX device

  //  This device is now ready to receive one packet of 16 bytes payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.

}



void SPI_CLR_Reg(BYTE R_T)
{
        CSN_CLR;
        if(R_T==1)                  // CSN low, init SPI transaction
        SPI_RW(FLUSH_TX);             // ..and write value to it..
        else
        SPI_RW(FLUSH_RX);             // ..and write value to it..
        CSN_EN;                   // CSN high again
}
//清除寄存器中的数值
void ifnnrf_CLERN_ALL()
{
  SPI_CLR_Reg(0);
  SPI_CLR_Reg(1);
  //写清空STATUS寄存器
  SPI_RW_Reg(WRITE_REG+STATUS,0xff);
}

//封装好的接收函数,成功返回1，失败返回0
uchar nrf_rx (void)
{
	uchar rx_sta,bit;
	ifnnrf_rx_mode ();
	_delay_us (400);
	rx_sta = SPI_Read (STATUS);
	SPI_RW_Reg (WRITE_REG+STATUS,0XFF);	
	if (rx_sta&STA_MARK_RX)
	{
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
		bit = 1;
	}
	else
	{
		bit = 0;
	}
	ifnnrf_CLERN_ALL();
	return bit;
//	printf ("the status reg is 0x%x\n",rx_sta);
}

//封装好的发送函数，成功返回1，失败返回0
uchar nrf_tx (void)
{
	uchar tx_sta,bit;
	ifnnrf_tx_mode ();
	while (PIND & (1<<IRQ));
	tx_sta = SPI_Read (STATUS);
	SPI_RW_Reg (WRITE_REG+STATUS,0Xff);
	if (tx_sta&STA_MARK_TX)
	{
		ifnnrf_CLERN_ALL ();
		bit = 1;
	}
	else 
	{
		ifnnrf_CLERN_ALL ();
		bit = 0;
	}
	return bit;
//	printf ("the status reg is 0x%x\n",tx_sta);
}
