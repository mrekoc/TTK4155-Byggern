#include <avr/io.h>
#include "SPI_driver.h"
#include "bit_macros.h"
#include "mcp2515_constants.h"

void SPI_init(void) {
	
	DDRB |= (1<<PB5)|(1<<PB7)|(1<<PB4);
	DDRB &= ~(1<<PB6);
	
	//Enable SPI, setter Atmega til master, setter clock rate til f/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	set_bit(PORTB, PB4);

}


void SPI_send(char data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

char SPI_read() {
	SPDR=0;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
