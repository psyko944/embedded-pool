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


all: hex flash

hex:
	$(CC) $(CFLAGS) -E $(TARGET).c > $(TARGET).i
	$(CC) $(CFLAGS) -o $(TARGET).bin $(TARGET).i
	$(OBJCOPY) -O ihex $(TARGET).bin $(TARGET).hex

flash:
	$(AVRDUDE) -p $(MICROCONTROLLER) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex:i

screen: all
	screen /dev/ttyUSB0 $(BAUD)
	
clean:
	rm -f $(TARGET).bin $(TARGET).hex $(TARGET).i

.PHONY: all hex flash clean screen