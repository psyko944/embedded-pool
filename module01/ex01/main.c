#include <avr/io.h>
#include <util/delay.h>
#ifndef FCPU
	#define FCPU 16000000UL
#endif
#define PRESCALER 1024UL
void	init_timer()
{
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
	OCR1A = FCPU / PRESCALER / 2UL;
}

int main()
{
	DDRB = (1 << PB1); // set register DDRB in OUTPUT. 1 for OUTPUT and 0 for INPUT
	init_timer();
	while (1)
	{
	}
	return 0;  // return 0 if the program is successful
}