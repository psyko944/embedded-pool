#include "atmega328p_common.h"

uint8_t is_valid_key(const char *str)
{
	if (!ft_strcmp(str, "READ") || !ft_strcmp(str, "WRITE") || !ft_strcmp(str, "FORGET") || !ft_strcmp(str, "PRINT"))
		return (1);
	return (0);
}

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