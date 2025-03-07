#ifndef ATMEGA328P_COMMON_H
# define ATMEGA328P_COMMON_H
# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>
# define BAUD 115200
# define CPU 16000000UL
# define PRESCALER 1024UL
# define UBRRNL 8


//		uart functions
void	init_uart();
void	uart_tx(char c);	// transmit character on serial port
void	uart_printstr(const char* str); // transmit string on serial port
char	uart_rx(void);  // read character from serial port

#endif