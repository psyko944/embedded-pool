#include <avr/io.h>

int main()
{
	DDRB = (1 << PB0); // set register DDRB in OUTPUT. 1 for OUTPUT and 0 for INPUT
	PORTB |= (1 << PB0); // set PORTB in HIGH for the first led. 1 for HIGH and 0 for LOW
	return 0;  // return 0 if the program is successful
}