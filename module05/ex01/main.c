#include "atmega328p_common.h"



int main()
{
	ADMUX = (1 << REFS0) | (1 << ADLAR); // use AVCC, set 8 bits
  	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
	  
	init_uart();
	while (1)
	{
		// Read POT
		ADMUX = (ADMUX & 0xF8) | 0; 
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		uart_print_hexa(ADCH);
		uart_printstr("\t");

		// Read LDR			
		ADMUX = (ADMUX & 0xF8) | 1; 
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		uart_print_hexa(ADCH); 
		uart_printstr("\t");

		// Read NTC
		ADMUX = (ADMUX & 0xF8) | 2; 
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		uart_print_hexa(ADCH);

		uart_printstr("\r\n");
		_delay_ms(20);
	}	
	return 0;  // return 0 if the program is successfull
}