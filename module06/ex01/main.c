#include "atmega328p_common.h"

#define SLAVE_ADDRESS 0x38
uint8_t	g_status;
void	print_hex_value(char c)
{
	uart_tx(((c >> 4) & 0x0F) > 9 ? ((c >> 4) & 0x0F) - 10 + 'A' : ((c >> 4) & 0x0F) + '0');
    uart_tx((c & 0x0F) > 9 ? (c & 0x0F) - 10 + 'A' : (c & 0x0F) + '0');
}

uint8_t	check_sensor_calibrated()
{
	uint8_t	data;
	uart_printstr(BLUE);
	uart_printstr("Calibration Sensor:")
	uart_printstr(RESET);
	i2c_start(SLAVE_ADDRESS, TH_WRITE);
	i2c_write(0x71);
	g_status = TWSR & 0xF8;
	if (g_status !+= TW_MT_DATA_ACK)
	{
		uart_printstr(RED);
		uart_printstr("Error: No ACK received for data.");
		uart_printstr(RESET);
		return (0);
	}
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
		_delay_ms(2000);
		i2c_stop();
	}
}