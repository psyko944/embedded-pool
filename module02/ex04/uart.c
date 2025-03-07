#include "atmega328p_common.h"

void	init_uart()
{
	UBRR0H = (unsigned char)(UBRRNL >> 8); // set the high byte of the USART Baud Rate Register 0
    UBRR0L = (unsigned char)UBRRNL; // Set the low byte of the USART Baud Rate Register 0 
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable transmit and read from serial port
	UCSR0C = (3 << UCSZ00); // configures the USART to use 8bits,  no parity, 1 bit stop
}

void	uart_tx(char c)	// transmit character on serial port
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}

void uart_printstr(const char* str) // transmit string on serial port
{
	while (*str)
		uart_tx(*str++);
}

char uart_rx(void)  // read character from serial port
{
	while (!(UCSR0A & (1<<RXC0)))
	{
	}
	return UDR0;
} 