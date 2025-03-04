#include <avr/io.h>
#include <util/delay.h>

void	wait_hz(volatile uint64_t i) 
{
	while (i--);
}

int main()
{
	DDRB = (1 << PB1); // set register DDRB in OUTPUT. 1 for OUTPUT and 0 for INPUT
	PORTB &= ~(1 << PB1); // set PORTB in HIGH for the first led. 1 for HIGH and 0 for LOW
	while (1)
	{
		wait_hz(90000); // loop for wait approximatively 1hz
		PORTB ^= (1 << PB1); // inverse state of led d2
	}
	
	return 0;  // return 0 if the program is successful
}