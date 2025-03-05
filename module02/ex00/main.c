#include <avr/io.h>
#include <util/delay.h>
#define BAUD 115200
#define CPU 16000000UL
#define UBRRNL (((float)(F_CPU) / (16UL * BAUD)) - 1)
void	init_uart()
{
	UBRR0H = (unsigned char)(8 >> 8);
    UBRR0L = (unsigned char)UBRRNL;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (3 << UCSZ00);
}

void	uart_tx(char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}

int main()
{
	init_uart();
	while (1)
	{
		uart_tx('A');
		_delay_ms(1000);
	}
	return 0;  // return 0 if the program is successfull
}