#include "atmega328p_common.h"


ISR(INT0_vect)
{
	pushed = 1;
}

int main()
{
	DDRB = (1 << PB0);
	DDRD &= ~(1 << PD2) | (1 << PD4);
	PORTD |= (1 << PD2) | (1 << PD4);
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);
	EIMSK |= (1 << INT0);
	sei();
	while (1)
	{
		if (pushed)
		{
			PORTB ^= D1;
			_delay_ms(150);
			pushed = 0;
		}
	}	
	return 0;  // return 0 if the program is successfull
}
