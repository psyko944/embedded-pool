#ifndef ATMEGA328P_COMMON_H
# define ATMEGA328P_COMMON_H
# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>
#include <util/twi.h>
# define PRESCALER 1024UL
# define UBRRNL 8
# define R (1 << PD5)
# define G (1 << PD6)
# define B (1 << PD3)
# define D1 (1 << PB0)
# define D2 (1 << PB1)
# define D3 (1 << PB2)
# define D4 (1 << PB4)

# define Y R | G
# define C B | G
# define M R | B
# define W R | G | B
# define DEF_COLOR "\033[0;39m"
# define GRAY "\033[0;90m"
# define RED "\033[0;91m"
# define GREEN "\033[0;92m"
# define SUPA_GREEN "\033[1;92m"
# define SUPA_PURPLE "\033[1;95m"
# define SUPA_RED "\033[1;91m"
# define YELLOW "\033[0;93m"
# define BLUE "\033[0;94m"
# define MAGENTA "\033[0;95m"
# define CYAN "\033[0;96m"
# define WHITE "\033[0;97m"
# define PURPLE "\033[38;2;255;105;180m"
# define RESET "\033[0m"

uint8_t g_status;
//		uart functions
void	init_uart();
void	uart_tx(char c);	// transmit character on serial port
void	uart_printstr(const char* str); // transmit string on serial port
char	uart_rx(void);  // read character from serial port
char	read_input(char *buffer, int mode); // read input and store in buffer
void	uart_print_number(long number);
void	uart_print_hexa(uint8_t number); // print number in hex on serial port

//		libft functions
uint8_t	ft_strlen(const char *buffer);
uint8_t is_xdigit(int c);
uint8_t is_valid_color(const char *buffer);
uint8_t	ft_atoi(const char *str);
char	to_lower(char c); // convert Uppercase in lowercase
uint8_t	ft_strlcpy(char *dest, const char *src, uint8_t size);
char	*ft_strchr(const char *str, int c);
int16_t	parse_values(const char *input);
int		ft_strcmp(const char *s1, const char *s2); // compare string
int		get_cmd(const char *input, char *cmd);
char	*get_token(char *input, int *index);
uint8_t is_printable(char c);

//		i2c functions
void	i2c_stop();
void	i2c_init();
void	i2c_start(uint8_t address, uint8_t rw);
void	i2c_write(unsigned char data);
void	i2c_read(void);
uint8_t i2c_read_nack();
uint8_t i2c_read_ack();


// utils function

uint8_t is_valid_key(const char *str);
void	print_color(const char *str, const char *color);
void	print_error(const char *str);
void	print_address(uint16_t addr);
void	print_hex_value(char c);
void	byte_to_hex(uint8_t byte, char* hex_str);

#endif