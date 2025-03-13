#include "atmega328p_common.h"

#define SLAVE_ADDRESS 0x38

void	print_hex_value(char c)
{
	uart_tx(((c >> 4) & 0x0F) > 9 ? ((c >> 4) & 0x0F) - 10 + 'A' : ((c >> 4) & 0x0F) + '0');
    uart_tx((c & 0x0F) > 9 ? (c & 0x0F) - 10 + 'A' : (c & 0x0F) + '0');
}

int main()
{
	uint8_t status;
	init_uart();
	_delay_ms(20);
	i2c_init();
	_delay_ms(40);
	while(1)
	{
		i2c_start(SLAVE_ADDRESS, 0);
		status = TWSR & 0XF8;
		uart_printstr("START: 0x");
		uart_print_hexa(TWSR & 0xF8);
		uart_printstr("\r\n");
		for (int i =0; i < 6;i++)
		{
			print_hex_value();
			uart_printstr(" ");
		}
		_delay_ms(1000);
		i2c_stop();
	}
}