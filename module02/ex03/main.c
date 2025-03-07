#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define PRESCALER 1024UL
#define UBRRNL 8

char toggle_case(char c) // toggle lower if upper, toggle upper if lower
{
  if (c >= 'A' && c <= 'Z')
    return c + 32; 
  else if (c >= 'a' && c <= 'z') 
    return c - 32;
  else
    return c;
}

void	init_uart()
{
	UBRR0H = (unsigned char)(UBRRNL >> 8); // set the high byte of the USART Baud Rate Register 0
    UBRR0L = (unsigned char)UBRRNL; // Set the low byte of the USART Baud Rate Register 0
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // enable transmit and read from serial port. enable interruption with RXCIE0
	UCSR0C = (3 << UCSZ00);  // configures the USART to use 8bits,  no parity, 1 bit stop
	sei(); // enable global interruption
}

void	uart_tx(char c) // transmit character on serial port
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}

ISR(USART_RX_vect) // what happens when the interrupt is triggered
{	unsigned char c = UDR0;
	uart_tx(toggle_case(c));
}

int main()
{
	init_uart();
	while(1);
	return 0;  // return 0 if the program is successfull
}