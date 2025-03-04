#include <avr/io.h>
#include <util/delay.h>
#define MAX 0x17  // 0x17 = 0001 0111
#define MIN 0x00  // 0x00 = 0000 0000

int main()
{
	DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // set register DDRB in OUTPUT
	DDRD &= ~(1 << PD2) | (1 << PD4);  // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2. USE | for choose SWITCH 1 on PD2 and SWITCH 2 on PD4
	PORTD |= (1 << PD2) | (1 << PD4);  // set register PORTD in HIGH.Use | for set actual value at bit 2 and let the other bits unchanged
	PORTB = MIN; // set PORTB in 0x00 which mean 0 of 4 leds are on
	while (1)
	{
		if (!(PIND & (1 << PD2)) && PORTB < MAX) // if SWITCH 1 is pressed, increase while PORTB less than 10111 which represent 4 for leds light up
		{ 
			PORTB += 1; 
			if (PORTB == 0x08)  // if we are at pb3, skip to pb4. pb3 begin at 0x08 = 0000 1000, we move at 0x10 = 0001 0000
				PORTB = 0x10; // skip to the next led
			while (!(PIND & (1 << PD2))) // delay for avoid bouncing
				_delay_ms(50);
		}
		if (!(PIND & (1 << PD4)) && PORTB > MIN) // if SWITCH 2 is pressed, decrease while PORTB greater than 0
		{
			PORTB -= 1;
			if (PORTB == 0x0F) // if we are at pb3. pb3 begin at 0x08 = 0000 1000, we move at 0x07 = 0000 0111
				PORTB = 0x07; // skip to pb2 instead of pb3
			while (!(PIND & (1 << PD4))) // delay for avoid bouncing
				_delay_ms(50);
		}
	}
	return 0;
}