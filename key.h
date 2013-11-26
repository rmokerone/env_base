#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define KEY PD3


void Init_key (void)
{	
    DDRD |= (1<<KEY);
    PORTD |= (1<<KEY);
    EICRA |= (1<<ISC11)|(0<<ISC10);
    EIMSK |= (1<<INT1);
}

