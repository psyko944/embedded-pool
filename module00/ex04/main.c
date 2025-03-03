#include <avr/io.h>
#include <util/delay.h>
#define MAX 0x17  // 0x17 = 0001 0111
#define MIN 0x00  // 0x00 = 0000 0000

int main()
{
	DDRB = 0x01; // set register DDRB in OUTPUT
	DDRD &= ~(1 << PD2) | (1 << PD4);  // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2. USE | for choose SWITCH 1 on PD2 and SWITCH 2 on PD4
	PORTD |= (1 << PD2) | (1 << PD4);  // set register PORTD in HIGH.Use | for set actual value at bit 2 and let the other bits unchanged
	PORTB = MIN; // set PORTB in 0x00 which mean 0 of 4 leds are on
	while (1)
	{
		if (!(PIND & (1 << PD2))) // if SWITCH 1 is pressed
		{
			if (PORTB < MAX) // if PORTB is different than 10111. 
				PORTB += 1; 
				if (PORTB == 0x08)  // if we are at pb3, skip to pb4. pb3 begin at 0x08 = 0000 1000, we move at 0x10 = 0001 0000
				PORTB += 8; // skip to the next led
			while (!(PIND & (1 << PD2))) // delay for avoid bouncing
				_delay_ms(50);
		}
		if (!(PIND & (1 << PD4))) // if SWITCH 2 is pressed
		{
			if (PORTB > MIN) // we can decremante until PORTB is different than 0
				PORTB -= 1;
			if (PORTB == 0x1F) // if we are at pb3. pb3 begin at 0x08 = 0000 1000, we move at 0x07 = 0000 0111
				PORTB -= 8; // skip to pb2 instead of pb3
			while (!(PIND & (1 << PD4))) // delay for avoid bouncing
				_delay_ms(50);
		}
	}
	return 0;
}