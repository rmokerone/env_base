#define F_CPU 8E6 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

unsigned char table[] = "hello world\n";

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
    DDRB = 0xff;
    PORTB = 0X00;
    init_uart (BAUD_SETTING);
    stdout = &mystdout;
    while(1)
    {
	    printf ("%s", table);
	   _delay_ms (1000);
    }
    return 0;
}
