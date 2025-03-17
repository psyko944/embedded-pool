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
uint8_t led_D7[4] = {0xFF,0x00,0x00,0x00};
uint8_t led_D8[4] = {0xFF,0x00,0x00,0x00};
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

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t tab[4] = 	{0xff, b, g, r};
	for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
	for (uint8_t i = 0;i < 4;i++) spi_transmit(tab[i]);
	for (uint8_t i = 0;i < 4;i++) spi_transmit(tab[i]);
	for (uint8_t i = 0;i < 4;i++) spi_transmit(tab[i]);
	for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
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

void	switch_led(const char  *input)
{
	uint8_t r = get_color(input);
	uint8_t g = get_color(input + 2);
	uint8_t b = get_color(input + 4);
	uint8_t tab[4] = {0xFF, b, g, r};
	if (!ft_strcmp(input + 6, "D6"))	
	{
		// led_D6 = tab;
		copy_tab(led_D6, tab);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D6[i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D7[i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D8[i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
	}
	else if (!ft_strcmp(input + 6, "D7"))
	{
		// led_D7 = tab;
		copy_tab(led_D7, tab);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D6[i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D7[i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D8[i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(tab[i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
	}
	else if (!ft_strcmp(input + 6, "D8"))
	{
		// led_D8 = tab;
		copy_tab(led_D8, tab);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D6[i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D7[i]);
		for (uint8_t i = 0;i < 4;i++) spi_transmit(led_D8[i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[7][i]);
		// for (uint8_t i = 0;i < 4;i++) spi_transmit(tab[i]);
	}
}

uint8_t is_valid_str(const char *buffer)
{
	uint8_t i = 0;
	while (buffer[i])
	{
		if (!is_xdigit(buffer[i]))
			return (0);
		i++;
	}
	i-=2;
	if (buffer[i] != 'D' || (buffer[i+1] != '6' && buffer[i+1] != '7' && buffer[i+1] != '8'))
		return (0);
	return (1);
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
	uint8_t state = 0;
	init_uart();
	spi_master_init();
	clear_leds();
	while(1)
	{
		uart_printstr("enter a color : #");
		char buffer[12];
		read_input(buffer, 1);
		if (!ft_strcmp(buffer, "FULLRAINBOW"))
		{
			for (uint8_t i = 0;i < 256;i++)
			{
				wheel(i);
				_delay_ms(20);
			}
			clear_leds();
			continue;
		}
		if (ft_strlen(buffer) != 8 || !is_valid_str(buffer))
		{
			print_color("invalid color\r\n",RED_STR);
			continue;
		}
		for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
		switch_led(buffer);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
	}

}