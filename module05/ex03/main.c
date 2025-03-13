#include "atmega328p_common.h"



int main()
{
	ADMUX = (1 << REFS0) | (1 << REFS1) | (1 << MUX3); // register of multiplexer ADC, we set on 1.1 volt for measure internal temp.
  	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
	long gross_value;
	long temp;
	init_uart();
	while (1)
	{
		// Read internal temp
		ADMUX = (ADMUX & 0xF8) | 0; // mask for set to 0 3 least significant bit and set last bit at 0
		ADCSRA |= (1 << ADSC); // trigger ADC conversion
		while (ADCSRA & (1 << ADSC)); // wait until conversion is complete 
		gross_value = ADC; // value in ADC
		temp = (gross_value - 324.31) / 1.22; // conversion in celcius
		uart_print_number(temp);
		uart_printstr("\r\n");
		_delay_ms(20);
	}	
	return 0;  // return 0 if the program is successfull
}