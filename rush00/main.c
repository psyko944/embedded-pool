#include "atmega328p_common.h"
#define SLAVE_ADDRESS 0x08 // Shared address
#define WIN_LED PD6
#define LOSE_LED PD5
volatile uint8_t win = 0;
volatile uint8_t transmitting = 0;

void	twi_putchar(uint8_t data) 
{
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
  if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    // ERROR();
	return;
  }
}

void	twi_start(uint8_t address, uint8_t rw)
{
	TWCR = (1 << TWINT) | (1 <<TWSTA) | ( 1<< TWEN);
	while (!(TWCR & (1 << TWINT)));
	uart_printstr("we will read data\r\n");
	if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START) {
		return;
	}
	TWDR = (address << 1) | rw;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) != (rw ? TW_MR_SLA_ACK : TW_MT_SLA_ACK)) {
		return;
	}
}

void	twi_stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void	init_twi()
{
	TWSR = 0x00;
	TWBR = 72;
	// DDRC = (1 << PC4) | (1 << PC5);	
}

void twi_slave_init(uint8_t address) {
  TWAR = (address << 1); // Définir l'adresse de l'esclave
  TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
}

void	init_leds()
{
	DDRD = R | G | B; // init 
}

int main()
{
	init_leds();
	init_uart();
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);
	init_twi();
	twi_slave_init(SLAVE_ADDRESS);
	while (1)
	{
		if (!(PIND & (1 << PD2)) && win == 0) 
		{ 
			_delay_ms(50);
			if (!(PIND & (1 << PD2)))
			{
				transmitting = 1;
				twi_start(SLAVE_ADDRESS, TW_WRITE);
				twi_putchar('C');
				twi_stop();
				transmitting = 0;
				win = 1; // Assume win until proven otherwise
			}
		}
		uart_printstr("test");
		while (!(TWCR & (1 << TWINT)) && !win);
		if (!win)
		{
			switch(TWSR & 0xF8)
			{
				case TW_SR_SLA_ACK:
					TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
					break;
				case TW_SR_DATA_ACK:
					if (TWDR == 'C')
						if (!transmitting)
							win = 2;
					else
						uart_printstr("perdu");
					TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
					break;
				case TW_SR_STOP:
                    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
					break;
				default: 
                    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
					break;
			}
		}
		if (win == 1)
		{
			uart_printstr("win");
			PORTD |= (1 << WIN_LED);
			PORTD &= ~(1 << LOSE_LED);
		}
		else if (win == 2)
		{
			PORTD |= (1 << LOSE_LED);
			PORTD &= ~(1 << WIN_LED);
		}
		if (win != 0)	break;
	}
}