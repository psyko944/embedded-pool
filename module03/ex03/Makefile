CC = avr-gcc
PROGRAMMER = arduino
MICROCONTROLLER = atmega328p
FCPU = 16000000UL
CFLAGS = -Os -DF_CPU=$(FCPU) -mmcu=$(MICROCONTROLLER)
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
TARGET = main
PORT = /dev/ttyUSB0
BAUD = 115200

SRC = main.c uart.c utils.c
OBJ = $(SRC:.c=.o)

all: hex flash

$(TARGET).bin: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

hex: $(TARGET).bin
	$(OBJCOPY) -O ihex $(TARGET).bin $(TARGET).hex

flash:
	$(AVRDUDE) -p $(MICROCONTROLLER) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex:i

screen: all
	screen /dev/ttyUSB0 $(BAUD)
	
clean:
	rm -f $(OBJ)
	rm -f $(TARGET).bin $(TARGET).hex

.PHONY: all hex flash clean screen