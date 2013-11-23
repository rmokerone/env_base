//nrf24l01主程序

#define F_CPU 8E6
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "nRF24L01.h"

#define RX  1
#define TX  0

uchar rx_buf[TX_PLOAD_WIDTH];
uchar tx_buf[TX_PLOAD_WIDTH];//这个应该是发送数据的缓冲区
uchar flag;

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

//封装好的发送函数
void nrf_tx (void);
//封装好的接收函数
void nrf_rx (void);

int main (void)
{
	flag = TX;	
	unsigned char i = 0;
	init_uart (BAUD_SETTING);
	stdout = &mystdout;
	NRF24L01_SPI_Init ();
	for (i = 0; i < 32; i++)
	{
		tx_buf[i] = 0xff;
	}
	while (1)
	{	
		if (flag)
		{
			nrf_rx ();
			printf ("the data are: \n");
			for (i = 0; i < 32; i++)
			{
				printf ("%4d", rx_buf[i]);
				rx_buf[i] = '\0';
				if ((i+1) % 10 == 0)
				printf ("\n");
			}
			printf ("\n");
		}
		else
		{
			nrf_tx ();
		}
		_delay_ms (1000);
	}
}

//封装好的接收函数
void nrf_rx (void)
{
	uchar rx_sta;
	ifnnrf_rx_mode ();
	_delay_us (200);
	rx_sta = SPI_Read (STATUS);
	SPI_RW_Reg (WRITE_REG+STATUS,0XFF);	
	if (rx_sta&STA_MARK_RX)
	{
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
	}
	printf ("the status reg is 0x%x\n",rx_sta);
}

//封装好的发送函数
void nrf_tx (void)
{
	uchar tx_sta;
	ifnnrf_tx_mode ();
	_delay_ms (10);
	tx_sta = SPI_Read (FIFO_STATUS);
	ifnnrf_CLERN_ALL ();
	printf ("the status reg is 0x%x\n",tx_sta);
}	

	


