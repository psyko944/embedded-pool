#include "atmega328p_common.h"


int main()
{
	while (1)
	{
		DDRB = (1 << PB0);
		DDRD &= ~(1 << PD2);
		PORTD |= (1 << PD2);
		EICRA |= (1 << ISC01);
		EICRA &= ~(1 << ISC00);
		EIMSK |= (1 << INT0);
		sei();
		ISR(INT0_vect)
		{
			PORTB ^= D1;
		}
	}	
	return 0;  // return 0 if the program is successfull
}