#include "atmega328p_common.h"

int	ft_strcmp(const char *s1, const char *s2) // compare string
{
	uint64_t	i;

	i = 0;
	while (s1[i] &&  s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - s2[i]);
}

char read_input(char *buffer, int mode) {  // read input and store in buffer
  int i = 0;
  while (i < 255) {
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

int	validate_prompt(char ursename_buffer[255])
{
	uart_printstr("Hello ");
	uart_printstr(ursename_buffer);
	DDRD &= ~(1 << PD2) | ~(1 << PD4);
	PORTD &= ~(1 << PD5); // light on Green led if we fill good login and password 
	PORTD |= (1 << PD6);
	PORTD |= (1 << PD2) | (1 << PD4);
	uart_printstr("\r\nShall we play a game?\r\n");
	uart_printstr("Press Switch 1 for replay, Switch 2 for end\r\n");
	while (1)
	{
		if (!(PIND & (1 << PD2))) // if SWITCH 1 is pressed
			return (PORTD &= ~(1 << PD6),1);
		if (!(PIND & (1 << PD4))) // if SWITCH 2 is pressed
			return (0);
	}
}

void	init_leds()
{
	DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4); // initialize the leds that are on register b
	DDRD = (1 << PD5) | (1 << PD6); // initialize the leds that are on register b. its for RGB led.
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
		uart_printstr("Enter your login:");
		uart_printstr("\r\n   ursename: ");
		read_input(ursename_buffer,1);
		uart_printstr("   password: ");
		read_input(password_buffer,0);
		if (!ft_strcmp(ursename, ursename_buffer) && !ft_strcmp(password,password_buffer))
		{
			if (!validate_prompt(ursename_buffer))
			{
				uart_printstr("\r\nGoodbye !\r\n");
				PORTD &= ~(1 << PD5) & ~(1 << PD6);
				break;
			}
			else
				continue;
		}
		else
		{
			PORTD &= ~(1 << PD6);
			PORTD |= (1 << PD5);  // light on Red led we give the wrong combinaison
			uart_printstr("Bad combinaison username/password\r\n");
		}
		uart_printstr("\r\n");
	}
	return 0;  // return 0 if the program is successfull
}