#include <avr/io.h>
#include <util/delay.h>

void	init_timer(short duty_cycle)
{
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); // 
	TCCR1B |=  (1<< WGM13)  | (1 << CS10) | (1 << CS12); // set for pwm and for prescaler at 1024
	ICR1 = (16000000UL / 1024UL) - 1; // set top vlaue
	OCR1A = ICR1 / duty_cycle; // duty cycle
}

int main()
{
	short duty_cycle = 10;
	DDRB = (1 << PB1); // set register DDRB in OUTPUT. 1 for OUTPUT and 0 for INPUT
	DDRD &= ~(1 << PD2) | (1 << PD4);  // set register DDRD in INPUT. PD2 is SWITCH 1. Use & for set 0 at bit 2. USE | for choose SWITCH 1 on PD2 and SWITCH 2 on PD4
	PORTD |= (1 << PD2) | (1 << PD4);  // set register PORTD in HIGH.Use | for set actual value at bit 2 and let the other bits unchanged
	init_timer(duty_cycle);
	while (1)
	{
		if (!(PIND & (1 << PD2)) && duty_cycle < 100) // if switch 1 is pressed and duty cycle less than 100 %
		{
			duty_cycle += 10; // increase
			OCR1A = ICR1 / duty_cycle;
			while (!(PIND & (1 << PD2)))
				_delay_ms(20);
		}

		if (!(PIND & (1 << PD4)) && duty_cycle > 10) // if switch 2 is pressed and duty cycle greater than 10
		{
			duty_cycle -= 10;	// decrease
			OCR1A = ICR1 / duty_cycle; 
			while (!(PIND & (1 << PD4)))
				_delay_ms(20);
		}
	}
	return 0;  // return 0 if the program is successful
}