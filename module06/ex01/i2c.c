#include "atmega328p_common.h"

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

void	i2c_start(uint8_t address, uint8_t rw)
{

	TWCR = (1 << TWINT) | (1 <<TWSTA) | ( 1<< TWEN);
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START) {
		uart_printstr("error in start transmission\r\n");
		return;
	}
	TWDR = (address << 1) | rw;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != (rw ? TW_MR_SLA_ACK : TW_MT_SLA_ACK)) {
		uart_printstr("error in ack\r\n");
	}
}

void	i2c_write(unsigned char data, uint8_t rw)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
  	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != 0x28)
	{
		uart_printstr("I2C error: ");
		uart_print_hexa(TWSR & 0xF8);
		uart_printstr("\r\n");
	}
}

void i2c_read(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
	uart_printstr("0x");
	uart_print_hexa(TWSR & 0xF8);
	uart_printstr("\r\n");
}