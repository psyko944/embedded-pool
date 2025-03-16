#include "atmega328p_common.h"
#include <avr/eeprom.h>

const char* base = "0123456789ABCDEF";

void print_address(uint16_t addr) {
    for (uint8_t i = 8; i > 0; --i)
        uart_tx(base[(addr >> (i * 4)) & 0x0F]); // each hexadigit reprensent 4 bits. we keep the the four last bits.
}

void	print_hex_value(char c)
{
	uart_tx(((c >> 4) & 0x0F) > 9 ? ((c >> 4) & 0x0F) - 10 + 'a' : ((c >> 4) & 0x0F) + '0');
	uart_tx((c & 0x0F) > 9 ? (c & 0x0F) - 10 + 'a' : (c & 0x0F) + '0');
}

int main()
{
	init_uart();
	uint8_t	buffer[1024];
	eeprom_read_block(buffer, (const void *)0, 1024);
	for (uint32_t i=0; i < 1024;i+=16)
	{
		print_address(i);
		uart_printstr(" ");
		for (uint32_t j = 0; j < 16; j++)
		{
			print_hex_value(buffer[i + j]);
			uart_tx(' ');
		}
		uart_printstr("\r\n");
	}
	while(1);
}