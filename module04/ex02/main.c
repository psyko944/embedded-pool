#include "atmega328p_common.h"

volatile int count = 0;
volatile uint8_t debounce_time = 0;
ISR(INT0_vect)
{
	if (!(PIND & (1 << PD2)) && debounce_time == 0)
	{
		if (count < 23)
		{
			count++;
			PORTB = count;
		}
		// _delay_ms(50);
		debounce_time = 50;
	}
}

ISR(PCINT2_vect)
{
	if (!(PIND & (1 << PD4)) && debounce_time == 0)
	{
		PORTB |= D1;
		if (count > 0)
		{
			count--;
			PORTB = count;
		}	
		_delay_ms(50);
		debounce_time = 50;
	}
}

ISR(TIMER0_COMPA_vect)
{
	if (debounce_time > 0)
		debounce_time--;
}
int main()
{
	DDRB = D1 | D2 | D3 | D4;
	DDRD &= ~((1 << PD2) | (1 << PD4));
	PORTD |= (1 << PD2) | (1 << PD4);
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);
	EIMSK |= (1 << INT0);
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT20);


// Timer setup
    TCCR0A = (1 << WGM01); // CTC mode
    OCR0A = 156;           // Adjust for desired debounce interval
    TIMSK0 = (1 << OCIE0A); // Enable compare match interrupt
    TCCR0B = (1 << CS02);  // Prescaler 256

	sei();
	while (1)
	{
	}	
	return 0;  // return 0 if the program is successfull
}
