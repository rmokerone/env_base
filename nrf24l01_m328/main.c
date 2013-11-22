//nrf24l01主程序

#define F_CPU 8E6
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "nRF24L01.h"

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main (void)
{	
	unsigned char i = 0;
	unsigned char sta;
	init_uart (BAUD_SETTING);
	stdout = &mystdout;
	for (i = 0; i < 32; i++)
	{
	   	tx_buf [i] = i;
	}	
	NRF24L01_SPI_Init ();
//	init_nrf24l01_io ();
//	ifnnrf_tx_mode ();
//	_delay_ms (10);
//	sta = SPI_Read (FIFO_STATUS);
	while (1)
	{
		ifnnrf_tx_mode ();
		_delay_ms (10);
		sta = SPI_Read (STATUS);
		ifnnrf_CLERN_ALL ();
		printf ("the status reg = 0x%x\n",sta);
		_delay_ms (1000);
	}
}
