#include "MCP2515_driver.h"
#include "bit_macros.h"
#include "SPI_driver.h"
#include "mcp2515_constants.h"
#include <avr/io.h>
#include <util/delay.h>

char MCP2515_read(char adress) {
	char data;
	clear_bit(PORTB, DDB4);
	SPI_send(MCP_READ);
	SPI_send(adress);
	data=SPI_read();
	set_bit(PORTB, DDB4);
	return data;
}

void MCP2515_write(char adress, char data){
	clear_bit(PORTB, PB4);
	SPI_send(CRP_WRITE);
	SPI_send(adress);
	SPI_send(data);
	set_bit(PORTB, PB4);
	return data;
}

char MCP2515_init() {
	char value;
	SPI_init();//initialize SPI
	MCP2515_reset(); //Reset MCP
	MCP2515_write(MCP_CNF3, 0x01);   // bit 7 SOF, bit6 WKO, 0-2, PS2 length
	MCP2515_write(MCP_CNF2, 0xb5);	  // bit 7 = determined by cnf3 bit 6 = 3 samples bit 3-5 ps1 length 0-3 propagation length
	MCP2515_write(MCP_CNF1, 0x43);
	value=MCP2515_read(MCP_CANSTAT);
	
	if((value & MODE_MASK) != MODE_CONFIG) {
		printf("\n\r MCP2515 is NOT in configuration mode after reset!\n\r");
		return 1;
	}
	
	clear_bit(PORTB, DDB4); //Activate MCP
	MCP2515_write(MCP_CANCTRL, MODE_NORMAL); // Set normal mode since we are going to communicate with node 2
	
	set_bit(PORTB, DDB4);//Deactivate MCP
	return 0;
}

void MCP2515_request_to_send(int buffernr) {
	clear_bit(PORTB, PB4);
	switch(buffernr) {
		case 0: SPI_send(MCP_RTS_TX0); break;
		case 1: SPI_send(MCP_RTS_TX1); break;
		case 2: SPI_send(MCP_RTS_TX2); break;
		case 3: SPI_send(MCP_RTS_ALL); break;
	}
	set_bit(PORTB, PB4);
}

void MCP2515_bit_modify(char adress, char mask, char data){
	clear_bit(PORTB, PB4);
	SPI_send(MCP_BITMOD);
	SPI_send(adress);
	SPI_send(mask);
	SPI_send(data);
	set_bit(PORTB, PB4);
}

void MCP2515_reset() {
	clear_bit(PORTB, PB4);
	//_delay_ms(500);
	SPI_send(MCP_RESET);
	set_bit(PORTB, PB4);
	//_delay_ms(20);
}

char MCP2515_read_status() {
	clear_bit(PORTB, PB4);
	SPI_send(MCP_READ_STATUS);
	char data = SPI_read();
	set_bit(PORTB, PB4);
	return data;
}

