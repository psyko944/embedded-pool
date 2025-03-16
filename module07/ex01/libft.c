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

char to_lower(char c) {  // convert Uppercase in lowercase
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A'); 
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
