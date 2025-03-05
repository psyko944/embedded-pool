#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#ifndef FCPU
	#define FCPU 16000000UL
#endif
#define UBRRNL (((float)(FCPU) / (16UL * BAUD)) - 1)
#define PRESCALER 1024UL
void	init_timer()
{
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
	OCR1A = (FCPU / PRESCALER) * 2;
	TIMSK1 |= (1 << OCIE1A);
	sei();
}

void	init_uart()
{
	UBRR0H = (unsigned char)(8 >> 8);
    UBRR0L = (unsigned char)8;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (3 << UCSZ00);
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_printstr(const char* str)
{
	while (*str)
		uart_tx(*str++);
}

int main()
{
	init_uart();
	init_timer();
	uart_printstr("Hello World !");
	uart_printstr("\n\r");
	while(1);
	return 0;  // return 0 if the program is successfull
}