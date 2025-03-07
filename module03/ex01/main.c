#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define R (1 << PD5)
#define G (1 << PD6)
#define B (1 << PD3)
#define Y R | G //yellow
#define C B | G //Cyan
#define M R | B //Magenta
#define W R | G | B	// white

const uint8_t led_tab[] = {R, G, B,Y,C,M,W};
void	init_leds()
{
	DDRD = R | G | B; // init register for led rgb
}

int main()
{
	uint8_t state = 0;
	init_leds();
	while (1)
	{
		PORTD = 0;
		PORTD |= led_tab[state];
		_delay_ms(1000);
		state = (state + 1) % 7;  // switch 7 colors
	}	
	return 0;  // return 0 if the program is successfull
}