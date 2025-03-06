#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define R PD5
#define G PD6
#define B PD3

const uint8_t led_tab[] = {R, G, B};
void	init_leds()
{
	DDRD = (1 << R) | (1 << G) | (1 << B);
}

int main()
{
	uint8_t state = 0;
	init_leds();
	while (1)
	{
		PORTD = 0;
		PORTD |= (1 << led_tab[state]);
		_delay_ms(1000);
		state = (state + 1) % 3;  
	}	
	return 0;  // return 0 if the program is successfull
}