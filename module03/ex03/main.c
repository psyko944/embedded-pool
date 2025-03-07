#include "atmega328p_common.h"

const uint8_t led_tab[] = {R,G,B,Y,C,M,W};

void	init_leds() // init register for led rgb
{
	DDRD = R | G | B;
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



void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = g;	// green led in register OCR0A
	OCR0B = r;	// red led in register OCR0B
	OCR2B = b;	// blue led in register OCR2b
}

void	hex_to_rgb(const char *hex_color_code) // break hexa code in 3 str for r, g, b then convert
{
	hex_color_code += 1;
	char red[3], green[3], blue[3];
	ft_strlcpy(red, hex_color_code, 3);
	ft_strlcpy(green, hex_color_code + 2, 3);
	ft_strlcpy(blue, hex_color_code + 4, 3);
	uart_printstr(RED);
	uart_printstr("red: ");
	uart_print_number(ft_atoi(red));
	uart_printstr("\t");
	uart_printstr(GREEN);
	uart_printstr("green: ");
	uart_print_number(ft_atoi(green));
	uart_printstr("\t");
	uart_printstr(BLUE);
	uart_printstr("blue: ");
	uart_print_number(ft_atoi(blue));
	uart_printstr(RESET);
	uart_printstr("\r\n");
	set_rgb(ft_atoi(red), ft_atoi(green), ft_atoi(blue));
}

int main()
{
	uint8_t pos = 0;
	init_rgb();
	init_uart();
	char	color_buffer[8];
	while (1)
	{
		uart_printstr("color: ");
		read_input(color_buffer,1);
		if (!is_valid_color(color_buffer))
		{
			uart_printstr("\r\ninvalid color\r\n");
			continue;
		}
		hex_to_rgb(color_buffer);
	}	
	return 0;  // return 0 if the program is successfull
}