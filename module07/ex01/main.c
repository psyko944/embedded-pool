#include "atmega328p_common.h"
#include <avr/eeprom.h>
#include <stdio.h>
const char* hex_chars = "0123456789ABCDEF";


void	print_color(const char *str, const char *color)
{
	uart_printstr(color);
	uart_printstr(str);
	uart_printstr(RESET);
}

void print_error(const char *str)
{
	uart_printstr(RED);
	uart_printstr(str);
	uart_printstr(RESET);
}

void print_address(uint16_t addr) {
	const char base[17] = "0123456789abcdef";
	for (uint8_t i = 8; i > 0; i--) 
		uart_tx(base[(addr >> ((i - 1) * 4)) & 0x0F]);
}

void	print_hex_value(char c)
{
	uart_tx(((c >> 4) & 0x0F) > 9 ? ((c >> 4) & 0x0F) - 10 + 'a' : ((c >> 4) & 0x0F) + '0');
	uart_tx((c & 0x0F) > 9 ? (c & 0x0F) - 10 + 'a' : (c & 0x0F) + '0');
}

void byte_to_hex(uint8_t byte, char* hex_str) {
    uint8_t first = (byte >> 4) & 0x0F;
    uint8_t second = byte & 0x0F;

    hex_str[0] = (first < 10) ? (first + '0') : (first - 10 + 'a');
    hex_str[1] = (second < 10) ? (second + '0') : (second - 10 + 'a');
    hex_str[2] = '\0';
}

void	print_hexdump(uint16_t addr)
{
	char	byte_str[3];
	for (uint16_t i =0;i < E2END;i+=16)
	{
		print_address(i);
		uart_tx(' ');
		for (uint8_t j=0;j < 16;j++)
		{
			uint16_t	current_addr = i + j;
			if (current_addr < E2END)
			{
				uint8_t first = eeprom_read_byte((uint8_t*)current_addr);
				byte_to_hex(first, byte_str);
				if (current_addr == addr)
					print_error(byte_str);
				else
					uart_printstr(byte_str);
				uint8_t second = eeprom_read_byte((uint8_t*)current_addr+1);
				byte_to_hex(second, byte_str);
				if (current_addr + 1 == addr)
					print_error(byte_str);
				else
					uart_printstr(byte_str);
				uart_tx(' ');
			}
		}
		uart_printstr("\r\n");
	}
	uart_printstr("\r\n");
}	

int main()
{
	init_uart();
	while (1)
	{
		char	input_buffer[16];
		print_color("input: ", BLUE);
		read_input(input_buffer, 1);
		uart_printstr("\r\n");
		char *sep_string = ft_strchr(input_buffer, ' ');
		if (!sep_string)
		{
			print_error("Error: too few arguments\r\n");
			continue;
		}
		*sep_string = 0;
		int16_t addr = parse_values(input_buffer);
		int16_t hex_value = parse_values(sep_string+1);
		uart_printstr(input_buffer);
		uart_printstr(sep_string+1);
		uart_printstr("\r\n");
		if (addr > E2END || hex_value > 0xFF)
		{
			print_error("value out of bounds\r\n");
			continue;
		}
		if (hex_value != eeprom_read_byte((uint8_t*)addr))
		{
			eeprom_write_byte((uint8_t*)addr, (uint8_t)hex_value);
		}
		else
			uart_printstr("values unchanged\r\n");
		print_hexdump(addr);
	}
	return (0);
}