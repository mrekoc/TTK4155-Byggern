
#include "sram_driver.h"

volatile char *ext_ram = (char *) 0x00; // Start address for the SRAM

void sram_init(void){
	MCUCR |= (1<< SRE );
	SFIOR |= (1<< XMM2);
//	PORTD |= (1<< 6 ) | (1<< 7 );		
}

void sram_write(int address, int data){
	ext_ram [address] = data;  // 0b010011110011 
}