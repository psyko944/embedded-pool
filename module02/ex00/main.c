#include <avr/io.h>
#include <util/delay.h>
#define BAUD 115200
#define CPU 16000000UL
#define UBRRNL 8

void	init_uart()
{
	UBRR0H = (unsigned char)(UBRRNL >> 8); // set the high byte of the USART Baud Rate Register 0
    UBRR0L = (unsigned char)UBRRNL; // Set the low byte of the USART Baud Rate Register 0 
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable transmit and read from serial port
	UCSR0C = (3 << UCSZ00); // configures the USART to use 8bits,  no parity, 1 bit stop
}

void	uart_tx(char c) // transmit a character on serial port
{
	while ((UCSR0A & (1 << UDRE0)) == 0); // wait until the buffer is empty
	UDR0 = c; // write c in the register UDR0 which send into serial port 
}

int main()
{
	init_uart();
	while (1)
	{
		uart_tx('Z');	// print Z every second
		_delay_ms(1000);
	}
	return 0;  // return 0 if the program is successfull
}