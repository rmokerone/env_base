//nrf24l01主程序

#define F_CPU 8E6
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "nRF24L01.h"
static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main (void)
{
	flag = RX;	
	unsigned char i = 0,bit;
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
			bit = nrf_rx ();
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
			nrf_tx ();
		}
		_delay_ms (1000);
	}
}

