#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define PRESCALER 1024UL
#define UBRRNL (((float)(F_CPU) / (16UL * BAUD)) - 1)

void	init_uart()
{
	UBRR0H = (unsigned char)(8 >> 8);
    UBRR0L = (unsigned char)8;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (3 << UCSZ00);
}

void	uart_tx(char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}

void uart_printstr(const char* str)
{
	while (*str)
		uart_tx(*str++);
}

char uart_rx(void)
{
	while (!(UCSR0A & (1<<RXC0)))
	{
	}
	return UDR0;
} 

int	check_str(const char *str, char *buffer)
{

	int i = -1;
	while (str[++i] && buffer[i])
	{
		if (str[i] != buffer[i])
			return 0;
	}
	return 1;
}

char read_input(char *buffer, int mode) {
  int i = 0;
  while (i < 255) {
    char c = uart_rx();
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
        uart_printstr("\b \b");
      }
    }
	else
	{
    	buffer[i++] = c;
		(mode == 1) ? uart_tx(c) : uart_tx('*');
    }
  }
  buffer[i] = '\0';
  uart_printstr("\r\n");
  return 0;
}

int	validate_prompt(char ursename_buffer[255])
{
	char response_buffer[4];
	uart_printstr("Hello ");
	uart_printstr(ursename_buffer);
	PORTB |= (1 << PB0);
	while (1)
	{
		uart_printstr("\r\nShall we play a game?\r\n");
		read_input(response_buffer,1);
		if (check_str(response_buffer, "yes"))
			return 1;
		else if (check_str(response_buffer, "no"))
		{
			return 0;
		}
	}
}

void	init_leds()
{
	DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
}

int main()
{
	const char	ursename[] = "login";
	const char	password[] = "login";
	char ursename_buffer[255];
	char password_buffer[255];
	init_leds();
	init_uart();
	while (1)
	{
		PORTB &= ~(1 << PB0);
		uart_printstr("Enter your login:");
		uart_printstr("\r\n   ursename: ");
		read_input(ursename_buffer,1);
		uart_printstr("   password: ");
		read_input(password_buffer,0);
		if (check_str(ursename, ursename_buffer) && check_str(password,password_buffer))
		{
			if (!validate_prompt(ursename_buffer))
			{
				PORTB |= (1 << PB4);
				return (0);
			}
			else
				continue;
		}
		else
			uart_printstr("Bad combinaison username/password\r\n");
		uart_printstr("\r\n");
	}
	return 0;  // return 0 if the program is successfull
}