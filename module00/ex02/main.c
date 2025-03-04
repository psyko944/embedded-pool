#include <avr/io.h>

int main()
{
	DDRB = (1 << PB0);; // set register DDRB in OUTPUT
	DDRD &= ~(1 << PD2); // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2
	PORTD |= (1 << PD2); // set register PORTD in HIGH. PD2 is SWITCH 1. Use | for set actual value at bit 2 and let the other bits unchanged
	while (1)
	{
		if (!(PIND & (1 << PD2))) // if SWITCH 1 is pressed
			PORTB |=  (1 << PB0); // lights up the led on PB0
		else
			PORTB &= ~(1 << PB0); // lights off the led on PB0
	}
	return 0; // return 0 if the program is successful
}