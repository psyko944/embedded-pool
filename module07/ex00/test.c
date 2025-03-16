#include <stdint.h>

int main()
{
	uint8_t v = 64;
	printf("%lf\n", (double)v >> 28);
	return (0);
}
