#define F_CPU 8E6

#include <avr/io.h>
#include <util/delay.h>
#include "ds18b20.h"

int main (void)
{
    DDRB = 0XFF;
    PORTB = 0XFF;
    while (1)
    {
	Ds_tmp_result ();
	if (tmp > 0)
	{
	    tmp = 0;
	    PORTB = 0X00;
	    _delay_ms (1000);
	}
	else
	    PORTB = 0XFF;
	_delay_ms (1000);
    }
    return 0;
}




