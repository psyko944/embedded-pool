#include "atmega328p_common.h"

#define SLAVE_ADDRESS 0x38
#define ERROR 0xFF

void	print_hex_value(char c)
{
	uart_tx(((c >> 4) & 0x0F) > 9 ? ((c >> 4) & 0x0F) - 10 + 'A' : ((c >> 4) & 0x0F) + '0');
	uart_tx((c & 0x0F) > 9 ? (c & 0x0F) - 10 + 'A' : (c & 0x0F) + '0');
}

void	print_hex_str(const uint8_t *str, uint8_t length)
{
	for (uint8_t i = 0; i < length;i++)
	{
		print_hex_value(str[i]);
		if (i < length - 1)
			uart_tx(' ');
	}
	uart_printstr("\r\n");
}



void	calibrate(void)
{
        uart_printstr("Sensor not calibrated, calibrating...\r\n");
        i2c_start(SLAVE_ADDRESS, TW_WRITE);
        i2c_write(0xBE);
        if (g_status != TW_MT_DATA_ACK) {
            uart_printstr(RED);
            uart_printstr("Error: No ACK for calibration command.\r\n");
            uart_printstr(RESET);
            i2c_stop();
			g_status = ERROR; 
            return ;
        }
        i2c_write(0x08);
        if (g_status != TW_MT_DATA_ACK) {
            uart_printstr(RED);
            uart_printstr("Error: No ACK for calibration data 1.\r\n");
            uart_printstr(RESET);
            i2c_stop();
			g_status = ERROR; 
            return ;
        }
        i2c_write(0x00);
        if (g_status != TW_MT_DATA_ACK) {
            uart_printstr(RED);
            uart_printstr("Error: No ACK for calibration data 2.\r\n");
            uart_printstr(RESET);
            i2c_stop();
			g_status = ERROR; 
            return ;
        }
        i2c_stop();
		g_status = 0;
        _delay_ms(10); // Wait 10ms after calibration
        //Indicate calibration was performed.
    }

uint8_t	check_sensor_calibrated()
{
	uint8_t	data;
	uart_printstr(BLUE);
	uart_printstr("Calibration Sensor:\r\n");
	uart_printstr(RESET);
	i2c_start(SLAVE_ADDRESS, TW_WRITE);
	i2c_write(0x71);
	g_status = TWSR & 0xF8;
	if (g_status != TW_MT_DATA_ACK)
	{
		uart_printstr(RED);
		uart_printstr("Error: No ACK received for data.");
		uart_printstr(RESET);
		return (0);
	}
	i2c_start(SLAVE_ADDRESS, TW_READ);
	data = i2c_read_nack();
	g_status = TWSR & 0xF8;
	if (g_status != TW_MR_DATA_NACK)
	{
		uart_printstr(RED);
		uart_printstr("Error: Read with NACK failed. Status: ");
		uart_print_hexa(g_status);
		uart_printstr("\r\n");
		uart_printstr(RESET);
		return (1);
	}
	i2c_stop();
	if ((data & 0x08) == 0)
		return 0;
	uart_printstr(GREEN);
	uart_printstr("Sensor already calibrated\r\n");
	uart_printstr(RESET);
	return (1);
}


uint8_t trigger_measurement()
{
	i2c_start(SLAVE_ADDRESS, TW_WRITE);
	if (g_status != TW_MT_SLA_ACK)
	{
		i2c_stop();
		return g_status;
	}
	i2c_write(0xAC);
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		i2c_stop();
		return (TWSR & 0xF8); // return the error code
    }
	i2c_write(0x33);
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		i2c_stop();
		return (TWSR & 0xF8);
	}
	i2c_write(0x00);
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	{
		i2c_stop();
		return (TWSR & 0xF8);
	}
	i2c_stop();
	return (0); // Success
}

int main()
{
	g_status = 0;
	uint8_t	measurement_array[7];
	init_uart();
	_delay_ms(20);
	i2c_init();
	_delay_ms(40);
	if (!check_sensor_calibrated())
	{	
		g_status = ERROR;
		while (g_status == ERROR)
			calibrate();
	}
	while(1)
	{
		if (!trigger_measurement())
		{
			_delay_ms(80);
			i2c_start(SLAVE_ADDRESS, TW_READ);
			for (int i = 0;i < 6; i++)
			{
				measurement_array[i] = i2c_read_ack();
			}
			measurement_array[6] = i2c_read_nack();
			i2c_stop();
			print_hex_str(measurement_array, 7);
		}
		else
		{
			uart_printstr(RED);
			uart_printstr("Error: trigger measurement\r\n");
			uart_printstr(RESET);
		}
		_delay_ms(2000);
	}
}