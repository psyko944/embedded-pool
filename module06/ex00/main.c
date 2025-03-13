#include "atmega328p_common.h"
#define slave_address 0x38;
void	i2c_stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void	i2c_init()
{
	TWSR = 0x00;
	TWBR = 72;
	TWCR = (1 << TWEN);	
}

void	i2c_start(void)
{
	TWCR = (1 << TWINT) | (1 <<TWSTA) | ( 1<< TWEN);
	while (!(TWCR & (1 << TWINT)));
}

int main()
{
	uint8_t status;
	init_uart();
	i2c_init();
	while(1)
	{
		i2c_start();
		status = TWSR & 0XF8;
		uart_printstr("START: 0x");
		uart_print_hexa(TWSR & 0xF8);
		uart_printstr("\r\n");
		_delay_ms(1000);
		i2c_stop();
	}
}