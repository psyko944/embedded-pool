#include "atmega328p_common.h"


void spi_master_init()
{
	DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
	// DDRB &= ~(1 << PB4);
	SPCR  = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  //enable spi, set master spi, and determine spi clock

}

void	spi_transmit(uint8_t data)
{
	SPDR = data;    
	while (!(SPSR & (1 << SPIF)));
}

int main()
{
	spi_master_init();
	for (uint8_t i =0;i< 4;i++) spi_transmit(0x00);

	// D6
	spi_transmit(0xFF); // global bightness
	spi_transmit(0x00); // blue
	spi_transmit(0x00); // green
	spi_transmit(0xFF); // red

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

	for (uint8_t i =0;i< 4;i++) spi_transmit(0xFF);
	while(1);

}