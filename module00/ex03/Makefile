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


all: hex flash

hex:
	$(CC) $(CFLAGS) -o $(TARGET).bin $(TARGET).c
	$(OBJCOPY) -O ihex $(TARGET).bin $(TARGET).hex

flash:
	$(AVRDUDE) -p $(MICROCONTROLLER) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex:i

clean:
	rm -f $(TARGET).bin $(TARGET).hex