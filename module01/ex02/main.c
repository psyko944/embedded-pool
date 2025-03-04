#include <avr/io.h>
#include <util/delay.h>

void	init_timer()
{
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); // set COM1A1 at 1 and WGM13 for use PMW. WGM11 is on register a 
	TCCR1B |=  (1<< WGM13)  | (1 << CS10) | (1 << CS12); // set for pwm and for prescaler at 1024. WGM13 is on register b
	ICR1 = (16000000UL / 1024UL) - 1; // set top value
	OCR1A = ICR1 / 10; // duty cycle
}

int main()
{
	DDRB = (1 << PB1); // set register DDRB in OUTPUT. 1 for OUTPUT and 0 for INPUT
	DDRD &= ~(1 << PD2) | (1 << PD4);  // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2. USE | for choose SWITCH 1 on PD2 and SWITCH 2 on PD4
	PORTD |= (1 << PD2) | (1 << PD4);  // set register PORTD in HIGH.Use | for set actual value at bit 2 and let the other bits unchanged
	init_timer(); // initialize timer for light on 10 % upon 1hz 
	while (1)
	{
	}
	return 0;  // return 0 if the program is successful
}