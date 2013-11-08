#define F_CPU 4E6

#include <avr/io.h>
#include <util/delay.h>

extern Ds_tmp_result (void);
extern double tmp;

int main (void)
{
    DDRC = 0XFF;
    PORTC = 0XFF;
    while (1)
    {
	Ds_tmp_result ();
	if (tmp != 0)
	    PORTC = 0X00;
	else
	    PORTC = 0XFF;
	_delay_ms (1000);
    }
    return 0;
}




