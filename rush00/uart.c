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

void uart_print_number(uint8_t number) // print number on serial port
{
    char buffer[4];
    int i = 0;

    if (number == 0) {
        uart_tx('0');
        return;
    }
    while (number > 0) {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }
    while (i > 0) {
        uart_tx(buffer[--i]);
    }
}

char read_input(char *buffer, int mode)
{  // read input and store in buffer
  int i = 0;
  while (i < 7)
  {
	PORTB &= ~(1 << PB4) & ~(1 << PB1); // clear leds
    char c = uart_rx();
	if (c == '\x1B') // if we press arrow
	{
		uart_rx();
		uart_rx();
		continue;
	}
    if (c == '\r' || c == '\n')
	{
      buffer[i] = '\0';
	  uart_printstr("\r\n");
      return 1;
    } 
	else if (c == 127 || c == 8)
	{ 
      if (i > 0)
	  {
        i--;
		PORTB |= (1 << PB4); // light on led D4 when i press backspace
        uart_printstr("\b \b");
		_delay_ms(20);
      }
    }
	else
	{
    	buffer[i++] = c;
		PORTB |= (1 << PB1); // light on led D2 when i press a key
		_delay_ms(20);
		(mode == 1) ? uart_tx(c) : uart_tx('*'); // output chr if we type ursename otherwise type * for password 
    }
  }
  buffer[i] = '\0';
  uart_printstr("\r\n");
  return 0;
}