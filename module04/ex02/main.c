#include "atmega328p_common.h"

volatile int count = 0;
ISR(INT0_vect)
{
	if (!PIND & (1 << PD2))
	{
		PORTB = count;
		if (PORTB == 0x08)  // if we are at pb3, skip to pb4. pb3 begin at 0x08 = 0000 1000, we move at 0x10 = 0001 0000
				PORTB = 0x10; // skip to the next led
		count = (count + 1) % 23;
		while (!(PIND & (1 << PD2))) // delay for avoid bouncing
				_delay_ms(50);
	}

}

ISR(PCINT2_vect)
{
	if (!PIND & (1 << PD4))
		count--;
}

int main()
{
	DDRB = (1 << PB0);
	DDRD &= ~(1 << PD2) | (1 << PD4);
	PORTD |= (1 << PD2) | (1 << PD4);
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);
	EIMSK |= (1 << INT0);
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT20);
	sei();
	// SREG & (1 << 7)
	while (1)
	{
	}	
	return 0;  // return 0 if the program is successfull
}
