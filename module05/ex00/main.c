#include "atmega328p_common.h"



int main()
{
	ADMUX = (1 << REFS0) | (1 << ADLAR); // use AVCC, set 8 bits
  	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
	  
	  init_uart();
	  while (1)
	  {
		ADCSRA |= (1 << ADSC); // trigger adc conversion
		while (ADCSRA & (1 << ADSC)); // wait until conversion is complete
		uart_print_hexa(ADCH); // print pot value in hexa
		uart_printstr("\r\n");
		_delay_ms(20);
	}	
	return 0;  // return 0 if the program is successfull
}