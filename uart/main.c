#define F_CPU 4E6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

unsigned char table[] = "hello world\n";

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
    DDRC = 0xff;
    PORTC = 0X00;
    init_uart (BAUD_SETTING);
    sei ();
    stdout = &mystdout;
    while(1)
    {
	if (uart_data != '\0')
	{
 	    PORTC = 0x00;
	    printf ("%c", uart_data);
	    uart_data = '\0';
	}
	else 
	    PORTC = 0xff;
    }
    return 0;
}
