#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define PRESCALER 1024UL
#define UBRRNL (((float)(F_CPU) / (16UL * BAUD)) - 1)

char toggle_case(char c)
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
	UBRR0H = (unsigned char)(8 >> 8);
    UBRR0L = (unsigned char)8;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (3 << UCSZ00);
	sei();
}

void	uart_tx(char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}

ISR(USART_RX_vect) 
{	unsigned char c = UDR0;
	uart_tx(toggle_case(c));
}

int main()
{
	init_uart();
	while(1);
	return 0;  // return 0 if the program is successfull
}