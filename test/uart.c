#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BAUD 9600
#define CRYSTAL 4000000

#define BAUD_SETTING (unsigned int)((unsigned long)CRYSTAL/(16*(unsigned long)BAUD)-1)

void init_uart(unsigned int baud);
static int uart_putchar (char c, FILE * stream);
unsigned char table[] = "hello world\n";
volatile unsigned char data;

static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);
int main(void)
{
    DDRC = 0xff;
    PORTC = 0X00;
    init_uart (BAUD_SETTING);
    sei ();
    stdout = &mystdout;;
    while(1)
    {
	if (data == 'a')
	{
	    PORTC = 0X00;
 	    printf ("%s",table);
	    data = '\0';
	}
    }
    return 0;
}

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

static int uart_putchar (char c, FILE * stream)
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
	data = UDR;
	PORTC = 0XFF;
    }
    else
        init_uart (BAUD_SETTING);
}

