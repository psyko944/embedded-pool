#include "atmega328p_common.h"


void	*ft_memset(void *s, int c, uint64_t n)
{
	uint64_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i++] = c;
	}
	return (s);
}


char to_upper(char c)  // convert uppercase if lowercase
{
	if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');
    return c;
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

uint8_t	ft_strlen(const char *buffer) // return length of buffer
{
	uint8_t	i = 0;
	while (buffer[i])
		i++;
	return (i);
}

uint8_t is_xdigit(int c) // check good chr for hexadecumal format 
{
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'));
}


uint8_t is_valid_color(const char *buffer) // check color
{
	if (!buffer || ft_strlen(buffer) != 7)
		return 0;
	if (buffer[0] != '#')
		return 0;
	for (uint8_t i = 1; buffer[i];i++)
	{
		if (!is_xdigit(buffer[i]))
			return 0;
	}
	return 1;
}

uint8_t	ft_strlcpy(char *dest, const char *src, uint8_t size) // copy string with \0
{
	uint8_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}

uint8_t	ft_atoi(const char *str)  // convert hexadecimal string to unsigned integer  
{
	uint8_t	i;
	uint16_t	res;
	char base[17] = "0123456789ABCDEF";
	res = 0;
	// while (*str == ' ' || (9 <= *str && *str <= 13))
	// ++str;
	// i = ft_strlen(str) - 1;
	i=0;
	while (is_in_base(to_upper(*str), base) != -1)
	{
		res = res * 16 + (is_in_base(to_upper(*str), base));
		str++;
	}
	return (res);
}