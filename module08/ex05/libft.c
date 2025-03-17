#include "atmega328p_common.h"

char	*ft_strchr(const char *str, int c)
{
	char	*retval;
	int		i;

	if (!str)
		return (0);
	retval = (char *)str;
	i = 0;
	while (retval[i] != (char)c)
	{
		if (retval[i] == '\0')
			return (0);
		i++;
	}
	return (retval + i);
}

uint8_t is_printable(char c)
{
	return (c >= 32 && c < 127);
}

uint8_t ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

char to_lower(char c) {  // convert Uppercase in lowercase
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A'); 
    return c;
}

char to_upper(char c) {  // convert Uppercase in lowercase
    if (c >= 'a' && c <= 'z')
        return c + ('A' - 'a'); 
    return c;
}

uint8_t is_xdigit(int c) // check good chr for hexadecumal format 
{
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'));
}

int	is_in_base(char c, char *base) // check if is in base and return index if this is the case
{
	int	i;

	i = 0;
	while (base[i] != '\0')
	{
		if (c == base[i])
			return (i);
		++i;
	}
	return (-1);
}

int16_t	parse_values(const char *input)
{
	if (!input)
		return (0);
	int16_t	res=0;
	int	i = 0;
	int8_t sign = 0;
	char base[17] = "0123456789abcdef";
	while (input[i] == ' ' || input[i] == ' ')
		i++;
	if (input[i] == '-' || input[i] == '+')
	{
		if (input[i] == '-')
			sign = 1;
		i++;
	}
	while (input[i])
	{
		if (!is_xdigit(input[i]))
			break;
		res = res * 16 +  is_in_base(to_lower(input[i]), base);
		i++;
	}
	return sign ? -res : res;
}

int	get_cmd(const char *input, char *cmd)
{
	int i = 0;
	int j = 0;
	while (ft_isspace(input[i]))
		i++;
	// uart_printstr(input + i);
	while (input[i] && !ft_isspace(input[i]))
	{
		cmd[j++] = input[i];
		i++;
	}
	cmd[j] = '\0';
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2) // compare string
{
	uint64_t	i;

	i = 0;
	while (s1[i] &&  s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - s2[i]);
}


char *get_token(char *input, int *index)
{

	while (input[*index] && ft_isspace(input[*index]))
	{
		(*index)++;
	}
	int i = *index;
	while (input[*index] && !ft_isspace(input[*index]))
		(*index)++;
	input[*index] = 0;
	return input + i;
}