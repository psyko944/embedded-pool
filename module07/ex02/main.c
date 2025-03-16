#include "atmega328p_common.h"
#include <avr/eeprom.h>
#include <stdio.h>
#define MAGIC_NUMBER 0x7f

uint16_t find_key(const char* key)
{
        uint16_t addr = 0;
        while (addr + 65 <= (E2END + 1)) {
                uint8_t magic = eeprom_read_byte((uint8_t*)addr);
                if (magic == MAGIC_NUMBER)
				{
                        char stored_key[32];
                        eeprom_read_block(stored_key, (void*)(addr + 1), 32);
                        if (ft_strcmp(stored_key, key) == 0) return addr;
                }
                addr += 65;
        }
        return 0xFFFF;
}

void uint16_to_hex_string(uint16_t value, char *str) {     // this function has it created by Gemini
    str[0] = '0';
    str[1] = 'x';
    str[6] = '\r';
    str[7] = '\n';
    str[8] = '\0'; // Null-terminate the string

    for (int i = 5; i >= 2; --i) {
        uint8_t nibble = value & 0xF; // Get the lowest 4 bits (nibble)
        if (nibble < 10) {
            str[i] = nibble + '0'; // Convert to '0' - '9'
        } else {
            str[i] = nibble - 10 + 'A'; // Convert to 'A' - 'F'
        }
        value >>= 4; // Shift right by 4 bits
    }
}


void read_cmd(const char* key)
{
		uint16_t addr = find_key(key);
        if (addr == 0xFFFF) {
                uart_printstr("empty\r\n");
                return;
        }
        char value[32];
        eeprom_read_block(value, (void*)(addr + 1 + 32), 32);
		uart_printstr(value);
        uart_printstr("\r\n");
}


void	write_cmd(const char *key, const char *value)
{
	if (find_key(key) != 0xFFFF)
	{
		uart_printstr("already exists\r\n");
		return;
	}
	uint16_t addr = 0;
	while (addr + 65 <= E2END)
	{
		uint8_t magic = eeprom_read_byte((uint8_t*)addr);
		if (magic != MAGIC_NUMBER)
		{
			eeprom_write_byte((uint8_t*)addr, MAGIC_NUMBER);
            eeprom_write_block((void*)key,(void*)(addr + 1), 32);
            eeprom_write_block((void*)value,(void*)(addr + 1 + 32), 32);
			char addr_str[16];
			uint16_to_hex_string(addr, addr_str);
			uart_printstr(addr_str);
			return;
		}
		addr += 65;
	}
	uart_printstr("no space left\r\n");
}

void	forget_cmd(const char *key)
{
	uint16_t	addr = find_key(key);
	if (addr == 0xFFFF)
	{
		uart_printstr("not found\r\n");
		return;
	}
	eeprom_write_byte((uint8_t*)addr, 0);
}

void	eeprom_printable(uint16_t addr)
{
	uart_printstr(" | ");
	for (uint16_t i = addr; i < addr + 16;i++)
	{
		uint8_t value = eeprom_read_byte((uint8_t*)i);
		if (is_printable(value))
			uart_tx(value);
		else
			uart_tx('.');
	}
	uart_printstr("|\r\n");
}

void	print_hexdump()
{
	char	byte_str[3];
	for (uint16_t i =0;i < E2END+1;i+=16)
	{
		print_address(i);
		uart_tx(' ');
		for (uint8_t j=0;j < 16;j+=2)
		{
			uint16_t	current_addr = i + j;
			uint8_t first = eeprom_read_byte((uint8_t*)current_addr);
			byte_to_hex(first, byte_str);
			uart_printstr(byte_str);
			uint8_t second = eeprom_read_byte((uint8_t*)(current_addr+1));
			byte_to_hex(second, byte_str);
			uart_printstr(byte_str);
			uart_tx(' ');
		}
		eeprom_printable(i);
		// uart_printstr("\r\n");
	}
	uart_printstr("\r\n");
}	

int main()
{
	init_uart();
	while (1)
	{
		char	input[72];
		print_color("> ", YELLOW);
		read_input(input, 1);
		char cmd[7];
		int index = get_cmd(input, cmd);
		uart_printstr("\r\n");
		if (!is_valid_key(cmd))
		{
			print_error("CMD invalid\r\n");
			continue ;
		}
		if (!ft_strcmp(cmd, "READ"))
		{
			char *token = get_token(input, &index);
			if (!token || !token[0])
				print_error("Error: empty key\r\n");
			else
				read_cmd(token);
		}
		else if (!ft_strcmp(cmd, "WRITE"))
		{
			char *key = get_token(input, &index);
			index++;
			char *value = get_token(input, &index);
			if (key && value)
				write_cmd(key, value);
			else
				print_error("Error: key or value not available");
		}
		else if (!ft_strcmp(cmd, "FORGET"))
		{
			char *key = get_token(input, &index);
			if (!key || !key[0])
				print_error("Error: empty key\r\n");
			else
				forget_cmd(key);
		}
		else if (!ft_strcmp(cmd, "PRINT"))
		{
			print_hexdump();		
		}
	}
	return (0);
}