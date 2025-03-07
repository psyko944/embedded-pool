#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#ifndef FCPU
	#define FCPU 16000000UL
#endif
#define UBRRNL 8 
#define PRESCALER 1024UL
void	init_timer() // timer in ctc mode
{
	TCCR1A |= (1 << COM1A0); // this mask determines behavior when we reach the value stored in OCR1A
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12); // set mask for ctc mode and prescaler 
	OCR1A = (FCPU / PRESCALER) * 2; // set OCR1A for print every 2 second
	TIMSK1 |= (1 << OCIE1A); // enable compare match interrupt for timer1
	sei(); // enable global interruptions
}

void	init_uart()
{
	UBRR0H = (unsigned char)(UBRRNL >> 8); // set the high byte of the USART Baud Rate Register 0
    UBRR0L = (unsigned char)UBRRNL; // Set the low byte of the USART Baud Rate Register 0 
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enable transmit and read from serial port
	UCSR0C = (3 << UCSZ00); // configures the USART to use 8bits,  no parity, 1 bit stop
}

void	uart_tx(char c) // transmit character on serial port
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_printstr(const char* str) // transmit string on serial port
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