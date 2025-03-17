#include "atmega328p_common.h"

#define RED {0xFF, 0x00, 0x00, 0xFF}
#define GREEN {0xFF, 0x00, 0xFF, 0x00}
#define BLUE {0xFF, 0xFF, 0x00, 0x00}
#define YELLOW {0xFF, 0x00, 0xFF, 0xFF}
#define CYAN {0xFF, 0xFF, 0xFF, 0x00}
#define MAGENTA {0xFF, 0xFF, 0x00, 0xFF}
#define WHITE {0xFF, 0xFF, 0xFF, 0xFF}
#define EMPTY {0xE0, 0x00, 0x00, 0x00}

const uint8_t led_tabs[][4] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, EMPTY};
uint8_t led_D6[4] = {0xFF,0x00,0x00,0x00};
uint8_t tab[4] = {0xE0,0x00,0x00,0x00};
uint8_t led_D7[4] = {0xFF,0x00,0x00,0x00};
uint8_t led_D8[4] = {0xFF,0x00,0x00,0x00};
volatile uint8_t color_state = 0;
volatile uint8_t led_state = 0;
void spi_master_init()
{
	DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
	SPCR  = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  //enable spi, set master spi, and determine spi clock

}

void	spi_transmit(uint8_t data)
{
	SPDR = data;    
	while (!(SPSR & (1 << SPIF)));
}

uint8_t	ft_strlen(const char *buffer) // return length of buffer
{
	uint8_t	i = 0;
	while (buffer[i])
		i++;
	return (i);
}

uint8_t is_digit(char c)
{
	return (c >= '0' && c <='9');
}

uint8_t get_color(const char *input)
{
	uint8_t i = 0;
	uint8_t res = 0;
	while (input[i] && i < 2)
	{
		char c = input[i];
        if (is_digit(c))
            res = res * 16 + (c - '0');
        else if (c >= 'a' && c <= 'f')
            res = res * 16 + (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            res = res * 16 + (c - 'A' + 10);
		i++;
	}
	return res;
}

void	copy_tab(uint8_t tab[4], uint8_t tab2[4])
{
	for (int i = 0; i < 4; i++) {
    	tab[i] = tab2[i];
}
}


void	print_tab(uint8_t tab[4])
{
	for (uint8_t i =0; i< 4;i++)
	{
		uart_print_number(tab[i]);
		uart_printstr("\r\n");
	}
}

void	value_pot(long value)
{
	if (value >= 341 && value < 682)	
		copy_tab(tab, (uint8_t[4]){0xFF, 0x00, 0x00, 0xFF});	
	else if (value >= 682 && value < 1023)
		copy_tab(tab, (uint8_t[4]){0xFF, 0x00, 0xFF, 0x00});
	else if (value == 1023)
		copy_tab(tab, (uint8_t[4]){0xFF, 0xFF, 0x00, 0x00});
	else
		copy_tab(tab, (uint8_t[4]){0xE0, 0x00, 0x00, 0x00});
}

void	switch_led(long value)
{
	if (!(PIND & (1 << PD2)))
	{
		value_pot(value);
		while (!(PIND & (1 << PD2))) // delay for avoid bouncing
			_delay_ms(50);
	}
	if (!(PIND & (1 << PD4)))
	{
		led_state = (led_state + 1) % 3;
		while (!(PIND & (1 << PD4))) // delay for avoid bouncing
			_delay_ms(50);
	}
	uint8_t empty_tab[4] = EMPTY;
	if (led_state == 0)
	{
		print_tab(tab);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
		for (uint8_t i =0;i< 4;i++) spi_transmit(tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(empty_tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(empty_tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
	}	
	else if (led_state == 1)
	{
		for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
		for (uint8_t i =0;i< 4;i++) spi_transmit(empty_tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(empty_tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);

	}
	else if (led_state == 2)
	{
		for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
		for (uint8_t i =0;i< 4;i++) spi_transmit(empty_tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(empty_tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(tab[i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
	}
	// color_state = 0;
}

void	clear_leds()
{
		for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
}

int main()
{
	DDRD &= ~(1 << PD2) | (1 << PD4);  // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2. USE | for choose SWITCH 1 on PD2 and SWITCH 2 on PD4
	PORTD |= (1 << PD2) | (1 << PD4);  // set register PORTD in HIGH.Use | for set actual value at bit 2 and let the other bits unchanged
	uint8_t state = 0;
	ADMUX = (1 << REFS0); // use AVCC, set 10 bits
  	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
	init_uart();
	spi_master_init();
	clear_leds();
	while(1)
	{
		ADMUX = (ADMUX & 0xF8) | 0; 
		ADCSRA |= (1 << ADSC); // trigger ADC conversion
		while (ADCSRA & (1 << ADSC)); // wait until conversion is complete
		switch_led(ADC);
		_delay_ms(20);
	}

}