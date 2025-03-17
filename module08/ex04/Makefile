CC = avr-gcc
PROGRAMMER = arduino
MICROCONTROLLER = atmega328p
F_CPU = 16000000UL
CFLAGS = -Os	-DF_CPU=$(F_CPU) -mmcu=$(MICROCONTROLLER)
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
TARGET = main
PORT = /dev/ttyUSB0
BAUD = 115200
OBJ = $(SRC:.c=.o)

SRC = main.c uart.c utils.c
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
	rm -f $(TARGET).bin $(TARGET).hex $(TARGET).i

.PHONY: all hex flash clean screen