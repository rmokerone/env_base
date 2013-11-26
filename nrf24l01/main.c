//nrf24l01主程序

#define F_CPU 8E6
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "nRF24L01.h"

uchar TX_ADDRESS[TX_ADR_WIDTH] = {0X34,0X43,0X10,0X10,0X01};

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

unsigned char TX_AP0[TX_ADR_WIDTH] =  {0X34,0X43,0X10,0X10,0X02};

int main (void)
{
	unsigned char AP;	
	unsigned char i = 0,bit;
	flag = RX;
	AP = 0;
//若为第一个节点则AP = 0，第二个节点AP = 1
	if (AP == 1)
	{
		for (i = 0; i< TX_ADR_WIDTH; i++)
		{
			TX_ADDRESS[i] = TX_AP0[i];
		}
	}
	init_uart (BAUD_SETTING);
	stdout = &mystdout;
	SPI_Init ();
	for (i = 0; i < 32; i++)
	{
		tx_buf[i] = 0xff;
	}
	while (1)
	{
		if (flag==RX)
		{
			bit = nrf_rx (TX_ADDRESS);
			if (bit)
			{
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
				printf ("the data error\n");
		}
		else
		{
			nrf_tx (TX_ADDRESS);
		}
		_delay_ms (1000);
	}
}

