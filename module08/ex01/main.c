#include "atmega328p_common.h"

#define RED {0xFF, 0x00, 0x00, 0xFF}
#define GREEN {0xFF, 0x00, 0xFF, 0x00}
#define BLUE {0xFF, 0xFF, 0x00, 0x00}
#define YELLOW {0xFF, 0x00, 0xFF, 0xFF}
#define CYAN {0xFF, 0xFF, 0xFF, 0x00}
#define MAGENTA {0xFF, 0xFF, 0x00, 0xFF}
#define WHITE {0xFF, 0xFF, 0xFF, 0xFF}

const uint8_t led_tabs[][4] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};
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


void	switch_led(uint8_t state)
{
	
	for (uint8_t i = 0;i < 4;i++) spi_transmit(led_tabs[state][i]);
	// D7
	spi_transmit(0xE0);   //   11100000
	spi_transmit(0x00);
	spi_transmit(0x00);
	spi_transmit(0x00);
	
	//D8
	spi_transmit(0xE0);
	spi_transmit(0x00);
	spi_transmit(0x00);
	spi_transmit(0x00);
}

int main()
{
	uint8_t state = 0;
	spi_master_init();
	while(1)
	{
		for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);
		switch_led(state);
		for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
		state = (state + 1) % 7;
		_delay_ms(1000);
	}

}