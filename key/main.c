#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/interrupt.h>
#define KEY PD3

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

void Init_key(void);

int main (void)
{
     init_uart(BAUD_SETTING);
     stdout = &mystdout;
     Init_key();
	while (1);
     return 0;
}

void Init_key (void)
{	
    DDRD |= (1<<KEY);
    PORTD |= (1<<KEY);
    EICRA |= (1<<ISC11)|(0<<ISC10);
    EIMSK |= (1<<INT1);
}

ISR (INT1_vect)
{
     printf ("hello world\n");
}
