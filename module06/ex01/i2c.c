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
  	g_status = TWSR & 0xF8;
	if ((TWSR & 0xF8) != (rw ? TW_MR_SLA_ACK : TW_MT_SLA_ACK)) {
		uart_printstr("error in ack\r\n");
		return;
	}
}

// void	i2c_write(unsigned char data, uint8_t rw)
// {
// 	TWDR = data;
// 	TWCR = (1 << TWINT) | (1 << TWEN);
//   	while (!(TWCR & (1 << TWINT)));
// 	if ((TWSR & 0xF8) != 0x28)
// 	{
// 		uart_printstr("I2C error: ");
// 		uart_print_hexa(TWSR & 0xF8);
// 		uart_printstr("\r\n");
// 	}
// }

uint8_t i2c_read_ack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return (TWDR);
}

uint8_t i2c_read_nack() {
    TWCR = (TWCR & ~((1<<TWSTA) | (1<<TWSTO) | (1<<TWEA))) | (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void	i2c_write(unsigned char data) 
{
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  g_status = TWSR & 0xF8;
  if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    // ERROR();
	return;
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