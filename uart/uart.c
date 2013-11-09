#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

void init_uart(unsigned int baud)
{
    DDRD |= 0x02;
    PORTD |= 0X03;
    UBRRH = (unsigned char)(baud >> 8);
    UBRRL = (unsigned char) baud;
    UCSRA = 0X00;
    UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
    UCSRC = (1 << URSEL)|(1 << UCSZ0)|(1<<UCSZ1);
}

int uart_putchar (char c, FILE * stream)
{
    if (c == '\n')
	uart_putchar ('\r', stream);
    loop_until_bit_is_set (UCSRA, UDRE);
    UDR = c;
    return 0;
}

ISR (USART_RXC_vect)
{
    unsigned char status;
    status = UCSRA;
    if ((status&((1<<DOR)|(1<<FE)|(1<<PE)))==0)
    {
	uart_data = UDR;
	PORTC = 0XFF;
    }
    else
        init_uart (BAUD_SETTING);
}

