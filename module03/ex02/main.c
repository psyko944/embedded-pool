#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define R (1 << PD5)
#define G (1 << PD6)
#define B (1 << PD3)
#define Y R | G		//yellow
#define C B | G		//cyan
#define M R | B		//magenta
#define W R | G | B		//white
#define PRESCALER 8

void	init_leds()
{
	DDRD = R | G | B; // init 
}

void	init_rgb()
{
	init_leds();
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);	//set register for fast pwm
	TCCR0B |= (1 << CS01);	// set for prescaler 8. we choose the smaller prescaler possible

	// Timer2 for blue LED
	TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS21); 
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = g;	// register for green
	OCR0B = r;	// register for red
	OCR2B = b;	// register for blue		.see the devboard 
}

const uint8_t led_tab[] = {R,G,B,Y,C,M,W}; 	// tab with all colors

void wheel(uint8_t pos) {		// function for gradiant color
	pos = 255 - pos;
	if (pos < 85)
		set_rgb(255 - pos * 3, 0, pos * 3);
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else 
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}


int main()
{
	uint8_t pos = 0;
	init_rgb();
	while (1)
	{
		wheel(pos);							// every 25 ms change shades of color
		pos = (pos + 1) % 255;
		_delay_ms(25);
	}	
	return 0;  // return 0 if the program is successfull
}