#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 115200
#define CPU 16000000UL
#define R PD5
#define G PD6
#define B PD3

int main()
{
	DDRD = (1 << R);
	PORTD |= (1 << R);
	while (1);	
	return 0;  // return 0 if the program is successfull
}