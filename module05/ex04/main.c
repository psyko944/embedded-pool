#include "atmega328p_common.h"

void	init_leds() // init register for leds
{
	DDRD = R | G | B;
	DDRB = D1 | D2 | D3 | D4;
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	init_leds();
	OCR0A = g;	// green led in register OCR0A
	OCR0B = r;	// red led in register OCR0B
	OCR2B = b;	// blue led in register OCR2b
	if (r == 0)
		DDRD &= ~(R);
	if (g == 0)
		DDRD &= ~(G);
	if (b == 0)
		DDRD &= ~(B);
}

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

void	handle_led(long value) 
{
	if (value < 255) // below 25 %
		PORTB = D1;
	else if (value < 511) // between 25 % and 50 %
		PORTB = D2;
	else if ( value < 767) // between 50 % and 75 %
		PORTB = D3;
	else // higher than 75%
		PORTB = D4;
	uart_print_number((value * 255) / 1023);
	wheel((value * 255) / 1023);
}


void	init_rgb()
{
	init_leds();
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // init register in fast pwm
	TCCR0B |= (1 << CS01);

	// Timer2 for blue LED
	TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS21); 
}


int main()
{
	init_rgb();
	ADMUX = (1 << REFS0); // use AVCC, set 10 bits
  	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
	  
	init_uart();
	while (1)
	{
		// Read POT
		ADMUX = (ADMUX & 0xF8) | 0; 
		ADCSRA |= (1 << ADSC); // trigger ADC conversion
		while (ADCSRA & (1 << ADSC)); // wait until conversion is complete
		handle_led(ADC);
		uart_printstr("\r\n");
		_delay_ms(20);
	}	
	return 0;  // return 0 if the program is successfull
}