#include "atmega328p_common.h"


volatile uint8_t value = 0;
volatile uint8_t direction = 1;

ISR(TIMER0_COMPA_vect)
{
	value += direction;
	if (value == 255 || value == 0)
		direction = -direction;
	// else if (value == 0)
		// direction = 1;
	OCR1A = value;
}

void	init_timer_ctc()
{
	TCCR0A = (1 << WGM01);
	// TCCR0B = (1 << CS00) | (1 << CS01);
	TCCR0B = (1 << CS02);	// prescaler 256
	OCR0A = 124; // FCPU / PRESCALER / 1000 / 2
	TIMSK0 = (1 << OCIE0A);
}

void	init_itmer_pwm()
{
	TCCR1A = (1 << COM1A1) | (1 << WGM10);
	TCCR1B = (1 << CS10);
}


int main()
{
	init_timer_ctc();
	init_itmer_pwm();
	DDRB |= D2;
	sei();
	while (1)
	{
	}	
	return 0;  // return 0 if the program is successfull
}