#include "atmega328p_common.h"

#define SLAVE_ADDRESS 0x08 // Adresse de l'esclave

void twi_slave_init(uint8_t address) {
  TWAR = (address << 1); // Définir l'adresse de l'esclave
  TWCR = (1 << TWEA) | (1 << TWEN); // Activer TWI et ACK
}

int main(void) 
{
  DDRB = (1 << PB0) | (1 << PB4);
  twi_slave_init(SLAVE_ADDRESS);
  uint8_t data;
  init_uart();
  while (1) 
  {
    while (!(TWCR & (1 << TWINT))); // Attendre une interruption TWI
    if ((TWSR & 0xF8) == TW_SR_SLA_ACK || (TWSR & 0xF8) == TW_SR_DATA_ACK)
    {
      data = TWDR;
      // Traiter les données reçues ici
      if ((TWSR & 0xF8) == TW_SR_DATA_ACK)
      {
          // if (data == 'A')
          //   PORTB |= (1 << PB0);
          // else
          //   PORTB |= (1 << PB4);
          if (data == 'C')
          {
            PORTB |= (1 << PB0);
            uart_tx(data);
          }
          else
            PORTB = 0;
      }
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Répondre avec ACK
    }
    else
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Répondre avec ACK
  }
}