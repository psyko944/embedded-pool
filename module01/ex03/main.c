#include <avr/io.h>
#include <util/delay.h>
#ifndef FCPU
	#define FCPU 16000000UL
#endif
#define PRESCALER 1024UL

void	init_timer(float duty_cycle)
{
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);  // set COM1A1 at 1 and WGM11 for use PMW. WGM11 is on register a
	TCCR1B |=  (1<< WGM13)  | (1 << CS10) | (1 << CS12); // set for pwm and for prescaler at 1024. WGM13 is on register b
	ICR1 = (FCPU / PRESCALER) -1; // set top value
	OCR1A = ICR1 * duty_cycle; // light on while duty cycle time
}

int main()
{
	float duty_cycle = 0.1;
	DDRB = (1 << PB1) | (1 << PB4); // set register DDRB in OUTPUT. 1 for OUTPUT and 0 for INPUT
	DDRD &= ~(1 << PD2) | (1 << PD4);  // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2. USE | for choose SWITCH 1 on PD2 and SWITCH 2 on PD4
	PORTD |= (1 << PD2) | (1 << PD4);  // set register PORTD in HIGH.Use | for set actual value at bit 2 and let the other bits unchanged
	init_timer(duty_cycle); // set register and calculate duty cycle
	while (1)
	{
		if (!(PIND & (1 << PD2))) // if switch 1 is pressed and duty cycle less than 100 %
		{
			duty_cycle += 0.1; // increase
			if (duty_cycle > 1)
				duty_cycle = 1;
			OCR1A = (ICR1 * duty_cycle);
			while (!(PIND & (1 << PD2))); // add delay for avoid bouncing
			_delay_ms(50);
		}
		if (!(PIND & (1 << PD4))) // if switch 2 is pressed and duty cycle greater than 10
		{
			
			duty_cycle -= 0.1;	// decrease
			if (duty_cycle < 0.1)
				duty_cycle = 0.1;
			OCR1A = (ICR1 * duty_cycle);
			while (!(PIND & (1 << PD4)));
			_delay_ms(50);
		}
	}
	return 0;  // return 0 if the program is successful
}